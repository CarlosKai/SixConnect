/********************************************************************************
** Form generated from reading UI file 'chessclient.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHESSCLIENT_H
#define UI_CHESSCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_chessClient
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QLineEdit *lineEdit;
    QTextEdit *textEdit;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *chessClient)
    {
        if (chessClient->objectName().isEmpty())
            chessClient->setObjectName(QStringLiteral("chessClient"));
        chessClient->resize(1200, 850);
        centralWidget = new QWidget(chessClient);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(900, 30, 101, 61));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(900, 90, 281, 31));
        lineEdit->setFont(font);
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(900, 210, 281, 341));
        textEdit->setLineWidth(3);
        textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        textEdit->setReadOnly(true);
        chessClient->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(chessClient);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1200, 23));
        chessClient->setMenuBar(menuBar);

        retranslateUi(chessClient);

        QMetaObject::connectSlotsByName(chessClient);
    } // setupUi

    void retranslateUi(QMainWindow *chessClient)
    {
        chessClient->setWindowTitle(QApplication::translate("chessClient", "chessClient", Q_NULLPTR));
        label->setText(QApplication::translate("chessClient", "\347\212\266\346\200\201\357\274\232", Q_NULLPTR));
        textEdit->setHtml(QApplication::translate("chessClient", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:20pt; font-weight:600;\">   </span><span style=\" font-size:20pt; font-weight:600; color:#0055ff;\">\346\270\205\351\243\216\346\275\207\346\275\207\344\270\215\345\257\220\345\244\251\357\274\214</span></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:20pt; font-weight:600;\">   </span><span style=\" font-size:20pt; font-weight:600; color:#00aa00;\">\346\230\237\347\251\272\347\202\271\347\202\271\344"
                        "\270\213\345\205\255\350\277\236\343\200\202</span></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:20pt; font-weight:600;\">   </span><span style=\" font-size:20pt; font-weight:600; color:#ff0000;\">\351\235\242\345\257\271\346\234\211\347\274\230\346\243\213\344\274\232\345\217\213\357\274\214</span></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:20pt; font-weight:600;\">   </span><span style=\" font-size:20pt; font-weight:600; color:#aa00ff;\">\345\261\200\344\270\255\345\205\210\345\226\234\347\254\221\345\274\200\351\242\234</span><span style=\" font-size:18pt; font-weight:600; color:#aa00ff;\">\343\200\202</span></p>\n"
"<p align=\"justify\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent"
                        ":0; text-indent:0px;\"><br /></p>\n"
"<p align=\"justify\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p align=\"justify\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt;\">    Hello\357\274\201</span></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt;\">   \346\254\242\350\277\216\346\235\245\345\210\260\345\205\255\345\255\220\346\243\213\347\232\204\347\275\221\347\273\234\345\257\271\346\210\230\345\271\263\345\217\260\357\274\214\345\217\257\344\273\245\351\200\232\350\277\207\350\277\231\351\207"
                        "\214\350\277\233\350\241\214\345\256\242\346\210\267\347\253\257\345\222\214\346\234\215\345\212\241\345\231\250\344\273\245\345\217\212\345\256\242\346\210\267\347\253\257\344\271\213\351\227\264\347\232\204\345\257\271\346\210\230\357\274\214\346\234\215\345\212\241\345\231\250\344\270\272\347\231\275\350\211\262\346\226\271\357\274\214\345\256\242\346\210\267\347\253\257\344\270\272\351\273\221\350\211\262\346\226\271\343\200\202</span></p></body></html>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class chessClient: public Ui_chessClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHESSCLIENT_H
