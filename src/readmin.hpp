
#ifndef READMIN_HPP
#define READMIN_HPP

#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QCryptographicHash>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDialog>
#include <QDebug>
#include <QMenu>
#include <QFile>

class RandDev;

namespace AdminHelper
{
    enum Keys{ RANK = 0, HASH = 1, SALT = 2 };
    const QString adminKeys[ 3 ]{ "rank", "hash", "salt" };
    const QStringList ranks
    {
        QStringList() << "Game Master" << "Co-Admin" << "Admin" << "Owner"
    };

    void setAdminData(const QString& key, const QString& subKey, QVariant& value);
    QVariant getAdminData(const QString& key, const QString& subKey);

    void setReqAdminAuth(QVariant& value);
    bool getReqAdminAuth();

    bool getIsRemoteAdmin(QString& serNum);
    bool cmpRemoteAdminPwd(QString& serNum, QVariant& value);

    quint32 getRemoteAdminRank(QString& sernum);
    void setRemoteAdminRank(QString& sernum, quint32 rank);

    quint32 changeRemoteAdminRank(QString& sernum);
    bool deleteRemoteAdmin(QString& sernum);
}

namespace Ui {
    class Dialog;
}

class ReAdmin : public QDialog
{
    Q_OBJECT

    Ui::Dialog *ui;

    QSortFilterProxyModel* tableProxy{ nullptr };
    QStandardItemModel* tableModel{ nullptr };

    RandDev* randDev{ nullptr };

    QMenu* contextMenu{ nullptr };
    QModelIndex menuIndex;

    public:
        enum Ranks{ GMASTER = 0, COADMIN = 1, ADMIN = 2, OWNER = 3 };

        explicit ReAdmin(QWidget *parent = 0);
        ~ReAdmin();

    private:
        void loadServerAdmins();
        void initContextMenu();
        void setAdminRank(int rank, QModelIndex index);

    private slots:
        void on_makeAdmin_clicked();
        void on_adminTable_customContextMenuRequested(const QPoint& pos);
        void on_actionRevokeAdmin_triggered();
        void on_actionChangeRank_triggered();
};

#endif // READMIN_HPP