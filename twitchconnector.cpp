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
 * /file    twitchconnector.cpp
 * /author  Hackspider
 * /brief   Source file of the twitch connector
 */

/* Necessary dependencies to Qt framework */
#include <QTextStream>
#include <QRegularExpression>

/* Necessary internal dependencies */
#include "twitchconnector.h"

/**
 * /brief TwitchConnector definition
 */
TwitchConnector::TwitchConnector(
                                  QString connectURL,
                                  quint16 port,
                                  QString loginName,
                                  QString loginPass,
                                  QStandardItemModel* channelModel,
                                  QLabel* connectionIcon,
                                  QPushButton* connectButton,
                                  QObject *parent)
    : QObject(parent),
    mSocket(nullptr),
    mConnectURL(connectURL),
    mPort(port),
    mCyclicTimer(nullptr),
    mState(DISCONNECTED),
    mLoginName(loginName),
    mLoginPass(loginPass),
    mChannelModel(channelModel),
    mConnectionIcon(connectionIcon),
    mConnectionButton(connectButton)
{
    /* Make sure no channel is tracked */
    mConnectedChannels.clear();

    /* Create a new tcp socket */
    mSocket = new QTcpSocket();

    /* Create a new cyclic timer */
    mCyclicTimer = new QTimer();

    /* Set intervall to 3 minutes for PING/PONG */
    mCyclicTimer->setInterval(180000);

    /* Connect the cyclic timer */
    connect(mCyclicTimer, SIGNAL(timeout()), this, SLOT(pingTimer()));

    /* Connect the tcp socket signals to corresponding methods */
    connect(mSocket, SIGNAL(connected()),                         this, SLOT(connected()));
    connect(mSocket, SIGNAL(disconnected()),                      this, SLOT(disconnected()));
    connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(mSocket, SIGNAL(readyRead()),                         this, SLOT(readyRead()));

    /* Connect the channel model signals to corresponding methods */
    connect(mChannelModel, SIGNAL(itemChanged(QStandardItem*)),       this, SLOT(channelModelItemChanged(QStandardItem*)));
    connect(mChannelModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(channelRowsChanged(QModelIndex,int,int)));
    connect(mChannelModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),  this, SLOT(channelRowsChanged(QModelIndex,int,int)));

    /* Set the ui elements to disabled */
    disconnectUI();
}

/**
 * /brief ~TwitchConnector definition
 */
TwitchConnector::~TwitchConnector()
{
    /* Check for null pointer and delete elements */
    if (mSocket != nullptr)
    {
        mSocket->disconnectFromHost();
        delete mSocket;
    }
    if (mCyclicTimer != nullptr)
    {
        delete mCyclicTimer;
    }
}

/**
 * /brief connected definition
 */
void TwitchConnector::connected()
{
    /* Create pass and nick strings */
    QString pass = "PASS " + mLoginPass + "\r\n";
    QString nick = "NICK " + mLoginName + "\r\n";

    /* Write pass and nick to tcp socket */
    mSocket->write(pass.toLocal8Bit());
    mSocket->write(nick.toLocal8Bit());
}

/**
 * /brief disconnected definition
 */
void TwitchConnector::disconnected()
{
    /* Set the ui elements to disabled */
    disconnectUI();
}

/**
 * /brief error definition
 */
void TwitchConnector::error(QAbstractSocket::SocketError error)
{
    /* In case of error disconnect from host */
    mSocket->disconnectFromHost();
}

/**
 * /brief readyRead definition
 */
