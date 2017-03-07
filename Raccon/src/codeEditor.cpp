#include <QtWidgets>

#include "codeEditor.h"

// 行号区宽度(像素)
#define LINE_NUMBER_AREA_BASE_WIDTH 15
// 行号区数字位置(像素)
#define LINE_NUMBER_AREA_NUM_POS -7

/* 行号显示控件 */
class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor)
    {// 插入代码编辑器中
        codeEditor = editor;

    }

    QSize sizeHint() const Q_DECL_OVERRIDE
    {// 行号宽
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE
    {// 行号实时显示
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private: // 父控件：代码编辑器
    CodeEditor *codeEditor;
};

/* 代码编辑器控件 */

// @overrride
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{// 编辑器主体框架
    lineNumberArea = new LineNumberArea(this);  // 显示行号区域
    // 链接信号槽
    // 行号宽度更新 - 根据文本块数量更新
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth()));
    // 文本框滚动绘制 - 始终保持行号框在窗口最右侧
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    // cursor位置改变 - 重写绘制高亮
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    // 初始化
    updateLineNumberAreaWidth();
    setMode(BROWSE);
    zoomIn(1);
}

int CodeEditor::lineNumberAreaWidth()
{// 文本宽度获取
    int digits = 1;
    int max = qMax(0, blockCount());// 获取最大行数
    while (max >= 10)
    {// 获取当前行号位
        max /= 10;
        ++digits;
    }
    // 当前行号区域宽度
    int space = LINE_NUMBER_AREA_BASE_WIDTH+fontMetrics().width(QLatin1Char('0')) * digits;
    return space;
}

void CodeEditor::updateLineNumberAreaWidth()
{// 更新行宽 - 设置可视距离
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{// 更新行号区域 - 滑动窗口
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
// 独立布局时的横向滚动 - 流布局无需设置
    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth();
}

// @override
void CodeEditor::resizeEvent(QResizeEvent *e)
{// 继承原QPlainTextEdit事件+行号区域所需事件
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{// 高亮当前行 - 通过ExtraSelection
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        // todo：找个好看的颜色... @`13
        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        // 设置高亮，可参数化
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{// 行号区域绘制
    QPainter painter(lineNumberArea);                // 初始化画笔
    painter.fillRect(event->rect(), Qt::lightGray); // 矩阵
    // todo：找个好看的颜色... @`13
    // 矩阵绘制参数
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    // 绘制矩阵 - 在主循环事件 重绘
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(LINE_NUMBER_AREA_NUM_POS, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

// @overrride
void CodeEditor::setMode(editorMode mode)
{// 设置编辑模式：编辑/浏览
    if(mode == BROWSE)
    {// 浏览
        this->setReadOnly(true);
        this->setStyleSheet("background:#CDC5BF;");
         highlightCurrentLine();
    }
    else if(mode == EDIT)
    {// 编辑
        this->setReadOnly(false);
        this->setStyleSheet("background:#FFF5EE;");
         highlightCurrentLine();
    }
}
