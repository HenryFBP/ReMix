
#include "includes.hpp"
#include "plrlistwidget.hpp"
#include "ui_plrlistwidget.h"

PlrListWidget::PlrListWidget(QWidget *parent, ServerInfo* svr, Admin* adm) :
    QWidget(parent),
    ui(new Ui::PlrListWidget)
{
    ui->setupUi(this);

    server = svr;
    admin = adm;

    //Create our Context Menus
    contextMenu = new QMenu( this );

    //Setup the PlayerInfo TableView.
    plrModel = new QStandardItemModel( 0, 8, 0 );
    plrModel->setHeaderData( 0, Qt::Horizontal, "Player IP:Port" );
    plrModel->setHeaderData( 1, Qt::Horizontal, "SerNum" );
    plrModel->setHeaderData( 2, Qt::Horizontal, "Age" );
    plrModel->setHeaderData( 3, Qt::Horizontal, "Alias" );
    plrModel->setHeaderData( 4, Qt::Horizontal, "Time" );
    plrModel->setHeaderData( 5, Qt::Horizontal, "IN" );
    plrModel->setHeaderData( 6, Qt::Horizontal, "OUT" );
    plrModel->setHeaderData( 7, Qt::Horizontal, "BIO" );

    //Proxy model to support sorting without actually
    //altering the underlying model
    plrProxy = new PlrSortProxyModel();
    plrProxy->setDynamicSortFilter( true );
    plrProxy->setSourceModel( plrModel );
    plrProxy->setSortCaseSensitivity( Qt::CaseInsensitive );
    ui->playerView->setModel( plrProxy );

    //Install Event Filter to enable Row-Deslection.
    ui->playerView->viewport()->installEventFilter(
                                    new TblEventFilter( ui->playerView,
                                                        plrProxy ) );
}

PlrListWidget::~PlrListWidget()
{
    contextMenu->deleteLater();

    plrModel->deleteLater();
    plrProxy->deleteLater();

    delete ui;
}

QStandardItemModel* PlrListWidget::getPlrModel()
{
    return plrModel;
}

void PlrListWidget::resizeColumns()
{
    ui->playerView->resizeColumnsToContents();
}

void PlrListWidget::initContextMenu()
{
    contextMenu->clear();

    ui->actionSendMessage->setText( "Send Message" );
    contextMenu->addAction( ui->actionSendMessage );
    contextMenu->addAction( ui->actionMakeAdmin );
    contextMenu->addAction( ui->actionMuteNetwork );
    contextMenu->addAction( ui->actionDisconnectUser );
    contextMenu->addAction( ui->actionBANISHUser );

    contextMenu->insertSeparator( ui->actionMuteNetwork );
}

void PlrListWidget::on_playerView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex menuIndex = plrProxy->mapToSource(
                                ui->playerView->indexAt( pos ) );

    this->initContextMenu();
    if ( menuIndex.row() >= 0 )
    {
        Player* plr = server->getPlayer(
                          server->getQItemSlot(
                              plrModel->item( menuIndex.row(), 0 ) ) );
        if ( plr != nullptr )
            menuTarget = plr;

        if ( menuTarget != nullptr )
        {
            if ( menuTarget->getIsAdmin() )
                ui->actionMakeAdmin->setText( "Revoke Admin" );
            else
                ui->actionMakeAdmin->setText( "Make Admin" );

            if ( menuTarget->getNetworkMuted() )
                ui->actionMuteNetwork->setText( "Un-Mute Network" );
            else
                ui->actionMuteNetwork->setText( "Mute Network" );
        }

        contextMenu->popup( ui->playerView->viewport()->mapToGlobal( pos ) );
    }
    else
    {
        contextMenu->removeAction( ui->actionSendMessage );
        contextMenu->removeAction( ui->actionMakeAdmin );
        contextMenu->removeAction( ui->actionMuteNetwork );
        contextMenu->removeAction( ui->actionDisconnectUser );
        contextMenu->removeAction( ui->actionBANISHUser );
    }
    contextMenu->popup( ui->playerView->viewport()->mapToGlobal( pos ) );
}

