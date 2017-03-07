#ifndef CODEEDITOR_H
#define CODEEDITOR_H

// 代码编辑器控件
// @`13
// 2017年2月26日

#include <QPlainTextEdit>
#include <QObject>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QSize>
#include <QWidget>
#include <QSyntaxHighlighter>

#include "src/editorMode.h"

/* 行号显示 */
class LineNumberArea;

/* 代码编辑器控件 */
class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);
    void setMode(editorMode mode);                      // 设置编辑模式
    void lineNumberAreaPaintEvent(QPaintEvent *event);  // 行号显示重写事件
    int lineNumberAreaWidth();                          // 行号宽
    // build-in func
    // bool find(const QRegExp &exp, QTextDocument::FindFlags options);    // 正则匹配
    // bool find(const QString &exp, QTextDocument::FindFlags options);    // 正则匹配
    bool find_by_KMPP(const QRegExp &exp, QTextDocument::FindFlags options);    // KMPP匹配算法
    bool find_by_BMH(const QRegExp &exp, QTextDocument::FindFlags options);     // BMH匹配算法


protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE; // 引发事件

private slots: // 通信槽
    void highlightCurrentLine();                            // 高亮当前行
    void updateLineNumberAreaWidth();                       // 更新行号宽
    void updateLineNumberArea(const QRect &, int);          // 更新行号区

private:
    QWidget *lineNumberArea;                                // 行号Qwidget
};


#endif // CODEEDITOR_H