void TwitchConnector::readyRead()
{
    /* Read all available data into a text stream */
    QTextStream textStream(mSocket->readAll().data());

    /* Iterate over all lines received */
    while (!textStream.atEnd())
    {
        /* Read a single line from received data */
        QString line = textStream.readLine();

        /* Parse successful connection message using regex */
        QRegularExpression successConnectMessage("(:tmi\\.twitch\\.tv)\\s([0-9]{3,3})\\s(.+?)\\s:(Welcome,\\sGLHF!)");
        QRegularExpressionMatch successConnectMessageMatch = successConnectMessage.match(line);

        /* If the regex matches */
        if (successConnectMessageMatch.hasMatch())
        {
            /* Set the ui elements to connected */
            connectUI();

            /* Start the ping/pong timer */
            mCyclicTimer->start();

            /* Iterate over all channels */
            for (int i=0; i<mChannelModel->rowCount(); i++)
            {
                /* If the channel shall be connected */
                if (Qt::Checked == mChannelModel->item(i)->checkState() )
                {
                    /* Create a join string */
                    QString join = "JOIN #" + mChannelModel->item(i)->text() + "\r\n";

                    /* Write join string to tcp socket */
                    mSocket->write(join.toLocal8Bit());

                }
            }

        }

        /* Parse oauth failed message using regex */
        QRegularExpression oauthFailedMessage("(:tmi.twitch.tv)\\s(NOTICE)\\s(\\*)\\s(:Improperly formatted auth)");
        QRegularExpressionMatch oauthFailedMessageMatch = oauthFailedMessage.match(line);

        /* Parse connect failed message using regex */
        QRegularExpression connectFailedMessage("(:tmi.twitch.tv)\\s(NOTICE)\\s(\\*)\\s(:Login authentication failed)");
        QRegularExpressionMatch connectFailedMessageMatch = connectFailedMessage.match(line);

        /* If one of the regex matches */
        if ( oauthFailedMessageMatch.hasMatch() ||
             connectFailedMessageMatch.hasMatch() )
        {
            /* Set the ui elements to connected */
            disconnectUI();

            /* Stop the cyclic ping/pong timer */
            mCyclicTimer->stop();
        }

        /* Parse channel connected message using regex */
        QRegularExpression channelConnected(":(.+)(.tmi.twitch.tv)\\s([0-9]{3,3})\\s(.+)\\s=\\s\\#(.+)\\s:(.+)");
        QRegularExpressionMatch channelConnectedMatch = channelConnected.match(line);

        /* If the regex matches */
        if ( channelConnectedMatch.hasMatch() )
        {
            /* Local channel name variable */
            QString channelName;

            /* The captured groups must be exactly 6 */
            if ( channelConnectedMatch.lastCapturedIndex() == 6 )
            {
                /* Channel name equals group nr. 5 */
                channelName = channelConnectedMatch.captured(5);

                /* Iterate over all channel items */
                for (int i=0; i<mChannelModel->rowCount(); i++)
                {
                    /* If channel name matches */
                    if  ( 0 == mChannelModel->item(i)->text().compare(channelName) )
                    {
                        /* Set the channel icon to OK */
                        mChannelModel->item(i)->setIcon(QIcon(":/icons/images/OK.png"));

                        /* Keep track of the connected channels */
                        mConnectedChannels.insert(channelName);
                    }
                }

            }
        }

        /* Parse channel disconnect message using regex */
        QRegularExpression channelDisconnected(":(.+)\\!(.+)\\@(.+)\\.tmi.twitch.tv\\sPART\\s\\#(.+)");
        QRegularExpressionMatch channelDisconnectedMatch = channelDisconnected.match(line);

        /* If the regex matches */
        if (channelDisconnectedMatch.hasMatch())
        {
            /* Local channel name variable */
            QString channelName;

            /* The captured groups must be exactly 4 */
            if ( channelDisconnectedMatch.lastCapturedIndex() == 4 )
            {
                /* Channel name equals group nr. 4 */
                channelName = channelDisconnectedMatch.captured(4);

                /* Iterate over all channel items */
                for (int i=0; i<mChannelModel->rowCount(); i++)
                {
                    /* If channel name matches */
                    if  ( 0 == mChannelModel->item(i)->text().compare(channelName) )
                    {
                        /* Set the channel icon to NOK */
                        mChannelModel->item(i)->setIcon(QIcon(":/icons/images/NOK.png"));

                        /* Keep track of the connected channels */
                        mConnectedChannels.remove(channelName);
                    }
                }
            }
        }
    }
}

/**
 * /brief pingTimer definition
 */
void TwitchConnector::pingTimer()
{
    /* Check if the socket is available and the twitch service is connected */
    if ( mState  == CONNECTED &&
         mSocket != nullptr )
    {
        /* Create ping string */
        QString ping = "PING\r\n";

        /* write ping string to tcp socket */
        mSocket->write(ping.toLocal8Bit());
    }
}

/**
 * /brief channelModelItemChanged definition
 */
void TwitchConnector::channelModelItemChanged(QStandardItem *item)
{
    /* Delegate to sub function */
    updateChannels();
}

/**
 * /brief channelRowsChanged definition
 */
void TwitchConnector::channelRowsChanged(const QModelIndex &parent, int start, int end)
{
    /* Delegate to sub function */
    updateChannels();
}

/**
 * /brief updateChannels definition
 */
void TwitchConnector::updateChannels()
{
    /* Check if socket is available and if twitch service is connected */
    if (mSocket == nullptr || mState != CONNECTED)
    {
        return;
    }

    /* Iterate over all channels */
    for (int i=0; i<mChannelModel->rowCount(); i++)
    {
        /* Get a single item from model */
        QStandardItem* item = mChannelModel->item(i);

        /* Get the channel name from item */
        QString channel = item->text();

        /* If the channel is connected and the check box is unchecked */
        if ( mConnectedChannels.contains(channel) &&
             item->checkState() == Qt::Unchecked )
        {
            /* Create disconnect string */
            QString disconnectString = "PART #" + channel + "\r\n";

            /* Write disconnect string to tcp socket */
            mSocket->write(disconnectString.toLocal8Bit());
        }
        /* If the channel is disconnected and the check box is checked */
        else if ( !mConnectedChannels.contains(channel) &&
                  item->checkState() == Qt::Checked )
        {
            /* Create connect string */
            QString connectString = "JOIN #" + channel + "\r\n";

            /* Write connect string to tcp socket */
            mSocket->write(connectString.toLocal8Bit());
        }
    }
}

