
#ifndef SERVERINFO_HPP
#define SERVERINFO_HPP

#include "prototypes.hpp"

//Required Qt Includes.
#include <QObject>

class ServerInfo
{
    QUdpSocket* masterSocket{ nullptr };
    QString masterInfoHost{ "http://synthetic-reality.com/synreal.ini" };

    QTimer upTimer;
    quint64 upTime{ 0 };

    bool isSetUp{ false };
    QString name{ "AHitB ReMix Server" };

    QString privateIP{ "" };
    int privatePort{ 8888 };

    QString publicIP{ "" };
    quint16 publicPort{ 8888 };

    quint32 usageArray[ SERVER_USAGE_48_HOURS ]{ 0 };
    quint32 usageCounter{ 0 };

    quint32 usageHours{ 0 };
    quint32 usageDays{ 0 };
    quint32 usageMins{ 0 };

    QTimer usageUpdate;

    quint32 playerCount{ 0 };
    int serverID{ 0 };

    bool isMaster{ false };
    bool isPublic{ false };

    bool sentUDPCheckin{ false };
    bool masterUDPResponse{ false };

    QTimer masterCheckIn;
    QTimer masterTimeOut;
    bool masterTimedOut{ false };

    QString masterIP{ "" };
    quint16 masterPort{ 23999 };

    int versionID{ 41252 };

    QHostInfo hostInfo;

    QString gameName{ "WoS" };
    int gameId{ 0 };

    QString gameInfo{ "" };
    QString info{ "" };

    Player* players[ MAX_PLAYERS ];

    quint32 userCalls{ 0 };
    quint32 serNumDc{ 0 };
    quint32 dupDc{ 0 };
    quint32 pktDc{ 0 };
    quint32 ipDc{ 0 };

    QElapsedTimer baudTime;
    quint64 bytesIn{ 0 };
    quint64 baudIn{ 0 };

    quint64 bytesOut{ 0 };
    quint64 baudOut{ 0 };

    bool logFiles{ false };

    public:
        ServerInfo();
        ~ServerInfo();

        void sendUDPData(QHostAddress& addr, quint16 port, QString& data);

        void sendServerInfo(QHostAddress& addr, quint16 port);
        void sendUserList(QHostAddress& addr, quint16 port);
        void sendMasterInfo(bool disconnect = false);

        Player* createPlayer(int slot);
        Player* getPlayer(int slot);
        void deletePlayer(int slot);

        int getEmptySlot();
        int getSocketSlot(QTcpSocket* soc);
        int getQItemSlot(QStandardItem* index);
        int getIPAddrSlot(QString ip);

        void sendServerRules(Player* plr);
        void sendServerGreeting(Player* plr);
        void sendMasterMessage(QString packet, Player* plr = nullptr,
                               bool toAll = false);
        quint64 sendToAllConnected(QString packet);

        quint64 getUpTime() const;
        QTimer* getUpTimer();

        QString getInfo() const;
        void setInfo(const QString& value);

        QString getGameInfo() const;
        void setGameInfo(const QString& value);

        int getGameId() const;
        void setGameId(int value);

        QString getGameName() const;
        void setGameName(const QString& value);

        QHostInfo getHostInfo() const;
        void setHostInfo(const QHostInfo& value);

        int getVersionID() const;
        void setVersionID(int value);

        quint16 getMasterPort() const;
        void setMasterPort(int value);

        QString getMasterIP() const;
        void setMasterIP(const QString& value);

        bool getIsPublic() const;
        void setIsPublic(bool value);

        bool getIsMaster() const;
        void setIsMaster(bool value);

        int getServerID() const;
        void setServerID(int value);

        quint32 getPlayerCount() const;
        void setPlayerCount(quint32 value);

        quint16 getPublicPort() const;
        void setPublicPort(quint16 value);

        QString getPublicIP() const;
        void setPublicIP(const QString& value);

        int getPrivatePort() const;
        void setPrivatePort(int value);

        QString getPrivateIP() const;
        void setPrivateIP(const QString& value);

        QString getName() const;
        void setName(const QString& value);

        bool getIsSetUp() const;
        void setIsSetUp(bool value);

        quint32 getUserCalls() const;
        void setUserCalls(const quint32& value);

        quint32 getSerNumDc() const;
        void setSerNumDc(const quint32& value);

        quint32 getDupDc() const;
        void setDupDc(const quint32& value);

        quint32 getPktDc() const;
        void setPktDc(const quint32& value);

        quint32 getIpDc() const;
        void setIpDc(const quint32& value);

        quint64 getBytesIn() const;
        void setBytesIn(const quint64& value);

        quint64 getBaudIn() const;
        void setBaudIn(const quint64& bIn);

        quint64 getBytesOut() const;
        void setBytesOut(const quint64& value);

        quint64 getBaudOut() const;
        void setBaudOut(const quint64& bOut);

        bool getLogFiles() const;
        void setLogFiles(bool value);

        QUdpSocket* getMasterSocket() const;
        bool initMasterSocket(QHostAddress& addr, quint16 port);

        QString getMasterInfoHost() const;
        void setMasterInfoHost(const QString& value);

        bool getSentUDPCheckin() const;
        void setSentUDPCheckIn(bool value);

        bool getMasterUDPResponse() const;
        void setMasterUDPResponse(bool value);

        bool getMasterTimedOut();
        void setMasterTimedOut(bool value);

        void startMasterCheckIn();
        void stopMasterCheckIn();

        quint32 getUsageHours() const;
        quint32 getUsageDays() const;
        quint32 getUsageMins() const;
};

#endif // SERVERINFO_HPP
