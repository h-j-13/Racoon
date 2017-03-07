//        Racoon 代码编辑器
//
//      -=继承自Qt官方示例框架=-
//
//      author  :   @`13
//      time    :   2017.03.02

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    // 初始化资源
    Q_INIT_RESOURCE(resources);
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Harbin Institute of Technology at WeiHai");
    QCoreApplication::setApplicationName("Racoon text editor");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    // 注册帮助控件
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);
    // 主窗口
    MainWindow mainWin;
    if (!parser.positionalArguments().isEmpty())
        mainWin.loadFile(parser.positionalArguments().first());
    mainWin.show();
    return app.exec();
}
