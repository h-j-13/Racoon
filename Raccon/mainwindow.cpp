//        Racoon 代码编辑器
//
//      -=继承自Qt官方示例框架=-
//
//      author  :   @`13
//      time    :   2017.03.02

#include <QtWidgets>
#include <QtDebug>
#include <QProcess>

#include "mainwindow.h"

int CUR_FILE_TYPE = /*DO*/C_FILE;

MainWindow::MainWindow(): textEdit(new CodeEditor)
{// 初始化
    // QProcess::execute("ls");
    // Debug-Code
    // qDebug()<<"hello, world!";

    // 初始化代码编辑器控件
    textEdit->setMode(EDIT);
    setCentralWidget(textEdit);
    new MyHighLighter(textEdit->document(),CUR_FILE_TYPE);

    // 其他UI
    createActions();
    createStatusBar();
    readSettings();

    // 槽: 文件变更保存
    connect(textEdit->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);
    // 槽: 会话管理
#ifndef QT_NO_SESSIONMANAGER
    QGuiApplication::setFallbackSessionManagementEnabled(false);
    connect(qApp, &QGuiApplication::commitDataRequest,
            this, &MainWindow::commitData);
#endif
    setWindowIcon(QIcon("Racoon.ico"));
    setCurrentFile(QString());              // 当前文件
    setUnifiedTitleAndToolBarOnMac(true);   // 初始化
}

void MainWindow::closeEvent(QCloseEvent *event)
{// 重写关闭事件 处理未保存情况
    if (maybeSave())
    {
        writeSettings();
        event->accept();
    }
    else
        event->ignore();
}

void MainWindow::newFile()
{// 新文件
    if (maybeSave()) {
        textEdit->clear();          // 清空当前文本
        setCurrentFile(QString());  // 更新为新文件内容
    }
}

