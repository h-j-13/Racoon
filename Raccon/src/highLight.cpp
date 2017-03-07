#include "highLight.h"

MyHighLighter::MyHighLighter(QTextDocument *parent, int FT): QSyntaxHighlighter(parent)
{// 继承QSyntaxHighlighter,对代码控件中的文字实现高亮
    HighlightingRule rule;  //  初始化高亮规则
    // 关键词高亮
    FileType = FT;
    if (FileType == C_FILE)
    {// 关键词高亮-c
        keywordFormat.setForeground(Qt::darkBlue);// 深蓝
        keywordFormat.setFontWeight(QFont::Bold);
        QStringList keywordPatterns_C;
        keywordPatterns_C << "\\bchar\\b"
                          << "\\bauto\\b"
                          << "\\bconst\\b"
                          << "\\bdouble\\b"
                          << "\\belse\\b"
                          << "\\benum\\b"
                          << "\\bexplicit\\b"
                          << "\\bfriend\\b"
                          << "\\binline\\b"
                          << "\\bint\\b"
                          << "\\bif\\b"
                          << "\\blong\\b"
                          << "\\bnamespace\\b"
                          << "\\boperator\\b"
                          << "\\bprivate\\b"
                          << "\\bprotected\\b"
                          << "\\bpublic\\b"
                          << "\\bshort\\b"
                          << "\\bsignals\\b"
                          << "\\bsigned\\b"
                          << "\\bslots\\b"
                          << "\\bstatic\\b"
                          << "\\bstruct\\b"
                          << "\\btemplate\\b"
                          << "\\btypedef\\b"
                          << "\\btypename\\b"
                          << "\\bunion\\b"
                          << "\\bunsigned\\b"
                          << "\\bvirtual\\b"
                          << "\\bvoid\\b"
                          << "\\bvolatile\\b"
                          << "\\breturn\\b"
                          << "\\bstd\\b"
                          << "\\busing\\b"
                          << "\\bbreak\\b"
                          << "\\bcontinue\\b";

        foreach (const QString &pattern, keywordPatterns_C) {
            rule.pattern = QRegExp(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }
    }

    else if (FileType == JAVA_FILE)
    {// 关键词高亮-java
        keywordFormat.setForeground(Qt::darkBlue);// 深蓝
        keywordFormat.setFontWeight(QFont::Bold);
        QStringList keywordPatterns_Java;
        keywordPatterns_Java << "\\babstract\\b"
                             << "\\bassert\\b"
                             << "\\bboolean\\b"
                             << "\\bbreak\\b"
                             << "\\bbyte\\b"
                             << "\\bcase\\b"
                             << "\\bcatch\\b"
                             << "\\bchar\\b"
                             << "\\bconst\\b"
                             << "\\bcontinue\\b"
                             << "\\bdefault\\b"
                             << "\\bdo\\b"
                             << "\\bdouble\\b"
                             << "\\belse\\b"
                             << "\\benum\\b"
                             << "\\bextends\\b"
                             << "\\bfinal\\b"
                             << "\\bfinally\\b"
                             << "\\bfloat\\b"
                             << "\\bfor\\b"
                             << "\\bgoto\\b"
                             << "\\bif\\b"
                             << "\\bimplements\\b"
                             << "\\binstanceof\\b"
                             << "\\bint\\b"
                             << "\\binterface\\b"
                             << "\\blong\\b"
                             << "\\bnew\\b"
                             << "\\bpackage\\b"
                             << "\\bprivate\\b"
                             << "\\bprotected\\b"
                             << "\\bpublic\\b"
                             << "\\bbreak\\b"
                             << "\\breturn\\b"
                             << "\\bshort\\b"
                             << "\\bstatic\\b"
                             << "\\bstrictfp\\b"
                             << "\\bsuper\\b"
                             << "\\bswitch\\b"
                             << "\\bsynchronized\\b"
                             << "\\bthis\\b"
                             << "\\bthrow\\b"
                             << "\\bthrows\\b"
                             << "\\btransient\\b"
                             << "\\btry\\b"
                             << "\\bvoid\\b"
                             << "\\bvolatile\\b"
                             << "\\bwhile\\b";

        foreach (const QString &pattern, keywordPatterns_Java) {
            rule.pattern = QRegExp(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }
    }
    if (FileType == PYTHON_FILE)
    {// 关键词高亮-python
        keywordFormat.setForeground(Qt::darkBlue);// 深蓝
        keywordFormat.setFontWeight(QFont::Bold);
        QStringList keywordPatterns_Python;
        keywordPatterns_Python << "\\band\\b"
                               << "\\bas\\b"
                               << "\\bassert\\b"
                               << "\\bbreak\\b"
                               << "\\bboolean\\b"
                               << "\\bcontinue\\b"
                               << "\\bdef\\b"
                               << "\\bdel\\b"
                               << "\\belif\\b"
                               << "\\belse\\b"
                               << "\\bexcept\\b"
                               << "\\bexec\\b"
                               << "\\bfinally\\b"
                               << "\\bfor\\b"
                               << "\\bfrom\\b"
                               << "\\bglobal\\b"
                               << "\\bif\\b"
                               << "\\bimport\\b"
                               << "\\bin\\b"
                               << "\\bis\\b"
                               << "\\blambda\\b"
                               << "\\bnot\\b"
                               << "\\bor\\b"
                               << "\\bpass\\b"
                               << "\\bprint\\b"
                               << "\\braise\\b"
                               << "\\breturn\\b"
                               << "\\btry\\b"
                               << "\\bwhile\\b"
                               << "\\bwith\\b"
                               << "\\byield\\b";

        foreach (const QString &pattern, keywordPatterns_Python) {
            rule.pattern = QRegExp(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }
    }

    // 类 c/++/java/python
    classFormat.setFontWeight(QFont::Bold);     // 粗体
    classFormat.setForeground(Qt::darkMagenta); // 深蓝
    rule.pattern = QRegExp("\\bclass\\b?:?\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    // 单行注释
    if (FileType==PYTHON_FILE)
    {
        singleLineCommentFormat.setForeground(Qt::lightGray); // 灰色
        rule.pattern = QRegExp("#[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);
    }
    else
    {
        singleLineCommentFormat.setForeground(Qt::lightGray); // 灰色
        rule.pattern = QRegExp("//[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);
    }

    // 字符串 (引号)
    quotationFormat.setForeground(Qt::darkGreen); // 绿色
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegExp("\'.*\'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // 头文件/扩展库 include/import
    headerFileFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("#include.?[<\"].*[>\"]");
    rule.format = headerFileFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegExp("[from]?.*import.?.*");
    rule.format = headerFileFormat;
    highlightingRules.append(rule);

    // 函数
    functionFormat.setFontItalic(true);     // 斜体
    functionFormat.setForeground(Qt::blue); // 蓝色
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // 多行注释
    if (FileType==C_FILE || FileType==JAVA_FILE)
    {
        commentStartExpression = QRegExp("/\\*");
        commentEndExpression = QRegExp("\\*/");
        multiLineCommentFormat.setForeground(Qt::lightGray);
    }
}

void MyHighLighter::highlightBlock(const QString &text)
{// 正则匹配 高亮关键词
    foreach (const HighlightingRule &rule, highlightingRules)
    {// 调用QSyntaxHighlighter 设置格式
        QRegExp expression(rule.pattern);
        // 获取关键词始末位置
        int index = expression.indexIn(text);
        while (index >= 0)
        {// 设置格式
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    // 获取注释始末位置
    if (FileType==C_FILE || FileType==JAVA_FILE)
    {
        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = commentStartExpression.indexIn(text);
        while (startIndex >= 0)
        {
            int endIndex = commentEndExpression.indexIn(text, startIndex);
            int commentLength;
            if (endIndex == -1)
            {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex
                        + commentEndExpression.matchedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
        }
    }
}
