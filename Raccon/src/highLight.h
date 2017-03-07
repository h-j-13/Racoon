#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

// 代码编辑器控件
// @`13
// 2017年2月26日

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>

#define C_FILE 1
#define JAVA_FILE 2
#define PYTHON_FILE 3
#define DOC_FILE 10

class MyHighLighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    MyHighLighter(QTextDocument *parent = 0,int FileType = C_FILE);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule
    {// 高亮格式
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    int FileType;
    // 多行注释正则匹配
    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;                      // 关键词格式
    QTextCharFormat keywordPatterns;                    // 关键词
    QTextCharFormat keywordPatterns_C;                  // 关键词-C/c++
    QTextCharFormat keywordPatterns_Python;             // 关键词-python
    QTextCharFormat keywordPatterns_Java;               // 关键词-Java
    QTextCharFormat classFormat;                        // 类
    QTextCharFormat singleLineCommentFormat;            // 单行注释
    QTextCharFormat multiLineCommentFormat;             // 多行注释
    QTextCharFormat quotationFormat;                    // 字符串
    QTextCharFormat functionFormat;                     // 函数
    QTextCharFormat headerFileFormat;                   // 头文件
};
#endif // HIGHLIGHTER_H
