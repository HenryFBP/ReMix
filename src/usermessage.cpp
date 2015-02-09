
#include "includes.hpp"
#include "usermessage.hpp"
#include "ui_usermessage.h"

UserMessage::UserMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserMessage)
{
    ui->setupUi(this);

    //Remove the "Help" button from the window title bars.
    {
        QIcon icon = this->windowIcon();
        Qt::WindowFlags flags = this->windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;

        this->setWindowFlags( flags );
        this->setWindowIcon( icon );
    }
}

UserMessage::~UserMessage()
{
    delete ui;
}

void UserMessage::newUserCommentSlot(QString& sernum, QString& alias,
                                     QString& message)
{
    QTextEdit* obj = ui->msgView;
    if ( obj == nullptr )
        return;

    quint64 date = QDateTime::currentDateTime()
                        .toTime_t();
    QString comment = QString( "\r\n --- \r\n"
                               "%1 \r\n"
                               "SerNum: %2 \r\n"
                               "%3: %4"
                               "\r\n --- \r\n" )
                          .arg( QDateTime::fromTime_t( date )
                                     .toString( "ddd MMM dd HH:mm:ss yyyy" ) )
                          .arg( sernum )
                          .arg( alias )
                          .arg( message );

    int curScrlPosMax = obj->verticalScrollBar()->maximum();
    int selStart{ 0 };
    int selEnd{ 0 };

    QTextCursor cursor( obj->textCursor() );
    if ( cursor.hasSelection() )
    {
        selStart = cursor.selectionStart();
        selEnd = cursor.selectionEnd();
    }
    cursor.movePosition( QTextCursor::End );
    cursor.insertText( comment );

    if ( selStart && selEnd )
    {
        cursor.setPosition( selStart );
        cursor.setPosition( selEnd, QTextCursor::KeepAnchor );
        obj->setTextCursor( cursor );
    }

    //Detect when the user is scrolling upwards.
    if ( obj->verticalScrollBar()->sliderPosition() == curScrlPosMax )
        obj->verticalScrollBar()->setSliderPosition(
                    obj->verticalScrollBar()->maximum() );

    if ( Helper::getLogComments() )
    {
        QString log = QDate::currentDate()
                       .toString( "mixComments/" % sernum
                                % "_yyyy-MM-dd.txt" );
        Helper::logToFile( log, comment, false, false );
    }

    //Show the Dialog when a new comment is recieved.
    if ( !this->isVisible() )
        this->show();
}