void MainWindow::open()
{// 打开新文件
    if (maybeSave())
    {// 适配文件高亮类型
        QString fileName = QFileDialog::getOpenFileName(this);
        QString cFormat_1 = ".cpp";
        QString cFormat_2 = ".h";
        QString cFormat_3 = ".c";
        QString pythonFormat = ".py";
        QString javaFormat = ".java";
        if(fileName.indexOf(cFormat_1)!=-1)
        {
            new MyHighLighter(textEdit->document(),C_FILE);
            CUR_FILE_TYPE = C_FILE;
        }

        else if(fileName.indexOf(cFormat_2)!=-1)
        {
            new MyHighLighter(textEdit->document(),C_FILE);
            CUR_FILE_TYPE = C_FILE;
        }
        else if(fileName.indexOf(cFormat_3)!=-1)
        {
            new MyHighLighter(textEdit->document(),C_FILE);
            CUR_FILE_TYPE = C_FILE;
        }
        else if(fileName.indexOf(pythonFormat)!=-1)
        {
            new MyHighLighter(textEdit->document(),PYTHON_FILE);
            CUR_FILE_TYPE = PYTHON_FILE;
        }
        else if(fileName.indexOf(javaFormat)!=-1)
        {
            new MyHighLighter(textEdit->document(),JAVA_FILE);
            CUR_FILE_TYPE = JAVA_FILE;
        }
        else
        {
            new MyHighLighter(textEdit->document(),DOC_FILE);
            CUR_FILE_TYPE = DOC_FILE;
        }
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::save()
{// 保存
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{// 另存为
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}

void MainWindow::run()
{// 运行
    switch (CUR_FILE_TYPE) {
    case C_FILE:// gcc
        QProcess::execute("rm tmp");    // 简单处理之前编译结果
        QProcess::execute("gcc -o tmp "+curFile);
        QProcess::execute("./tmp");
        break;
    case PYTHON_FILE:// python
        QProcess::execute("python "+curFile);
        break;
    case JAVA_FILE:// javac,java
        QProcess::execute("javac "+curFile);
        QString javaFileSuffix = ".java";
        int sufPos = curFile.lastIndexOf(javaFileSuffix)-5;
        int start = curFile.lastIndexOf("/");
        QString curJavaPath = curFile.left(start+1);
        QProcess::execute("java -classpath "
                          + curJavaPath
                          + " " + curFile.mid(start+1,(sufPos-start)+4));
        break;
    }
}

void MainWindow::comm()
{// 注释
    // 初始化注释格式
    QString commentFormat;
    if (CUR_FILE_TYPE == C_FILE || CUR_FILE_TYPE == JAVA_FILE)
        commentFormat = "//";
    else if(CUR_FILE_TYPE == PYTHON_FILE)
        commentFormat = "#";
    else // 其他格式不进行处理
        return;

    // 根据当前是否选中文本有两种处理情况
    QString curSelectedText = textEdit->textCursor().selectedText();
    if (curSelectedText == "")
    {// 若当前未选中文本，直接在光标前添加注释符号
        textEdit->textCursor().insertText(commentFormat);
    }
    else
    {
        if (curSelectedText.indexOf(commentFormat) == -1)
        {// 选中段落没有被注释
            if (CUR_FILE_TYPE == C_FILE || CUR_FILE_TYPE == JAVA_FILE)
                curSelectedText.replace(QString("\u2029"), QString("\u2029// "));
            else if(CUR_FILE_TYPE == PYTHON_FILE)
                curSelectedText.replace(QString("\u2029"), QString("\u2029# "));
            textEdit->textCursor().insertText(commentFormat + " " + curSelectedText);
        }
        else
        {// 选中段落已经被注释过
            if (CUR_FILE_TYPE == C_FILE || CUR_FILE_TYPE == JAVA_FILE)
            {
                curSelectedText.replace(QString("\u2029// "), QString("\u2029"));
                curSelectedText.replace(QString("//"), QString(""));
            }
                else if(CUR_FILE_TYPE == PYTHON_FILE)
            {
                curSelectedText.replace(QString("\u2029# "), QString("\u2029"));
                curSelectedText.replace(QString("#"), QString(""));
            }
                textEdit->textCursor().insertText(curSelectedText.mid(1,-1));
        }
    }
}

void MainWindow::find_KMP(){}

void MainWindow::find_BMH(){}

void MainWindow::search()
{// 查找 : todo 内建函数调用有问题，找不到Demo和源码，无法重写。@`13
    // 查找当前选中文本
    QString curSelectedText = textEdit->textCursor().selectedText();
    if (curSelectedText.isEmpty())
        return;
    if(textEdit->find(curSelectedText,QTextDocument::FindBackward)) //查找后一个
    {// 查找到后选中并高亮
            QPalette palette = textEdit->palette();
            palette.setColor(QPalette::Highlight,palette.color(QPalette::Active,QPalette::Highlight));
            textEdit->setPalette(palette);
    }
    else
        QMessageBox::information(this,tr("注意"),tr("查找不到所选内容"),QMessageBox::Ok);
}

void MainWindow::format()
{// 简单一键格式化 (不处理特意修改过的文本)
    if(!(CUR_FILE_TYPE == C_FILE || CUR_FILE_TYPE == JAVA_FILE))
        // 只处理C/Java文本
        return;
    // 获取当前文本并拆分成列表
    QString curText = textEdit->toPlainText();
    QStringList lineList = curText.split("\n");
    QStringList formatLineList;
    QString formatText;
    int formatLevel = 0;        // 期望缩进级别
    int lineFormatLevel = 0;    // 当前缩进级别
    for(QString line : lineList)
    {// 处理每行的缩进情况
        for(QChar c : line)
        {// 获取每行的缩进级别
            if(c == "{")
                formatLevel++;
            else if(c == "}")
                if(formatLevel > 0)
                    formatLevel--;
        }
        lineFormatLevel = 0;
        if (formatLevel > 0 && line[0] != " ")
        {// 进行缩进 / 不处理前方有空格的情况
            // 含有 ”{“ 的行减少一级缩进
            if (line.contains("{"))
                lineFormatLevel ++;
            while(lineFormatLevel<formatLevel)
            {// 缩进至相应级别
                line = "    " + line;    // 使用4个空格缩进
                lineFormatLevel++;
            }
        }
        line = line.append("\n");
        formatLineList.append(line);
    }// 获取缩进文本
    for(QString line : formatLineList)
        formatText += line;
    int needLength = formatText.size(); // 去掉最后一个换行符
    formatText = formatText.left(needLength-1);
    // 设置缩进文本并重置光标位置
    textEdit->setPlainText(formatText);
}

void MainWindow::setCursorPos(int row,int col)
{// 设置光标位置
    const QTextBlock block = textEdit->document()->findBlockByLineNumber(row-1);
    if(block.isValid())
    {
        QTextCursor cursor = textEdit->textCursor();
        cursor.setPosition(block.position()+col-1);
        textEdit->setTextCursor(cursor);
        textEdit->ensureCursorVisible();
    }
}

void MainWindow::bracketMatch()
{// 括号匹配
    QString expectMatchStr;
    QString curSelectedText = textEdit->textCursor().selectedText();
    // 括获取匹配字符
    if (curSelectedText == "(")
        expectMatchStr = ")";
    else if (curSelectedText == "[")
        expectMatchStr = "]";
    else if (curSelectedText == "{")
        expectMatchStr = "}";
    else
        return;
    QChar curMatchChar = curSelectedText[0];
    QChar expectMatchChar = expectMatchStr[0];

    // 括获光标位置
    int lineNumber = textEdit->textCursor().blockNumber();
    int colNumber = textEdit->textCursor().columnNumber();

    // 从光标位置开始向下查找匹配字符
    int matchStack = 1;                                         // 匹配栈
    int totalLineNumber = textEdit->document()->lineCount();    // 总行数
    QString lineText = textEdit->document()->findBlockByLineNumber(lineNumber).text().mid(colNumber,-1);

    if (lineText[0] == curMatchChar)
    {// 检测选中时光标的位置并修改([由右向左选中] 和 [由左向右选中] 统一改为 [由左向右选中] )
        lineText = lineText.mid(1,-1);
        colNumber ++;
    }
    while(lineNumber<totalLineNumber)
    {// 遍历光标下每行
        for(QChar s:lineText)
        {// 遍历每行每个字符，找到匹配位置
            if (s == curMatchChar)
                matchStack ++;
            else if(s == expectMatchChar)
                matchStack --;

            if(matchStack == 0) // 表示匹配成功
                goto success;

            colNumber ++;
        }
        colNumber = 0;
        lineNumber ++;
        lineText= textEdit->document()->findBlockByLineNumber(lineNumber).text();
    }
    return; // 匹配不成功无响应
    success:// 匹配成功则定位光标
        setCursorPos(lineNumber+1,colNumber+1);

}

void MainWindow::about()
{// 关于
   QMessageBox::about(this, tr("About Raccon"),
            tr("The <b>Raccon</b> is a CodeEditor    "
               "written by <b>`13</b>    "
               "if you want to learn more.  "
               "contact me at <b>Github</b>:  "
               "https://github.com/h-j-13  "));
}

void MainWindow::documentWasModified()
{// 判断文件是否改变
    setWindowModified(textEdit->document()->isModified());
}

void MainWindow::createActions()
{// UI
    // File-opreator
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    // 1,NEW
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("创建一个新文件"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    // 2,Open
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("打开一个文件"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    // 3,Save
    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("保存文件"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    // 4,SaveAs
    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
    QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("另存为"));

    // 5,run
    const QIcon runAsIcon = QIcon::fromTheme("document-run", QIcon(":/images/run.png"));
    QAction *runAct = fileMenu->addAction(runAsIcon, tr("&Run"), this, &MainWindow::run);
    runAct->setShortcuts(QKeySequence::Refresh);
    runAct->setStatusTip(tr("运行"));
    fileMenu->addAction(runAct);
    fileToolBar->addAction(runAct);

    // 6,Exit
    fileMenu->addSeparator();
    const QIcon exitIcon = QIcon::fromTheme("Racoon-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the Racoon"));

    // Text-opreator
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("Edit"));

    // 1,cut
#ifndef QT_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
    QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("剪切文本"));
    connect(cutAct, &QAction::triggered, textEdit, &QPlainTextEdit::cut);
    editMenu->addAction(cutAct);
    editToolBar->addAction(cutAct);

    // 2,copy
    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
    QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("复制文本"));
    connect(copyAct, &QAction::triggered, textEdit, &QPlainTextEdit::copy);
    editMenu->addAction(copyAct);
    editToolBar->addAction(copyAct);

    // 3,paste
    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
    QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("粘贴文本"));
    connect(pasteAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
    editMenu->addAction(pasteAct);
    editToolBar->addAction(pasteAct);

    // 4,undo
    const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(":/images/revoke.png"));
    QAction *undoAct = new QAction(undoIcon, tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("撤销操作"));
    connect(undoAct, &QAction::triggered, textEdit, &QPlainTextEdit::undo);
    editMenu->addAction(undoAct);
    editToolBar->addAction(undoAct);

    // 4,redo
    const QIcon redoIcon = QIcon::fromTheme("edit-redo", QIcon(":/images/recovery.png"));
    QAction *redoAct = new QAction(redoIcon, tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("恢复操作"));
    connect(redoAct, &QAction::triggered, textEdit, &QPlainTextEdit::redo);
    editMenu->addAction(redoAct);
    editToolBar->addAction(redoAct);

    // 5,comment
    const QIcon commIcon = QIcon::fromTheme("edit-comm", QIcon(":/images/comment.png"));
    QAction *commAct = editMenu->addAction(commIcon, tr("Com&ment"), this, &MainWindow::comm);
    commAct->setStatusTip(tr("注释代码"));
    commAct->setShortcuts(QKeySequence::Underline);
    editMenu->addAction(commAct);
    editToolBar->addAction(commAct);

    // 6,find
    const QIcon findIcon = QIcon::fromTheme("edit-find", QIcon(":/images/search.png"));
    QAction *findAct = editMenu->addAction(findIcon, tr("&Find"), this, &MainWindow::search);
    findAct->setStatusTip(tr("查找"));
    findAct->setShortcuts(QKeySequence::Find);
    editToolBar->addAction(findAct);

    // 7,match
    const QIcon matchIcon = QIcon::fromTheme("edit-match", QIcon(":/images/match.png"));
    QAction *matchAct = editMenu->addAction(matchIcon, tr("Mat&ch"), this, &MainWindow::bracketMatch);
    matchAct->setStatusTip(tr("括号匹配"));
    matchAct->setShortcuts(QKeySequence::Italic);
    editToolBar->addAction(matchAct);


    // 8,format
    const QIcon formatIcon = QIcon::fromTheme("edit-find", QIcon(":/images/format.png"));
    QAction *formatdAct = editMenu->addAction(formatIcon, tr("F&ormat"), this, &MainWindow::format);
    formatdAct->setStatusTip(tr("自动格式化"));
    editToolBar->addAction(formatdAct);
    menuBar()->addSeparator();

#endif // !QT_NO_CLIPBOARD
    // Help-Menu
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    // 1,About
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the Racoon's About box"));
    // 2,About Qt
    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

#ifndef QT_NO_CLIPBOARD
    // 剪切粘贴设置-剪贴板
    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, &QPlainTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
    connect(textEdit, &QPlainTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD
}

void MainWindow::createStatusBar()
{// 初始化状态栏
    statusBar()->showMessage(tr("系统初始化完成..."));
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::writeSettings()
{// // 写入配置
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

bool MainWindow::maybeSave()
{//
    if (!textEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Racoon"),
                               tr("当前编辑的文件已经被修改\n"
                                  "是否保存变更 ?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{// 载入文件
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Racoon"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }
    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{// 保存文件
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Racoon"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{// 设置当前文件
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

QString MainWindow::strippedName(const QString &fullFileName)
{// 全路径
    return QFileInfo(fullFileName).fileName();
}

#ifndef QT_NO_SESSIONMANAGER
void MainWindow::commitData(QSessionManager &manager)
{// 会话管理
    if (manager.allowsInteraction()) {
        if (!maybeSave())
            manager.cancel();
    } else {
        // Non-interactive: save without asking
        if (textEdit->document()->isModified())
            save();
    }
}
#endif
