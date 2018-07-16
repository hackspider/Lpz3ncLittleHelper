/****************************************************************************
 * Copyright (C) 2018 by Sebastian Ziganki                                  *
 *                                                                          *
 * This file is part of Lpz3ncLittleHelper.                                 *
 *                                                                          *
 *   Lpz3ncLittleHelper is free software: you can redistribute it and/or    *
 *   modify it under the terms of the GNU Lesser General Public License as  *
 *   published by the Free Software Foundation, either version 3 of the     *
 *   License.                                                               *
 *                                                                          *
 *   Lpz3ncLittleHelper is distributed in the hope that it will be useful,  *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Box. If not, see  http://www.gnu.org/licenses .     *
 ****************************************************************************/

/**
 * /file    twitchconnector.h
 * /author  Hackspider
 * /brief   Header file of the twitch connector
 */

/* Include Guard */
#ifndef TWITCHCONNECTOR_H
#define TWITCHCONNECTOR_H

/* Necessary dependencies to Qt framework */
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QStandardItemModel>
#include <QSet>
#include <QLabel>
#include <QPushButton>

/* Type to distinguish the twitch connection state */
typedef enum
{
    DISCONNECTED = 0,
    CONNECTING,
    CONNECTED,
} ConnectionState;


/**
 * /brief  The TwitchConnector class declaraation. Derived from QObject class.
 */
class TwitchConnector : public QObject
{
    Q_OBJECT
public:
    /**
     * /brief  Constructor of the TwitchConnector class
     * /param  connectURL      Url to twitch service to connect to
     * /param  port            Port of the twitch service to connect to
     * /param  loginName       Username to connect to twitch
     * /param  loginPass       OAuth2 token for authentication
     * /param  channelModel    Pointer to the channel model
     * /param  connectionIcon  Pointer to the icon label
     * /param  connectButton   Pointer to the connect button
     * /param  parent          Parent of the TwitchConnector object according to Qt's object tree
     */
    explicit TwitchConnector( QString connectURL,
                              quint16 port,
                              QString loginName,
                              QString loginPass,
                              QStandardItemModel* channelModel,
                              QLabel* connectionIcon,
                              QPushButton* connectButton,
                              QObject *parent = nullptr);

    /**
     * /brief  Destructor of the TwitchConnector class
     */
    virtual ~TwitchConnector(void);

public slots:
    /**
     * /brief   Method to ban a user in all connected channels
     * /param   userName  Name of the user that shall be banned on connected channels
     * /return  Returns a set of channels which the user has been banned
     */
    QSet<QString> banUser(QString userName);

    /**
     * /brief   Method to unban a user in all connected channels
     * /param   userName  Name of the user that shall be unbanned on connected channels
     * /return  Returns a set of channels which the user has unbeen banned
     */
    QSet<QString> unbanUser(QString userName);

    /**
     * /brief  Method to connect to the twitch service
     */
    void Connect(void);

    /**
     * /brief  Method to disconnect from twitch service
     */
    void Disconnect(void);

    /**
     * /brief   Method to get the internal connection state to twitch service
     * /return  Return the internal connection state to twitch service
     */
    ConnectionState GetConnectionState(void);

    /**
     * /brief  Method to change the login name
     * /param  loginName  New login name
     */
    void SetLoginName(QString loginName);

    /**
     * /brief  Method to change the oauth2
     * /param  oauth2  New oauth2
     */
    void SetOauth2(QString oauth2);

private slots:
    /**
     * /brief  Method will be called if the tcp socket established a connection to twitch service
     */
    void connected(void);

    /**
     * /brief  Method will be called if the tcp socket is disconnected ffrom twitch service
     */
    void disconnected(void);

    /**
     * /brief  Method will be called if the tcp socket raises an error
     * /param  error  Contains information which kind of an error occured
     */
    void error(QAbstractSocket::SocketError error);

    /**
     * /brief  Method will be called if the tcp socket receives data
     */
    void readyRead(void);

    /**
     * /brief  Mthod will be called cyclic if the twitch service is connected to prevent disconnect by PING?PONG
     */
    void pingTimer(void);

    /**
     * /brief  Method will be called if an channel item chanes (e.g. by chaning the check box)
     * /param  item  Pointer to the item that changes
     */
    void channelModelItemChanged(QStandardItem* item);

    /**
     * /brief  Method will be called if a row of the channel model changes (e.g. add/remove a channel)
     * /param  parent  Reference to the model index
     * /param  start   Start row number of the changed rows
     * /param  end     End row number of the changed rows
     */
    void channelRowsChanged(const QModelIndex & parent, int start, int end);

    /**
     * /brief  Internally used method to iterate over all channels and update the connection to the changed channels
     */
    void updateChannels(void);

    /**
     * /brief  Internally used method to set internal connection state and lock/unlock ui elements
     */
    void connectingUI(void);

    /**
     * /brief  Internally used method to set internal connection state and lock/unlock ui elements
     */
    void connectUI(void);

    /**
     * /brief  Internally used method to set internal connection state and lock/unlock ui elements
     */
    void disconnectUI(void);


private:
    /**
     * /brief Internal used variables
     */
    QTcpSocket* mSocket;
    QString mConnectURL;
    quint16 mPort;
    QTimer* mCyclicTimer;
    ConnectionState mState;
    QString mLoginName;
    QString mLoginPass;
    QStandardItemModel* mChannelModel;
    QSet<QString> mConnectedChannels;
    QLabel* mConnectionIcon;
    QPushButton* mConnectionButton;
};

#endif // TWITCHCONNECTOR_H