/**
 * /brief connectingUI definition
 */
void TwitchConnector::connectingUI()
{
    /* Set internal state to connecting */
    mState = CONNECTING;

    /* Set connection icon to connecting */
    mConnectionIcon->setPixmap(QIcon(":/icons/images/refresh.png").pixmap(16,16));

    /* Disable the connection button while connecting */
    mConnectionButton->setEnabled(false);

    /* Change the connect button text to connecting */
    mConnectionButton->setText("Connecting");
}

/**
 * /brief disconnectUI definition
 */
void TwitchConnector::disconnectUI()
{
    /* Set internal state to disconnected */
    mState = DISCONNECTED;

    /* Remove all channels */
    mConnectedChannels.clear();

    /* Stop the ping/pong cyclic timer */
    mCyclicTimer->stop();

    /* Set connection icon to disconnected */
    mConnectionIcon->setPixmap(QIcon(":/icons/images/NOK.png").pixmap(32,32));

    /* Enable the connection button */
    mConnectionButton->setEnabled(true);

    /* Change the connect button text to connect */
    mConnectionButton->setText("Connect");

    /* Iterate over all channel items */
    for (int i=0; i<mChannelModel->rowCount(); i++)
    {
        /* Set all channel icons to disconnected */
        mChannelModel->item(i)->setIcon(QIcon(":/icons/images/NOK.png"));
    }
}

/**
 * /brief connectUI definition
 */
void TwitchConnector::connectUI()
{
    /* Set internal state to connected */
    mState = CONNECTED;

    /* Remove all channels */
    mConnectedChannels.clear();

    /* Set connection icon to connected */
    mConnectionIcon->setPixmap(QIcon(":/icons/images/OK.png").pixmap(32,32));

    /* Enable the connection button */
    mConnectionButton->setEnabled(true);

    /* Change the connect button text to disconnect */
    mConnectionButton->setText("Disconnect");
}

/**
 * /brief banUser definition
 */
QSet<QString> TwitchConnector::banUser(QString userName)
{
    /* Check if the socket and twitch service is available */
    if (mSocket == nullptr || mState != CONNECTED)
    {
        return QSet<QString>();
    }

    /* Local variable to gather all channels where the user is banned */
    QSet<QString> bannedChannels;

    /* Iterate over all connected channels */
    foreach (const QString& channel, mConnectedChannels)
    {
        /* Create ban command */
        QString banCommand = "PRIVMSG #"+ channel + " :.ban " + userName +"\r\n";

        /* Write ban command to tcp socket */
        mSocket->write(banCommand.toUtf8());

        /* Add the channel to keep track in which channels the user is banned */
        bannedChannels.insert(channel);
    }

    /* Return set of channels where the user actually has been banned */
    return bannedChannels;
}

/**
 * /brief unbanUser definition
 */
QSet<QString> TwitchConnector::unbanUser(QString userName)
{
    /* Check if the socket and twitch service is available */
    if (mSocket == nullptr || mState != CONNECTED)
    {
        return QSet<QString>();
    }

    /* Local variable to gather all channels where the user is unbanned */
    QSet<QString> unbannedChannels;

    /* Iterate over all connected channels */
    foreach (const QString& channel, mConnectedChannels)
    {
        /* Create unban command */
        QString unbanCommand = "PRIVMSG #"+ channel + " :.unban " + userName + "\r\n";

        /* Write unban command to tcp socket */
        mSocket->write(unbanCommand.toUtf8());

        /* Add the channel to keep track in which channels the user is unbanned */
        unbannedChannels.insert(channel);
    }

    /* Return set of channels where the user actually has been unbanned */
    return unbannedChannels;
}

/**
 * /brief Connect definition
 */
void TwitchConnector::Connect()
{
    /* Set the ui elements to connecting */
    connectingUI();

    /* Connect to twitch service*/
    mSocket->connectToHost( mConnectURL, mPort );
}

/**
 * /brief Disconnect definition
 */
void TwitchConnector::Disconnect()
{
    /* Disconnect from twitch service */
    mSocket->disconnectFromHost();
}

/**
 * /brief GetConnectionState definition
 */
ConnectionState TwitchConnector::GetConnectionState()
{
    /* Return the internal twitch service connection state */
    return mState;
}

/**
 * /brief SetLoginName definition
 */
void TwitchConnector::SetLoginName(QString loginName)
{
    /* Save the new login name internally */
    mLoginName = loginName;
}

/**
 * /brief SetOauth2 definition
 */
void TwitchConnector::SetOauth2(QString oauth2)
{
    /* Save the new login pass internally */
    mLoginPass = oauth2;
}


