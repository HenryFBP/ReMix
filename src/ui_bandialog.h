/********************************************************************************
** Form generated from reading UI file 'bandialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BANDIALOG_H
#define UI_BANDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BanDialog
{
public:
    QGridLayout *gridLayout;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QWidget *widget;

    void setupUi(QDialog *BanDialog)
    {
        if (BanDialog->objectName().isEmpty())
            BanDialog->setObjectName(QStringLiteral("BanDialog"));
        BanDialog->resize(479, 267);
        gridLayout = new QGridLayout(BanDialog);
        gridLayout->setSpacing(5);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(5, 5, 5, 5);
        frame = new QFrame(BanDialog);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(frame);
        widget->setObjectName(QStringLiteral("widget"));

        gridLayout_2->addWidget(widget, 0, 0, 1, 1);


        gridLayout->addWidget(frame, 0, 0, 1, 1);


        retranslateUi(BanDialog);

        QMetaObject::connectSlotsByName(BanDialog);
    } // setupUi

    void retranslateUi(QDialog *BanDialog)
    {
        BanDialog->setWindowTitle(QApplication::translate("BanDialog", "Ban Information:", 0));
    } // retranslateUi

};

namespace Ui {
    class BanDialog: public Ui_BanDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BANDIALOG_H