void PlrListWidget::on_actionSendMessage_triggered()
{
    SendMsg* adminMsg = new SendMsg( this, server, menuTarget );
             adminMsg->exec();
    adminMsg->deleteLater();
    menuTarget = nullptr;
}

void PlrListWidget::on_actionMakeAdmin_triggered()
{
    bool send{ false };
    if ( menuTarget == nullptr )
        return;

    QString msg{ "" };
    QString make{ "The Server Host is attempting to register you as an "
                  "Admin with the server. Please reply to this message with "
                  "(/register *YOURPASS). Note: The server Host and other "
                  "Admins will not have access to this information." };

    QString revoke{ "Your Remote Administrator privileges have been REVOKED "
                    "by either the Server Host. Please contact the Server Host "
                    "if you believe this was in error." };

    QString sernum{ menuTarget->getSernumHex_s() };
    if ( !Admin::getIsRemoteAdmin( sernum ) )
    {
        msg = make;
        if ( Admin::createRemoteAdmin( this, sernum ) )
        {
            menuTarget->setReqNewAuthPwd( true );
            send = true;
        }
    }
    else
    {
        msg = revoke;
        if ( Admin::deleteRemoteAdmin( this, sernum ) )
        {
            menuTarget->resetAdminAuth();
            admin->loadServerAdmins();
            send = true;
        }
    }
    if ( send )
        server->sendMasterMessage( msg, menuTarget, false );

    menuTarget = nullptr;
}

void PlrListWidget::on_actionMuteNetwork_triggered()
{
    bool mute{ true };
    if ( menuTarget != nullptr )
    {
        QString title{ "%1 User:" };
        QString prompt{ "Are you certain you want to %1 [ %2 ]'s Network?" };
        if ( menuTarget->getNetworkMuted() )
        {
            title = title.arg( "Un-Mute" );
            prompt = prompt.arg( "Un-Mute" );

            mute = false;
        }
        else
        {
            title = title.arg( "Mute" );
            prompt = prompt.arg( "Mute" );
        }
        prompt = prompt.arg( menuTarget->getSernum_s() );

        if ( Helper::confirmAction( this, title, prompt ) )
            menuTarget->setNetworkMuted( mute );
    }
    menuTarget = nullptr;
}

void PlrListWidget::on_actionDisconnectUser_triggered()
{
    if ( menuTarget == nullptr )
        return;

    QTcpSocket* sock = menuTarget->getSocket();
    if ( sock != nullptr )
    {
        QString title{ "Disconnect User:" };
        QString prompt{ "Are you certain you want to DISCONNECT ( " %
                        menuTarget->getSernum_s() % " )?" };

        QString inform{ "The Server Host has disconnected you from the Server. "
                        "Reason: %1" };

        if ( Helper::confirmAction( this, title, prompt ) )
        {
            inform = inform.arg( Helper::getDisconnectReason( this ) );
            server->sendMasterMessage( inform, menuTarget, false );

            if ( sock->waitForBytesWritten() )
            {
                menuTarget->setDisconnected( true );
                server->setIpDc( server->getIpDc() + 1 );
            }
        }
    }
    menuTarget = nullptr;
}

void PlrListWidget::on_actionBANISHUser_triggered()
{
    if ( menuTarget == nullptr )
        return;

    QString title{ "Ban SerNum:" };
    QString prompt{ "Are you certain you want to BANISH User [ "
                  % menuTarget->getSernum_s() % " ]?" };

    QString inform{ "The Server Host has BANISHED you. Reason: %1" };
    QString reason{ "Manual Banish; %1" };

    QTcpSocket* sock = menuTarget->getSocket();
    if ( sock != nullptr )
    {
        if ( Helper::confirmAction( this, title, prompt ) )
        {
            reason = reason.arg( Helper::getBanishReason( this ) );
            inform = inform.arg( reason );

            server->sendMasterMessage( inform, menuTarget, false );
            admin->getBanDialog()->addBan( menuTarget, reason );

            if ( sock->waitForBytesWritten() )
            {
                menuTarget->setDisconnected( true );
                server->setIpDc( server->getIpDc() + 1 );
            }
        }
    }
    menuTarget = nullptr;
}
