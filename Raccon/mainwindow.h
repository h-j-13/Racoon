/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "src/codeEditor.h"
#include "src/highLight.h"
#include "src/editorMode.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void loadFile(const QString &fileName);     // 载入文件

protected:
    void closeEvent(QCloseEvent *event) override;   // 关闭事件
    /* *重写 用于处理未保存的文件 */

private slots:
    void newFile(); // 新文件
    void open();    // 打开
    bool save();    // 保存
    bool saveAs();  // 另存为
    void about();   // 关闭
    void documentWasModified(); // 文本是否修改
#ifndef QT_NO_SESSIONMANAGER
    // 会话管理器
    void commitData(QSessionManager &);
#endif

private:
    void createActions();   // 按钮
    void createStatusBar(); // 状态栏
    void readSettings();    // 读取配置
    void writeSettings();   // 写入配置
    bool maybeSave();       // 是否保存
    bool saveFile(const QString &fileName);         // 保存文件
    void setCurrentFile(const QString &fileName);   // 当前文件
    void run();                                     // 运行
    void comm();                                    // 注释代码
    void find_KMP();                                // KMP算法查找
    void find_BMH();                                // BMH算法查找
    void search();                                  // 查找
    void format();                                  // 一键格式化
    void bracketMatch();                            // 括号匹配
    void setCursorPos(int row,int col);             // 设置光标位置

    QString strippedName(const QString &fullFileName);  // 文件url
    CodeEditor *textEdit;                               // 代码编辑器控件
    QString curFile;                                    // 文件名
};
#endif  // MAINWINDOW_H
