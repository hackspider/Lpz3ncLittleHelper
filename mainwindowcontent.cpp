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
 * /file    mainwindowcontent.cpp
 * /author  Hackspider
 * /brief   Source file of the main window
 */

/* Necessary dependencies to Qt framework */
#include <QSettings>
#include <QStandardItem>
#include <QDebug>
#include <QTimer>

/* Necessary internal dependencies */
#include "framelesswindow.h"
#include "mainwindowcontent.h"
#include "ui_mainwindowcontent.h"
#include "addchanneldialog.h"
#include "loginwindow.h"

/**
 * /brief MainWindowContent definition
 */
MainWindowContent::MainWindowContent(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowContent),
    mSettings(nullptr),
    mLoginName(""),
    mOauth2(""),
    mSaveCredentials(true),
    mIconRightDelegate(nullptr),
    mChannelModel(nullptr),
    mUserModel(nullptr),
    mTwitchConnector(nullptr)
{
    /*=================================================================*/
    /*===                          UI Setup                         ===*/
    /*=================================================================*/

    /* Setup all widgets of the ui (avoids seg faults if accessed below) */
    ui->setupUi(this);

    /* Create and set a new item delegate to show the icons of the channel list on the right side (status if channel is connected) */
    mIconRightDelegate = new IconRightStyledItemDelegate(this);
    ui->channels->setItemDelegate(mIconRightDelegate);

    /* Create and set the item model for the channel list */
    mChannelModel = new QStandardItemModel();
    ui->channels->setModel(mChannelModel);

    /* Create user model for the user list */
    mUserModel = new QStandardItemModel();

    /* Set the header for user model */
    mUserModel->setHorizontalHeaderItem(0,new QStandardItem(QString("Timestamp")));
    mUserModel->setHorizontalHeaderItem(1,new QStandardItem(QString("Username")));
    mUserModel->setHorizontalHeaderItem(2,new QStandardItem(QString("Channels")));
    mUserModel->setHorizontalHeaderItem(3,new QStandardItem(QString("Reason")));
    mUserModel->setHorizontalHeaderItem(4,new QStandardItem(QString("URL")));

    /* Set the model to the corresponding UI element */
    ui->users->setModel(mUserModel);

    /* Make the user items left align */
    ui->users->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    /* Preset the columnwidth of the user list */
    ui->users->setColumnWidth(0,135);
    ui->users->setColumnWidth(1,120);
    ui->users->setColumnWidth(2,323);
    ui->users->setColumnWidth(3,200);
    ui->users->setColumnWidth(4,200);

    /*=================================================================*/
    /*===                          Settings                         ===*/
    /*=================================================================*/

    /* Set settings file */
    mSettings = new QSettings(CONFIG_FILE, QSettings::IniFormat);

    /* Read username and oauth2 from settings. If username or oauth2 is missing set values to an empty String */
    mLoginName = mSettings->value("loginName","").toString();
    mOauth2 = mSettings->value("oauth2","").toString();

    /* Check if setting to save credentials is available and set */
    if ( 0 == mSettings->value("saveCredentials","").toString().compare("true", Qt::CaseInsensitive) )
    {
        mSaveCredentials = true;
    }

    /* Read all channels from config file */
    int channelCount = mSettings->beginReadArray("channels");

    for (int i =  0; i<channelCount; i++)
    {
        mSettings->setArrayIndex(i);

        /* Read a channel name and if the channel should be connected on startup */
        QString currentChannel = mSettings->value("channelName","").toString();
        QString currentChannelDefualtConnect = mSettings->value("channelDefaultConnect","").toString();

        /* Remove whitespaces from channel name */
        currentChannel = currentChannel.simplified().replace(" ","");

        /* Check if defualt connect is true */
        currentChannelDefualtConnect = currentChannelDefualtConnect.simplified().replace(" ","");

        bool defaultConnect = false;
        if ( 0 == currentChannelDefualtConnect.compare("true", Qt::CaseInsensitive) )
        {
            defaultConnect = true;
        }

        /* Create objects for every channel */
        if ( !currentChannel.isEmpty() )
        {
            QStandardItem* channel = new QStandardItem(currentChannel);
            channel->setCheckable(true);
            channel->setIcon(QIcon(":/icons/images/NOK.png"));

            if (defaultConnect == true)
            {
                channel->setCheckState(Qt::Checked);
            }
            else
            {
                channel->setCheckState(Qt::Unchecked);
            }

            mChannelModel->appendRow(channel);
        }

    }

    /* Complete array */
    mSettings->endArray();

    /* Get the count of the users of the config file */
    int userCount = mSettings->beginReadArray("users");

    /* Iterate over all users */
    for (int i=0; i<userCount; i++)
    {
        mSettings->setArrayIndex(i);

        /* Read user properties from config file */
        QString userTimestamp = mSettings->value("timestamp").toString();
        QString userName = mSettings->value("username").toString().simplified().replace(" ","");
        QString userChannels = mSettings->value("channelNames").toString().simplified().replace(" ","");
        QString userBanReason = mSettings->value("banReason").toString();
        QString userURLReason = mSettings->value("banURL").toString();

        /* Create a item form timestamp */
        QStandardItem* userTimestampItem = new QStandardItem();
        userTimestampItem->setText(userTimestamp);

        /* Create a item form username */
        QStandardItem* userNameItem = new QStandardItem();
        userNameItem->setText(userName);

        /* Create a item form channels */
        QStandardItem* userChannelsItem = new QStandardItem();
        userChannelsItem->setText(userChannels);

        /* Create a item form ban reason */
        QStandardItem* userBanReasonItem = new QStandardItem();
        userBanReasonItem->setText(userBanReason);

        /* Create a item form URL */
        QStandardItem* userURLReasonItem = new QStandardItem();
        userURLReasonItem->setText(userURLReason);

        /* Create a list of all items */
        QList<QStandardItem*> userRowList;

        /* Push all created items into the list */
        userRowList << userTimestampItem << userNameItem << userChannelsItem << userBanReasonItem << userURLReasonItem;

        /* Add a row based on the list created */
        mUserModel->appendRow(userRowList);
    }

    /* Complete array */
    mSettings->endArray();

    /*=================================================================*/
    /*===                        Connections                        ===*/
    /*=================================================================*/

    /* If the model changed call the corresponding notification method */
    connect(mChannelModel, SIGNAL(itemChanged(QStandardItem*)),       this, SLOT(channelModelItemChanged(QStandardItem*)));
    connect(mChannelModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),  this, SLOT(channelRowsChanged(QModelIndex,int,int)));
    connect(mChannelModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(channelRowsChanged(QModelIndex,int,int)));

    connect(mUserModel, SIGNAL(itemChanged(QStandardItem*)),       this, SLOT(userModelItemChanged(QStandardItem*)));
    connect(mUserModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),  this, SLOT(userRowsChanged(QModelIndex,int,int)));
    connect(mUserModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(userRowsChanged(QModelIndex,int,int)));

    /* Let the ui load and get credentials afterwards */
    QTimer::singleShot(0,this,SLOT(loginConnect()));

}

/**
 * /brief ~MainWindowContent definition
 */
MainWindowContent::~MainWindowContent()
{
    /* Check for null pointer and delete all objects from heap */

    if (nullptr != mIconRightDelegate)
    {
        delete mIconRightDelegate;
    }

    if (nullptr != mChannelModel)
    {
        delete mChannelModel;
    }

    if (nullptr != mSettings)
    {
        delete mSettings;
    }

    if (nullptr != mUserModel)
    {
        delete mUserModel;
    }

    if (nullptr != ui)
    {
        delete ui;
    }

    if (nullptr != mTwitchConnector)
    {
        delete mTwitchConnector;
    }

}

/**
 * /brief on_addChannelButton_clicked definition
 */
void MainWindowContent::on_addChannelButton_clicked()
{
    /* We need a new window for the user to enter the information regarding the new channel */
    FramelessWindow addChannelWindow;
    AddChannelDialog addChannelDialog;

    /* Set the window title */
    addChannelWindow.setWindowTitle("Add Channel");

    /* Set the content of the frameless window to our 'add channel dialog' */
    addChannelWindow.setContent(&addChannelDialog);

    /* Block the main program until the user entered the channel information */
    addChannelWindow.exec();

    /* Proceed only if the dialog was accepted */
    if ( 1 == addChannelDialog.result())
    {
        /* First check if the channel is already added */
        QString channelName = addChannelDialog.GetChannel().simplified().replace(" ","");
        for (int i =0; i<mChannelModel->rowCount(); i++)
        {
            /* Compare channel name with all channels in the model */
            if (mChannelModel->item(i)->text().compare(channelName)==0)
            {
                /* If channel is already in the list, nothing to do */
                return;
            }
        }

        /* Remove all whitespaces from entered channel name and check if it's not empty */
        if ( !addChannelDialog.GetChannel().simplified().replace(" ","").isEmpty() )
        {
            /* Create a new channel object and put it into the channel model */
            QStandardItem* newTwichChannel = new QStandardItem(channelName);
            newTwichChannel->setCheckable(true);
            newTwichChannel->setIcon(QIcon(":/icons/images/NOK.png"));

            if (addChannelDialog.GetDefaultConnect() == true)
            {
                newTwichChannel->setCheckState(Qt::Checked);
            }
            else
            {
                newTwichChannel->setCheckState(Qt::Unchecked);
            }

            mChannelModel->appendRow(newTwichChannel);

            /* To keep an alphabetical order sort the channel model */
            mChannelModel->sort(0);
        }
    }


}

/**
 * /brief on_removeChannelButton_clicked definition
 */
void MainWindowContent::on_removeChannelButton_clicked()
{
    /* Get the selection model and the list of selected items from model */
    QItemSelectionModel* selectionModel = ui->channels->selectionModel();
    QModelIndexList selectedChannels = selectionModel->selectedRows();

    /* Delete all selected channels */
    for (int i=0; i<selectedChannels.count(); i++)
    {
        /* Get the index of a selected channel */
        QModelIndex channelIndex =  selectedChannels.at(i);

        /* Remove the selected channel (by index) and take over ownership */
        QList<QStandardItem*> rowsToRemove = mChannelModel->takeRow(channelIndex.row());

        /* Also it is one row we need to satisfy the Qt's API and iterate over all entries from takerow */
        for (int j = 0; j<rowsToRemove.count(); j++)
        {
            /* Conversion from the QStandardItem to our own implementation */
            QStandardItem* toRemove = rowsToRemove.at(j);

            /* Finally delete the Twich channel object complete from heap */
            delete toRemove;
        }

        /* To keep an alphabetical order sort the channel model */
        mChannelModel->sort(0);

    }

}

/**
 * /brief rewriteChannelSettings definition
 */
void MainWindowContent::rewriteChannelSettings()
{
    /* First of all remove all channels from config */
    mSettings->remove("channels");

    /* Start the new channels array */
    mSettings->beginWriteArray("channels");

    /* Iterate over the whole channel model and store every entry in a separate settings object */
    for (int i = 0; i< mChannelModel->rowCount(); i++)
    {
        /* Set the current array index based on the channel model */
        mSettings->setArrayIndex(i);

        /* Save the channel name */
        mSettings->setValue("channelName",mChannelModel->item(i)->text());

        /* Save the default connect value based on the check state of the channel's check box */
        if ( Qt::Checked == mChannelModel->item(i)->checkState())
        {
            mSettings->setValue("channelDefaultConnect","true");
        }
        else
        {
            mSettings->setValue("channelDefaultConnect","false");
        }
    }

    /* Complete array */
    mSettings->endArray();
}

/**
 * /brief rewriteUserSettings definition
 */
void MainWindowContent::rewriteUserSettings()
{
    /* First of all remove all users from config */
    mSettings->remove("users");

    /* Start the new user array */
    mSettings->beginWriteArray("users");

    /* Iterate over the whole user model and store every entry in a separate settings object */
    for (int i = 0; i< mUserModel->rowCount(); i++)
    {
        /* Set the current array index based on the user model */
        mSettings->setArrayIndex(i);

        /* Save the timestamp */
        mSettings->setValue("timestamp",mUserModel->item(i,0)->text());

        /* Save the username */
        mSettings->setValue("username",mUserModel->item(i,1)->text());

        /* Save the channel names */
        mSettings->setValue("channelNames",mUserModel->item(i,2)->text());

        /* Save the ban reason */
        mSettings->setValue("banReason",mUserModel->item(i,3)->text());

        /* Save the ban reason */
        mSettings->setValue("banURL",mUserModel->item(i,4)->text());
    }

    /* Complete array */
    mSettings->endArray();

}

/**
 * /brief loginConnect definition
 */
void MainWindowContent::loginConnect(bool forceDialog)
{
    /* Check if it is necessary to show a credentials dialog */
    if ( mSaveCredentials == false ||
         mOauth2.isEmpty() ||
         mLoginName.isEmpty() ||
         forceDialog == true )
    {
        /* We need a new window for the user to enter the information regarding the login data */
        FramelessWindow loginWindow;
        LoginWindow loginDialog;

        /* Set the window title */
        loginWindow.setWindowTitle("User Credentials");

        /* Set the content of the frameless window to our 'login dialog' */
        loginWindow.setContent(&loginDialog);

        /* For convenience reason set the current settings to the ui elements */
        loginDialog.SetLoginName(mLoginName);
        loginDialog.SetOAuth2Name(mOauth2);
        loginDialog.SetSaveCredentials(mSaveCredentials);

        /* Block the main program until the user credential information */
        loginWindow.exec();

        /* Get the values from dialog */
        mLoginName = loginDialog.GetLoginName();
        mOauth2 = loginDialog.GetOAuth2();
        mSaveCredentials = loginDialog.GetSaveCredentials();
    }

    /* Check if the credentials shall be saved */
    if (mSaveCredentials)
    {
        /* Save credentials to file */
        mSettings->setValue("loginName",mLoginName);
        mSettings->setValue("oauth2",mOauth2);
        mSettings->setValue("saveCredentials","true");
    }
    else
    {
        /* If credentials shall not be saved empty the config */
        mSettings->setValue("loginName","");
        mSettings->setValue("oauth2","");
        mSettings->setValue("saveCredentials","false");
    }

    /* Create a new connector to twitch if necessary */
    if ( mTwitchConnector == nullptr )
    {
        mTwitchConnector = new TwitchConnector( "irc.twitch.tv",
                                                6667,
                                                mLoginName,
                                                mOauth2,
                                                mChannelModel,
                                                ui->connectionIcon,
                                                ui->connectButton,
                                                this );
    }
    else
    {
        /* Update login name and oauth2 of the twitch connector */
        mTwitchConnector->SetLoginName(mLoginName);
        mTwitchConnector->SetOauth2(mOauth2);
    }
}

/**
 * /brief channelModelItemChanged definition
 */
void MainWindowContent::channelModelItemChanged(QStandardItem *item)
{
    /* Update settings file */
    rewriteChannelSettings();
}

/**
 * /brief channelRowsChanged definition
 */
void MainWindowContent::channelRowsChanged(const QModelIndex &parent, int start, int end)
{
    /* Update settings file */
    rewriteChannelSettings();
}

/**
 * /brief userModelItemChanged definition
 */
void MainWindowContent::userModelItemChanged(QStandardItem *item)
{
    /* Update settings file */
    rewriteUserSettings();
}

/**
 * /brief userRowsChanged definition
 */
void MainWindowContent::userRowsChanged(const QModelIndex &parent, int start, int end)
{
    /* Update settings file */
    rewriteUserSettings();
}

/**
 * /brief on_users_clicked definition
 */
void MainWindowContent::on_users_clicked(const QModelIndex &index)
{
    /* Make only the reason column editable and get the current flags from model */
    Qt::ItemFlags flags = mUserModel->itemFromIndex(index)->flags();

    /* If column "Reason" or "URL" is selected set item to editable else forbid editing of cells */
    if (index.column() == 3 || index.column() == 4)
    {
        flags |= Qt::ItemIsEditable;
    }
    else
    {
        flags &= ~Qt::ItemIsEditable;
    }

    /* Set the new flags to model */
    mUserModel->itemFromIndex(index)->setFlags(flags);

    /* Based of the row selected set the username and reason line edit to the selected value (UX reasons) */
    ui->usernameLineEdit->setText( mUserModel->item(index.row(),1)->text() );
    ui->reasonLineEdit->setText( mUserModel->item(index.row(),3)->text() );
}

/**
 * /brief getCurrentTimestamp definition
 */
QString MainWindowContent::getCurrentTimestamp()
{
    /* Define the time/date format */
    QString date = QDate::currentDate().toString("yyyy.MM.dd");
    QString time = QTime::currentTime().toString("hh:mm:ss");

    /* Concatenate date and time and return the string */
    return date.append(" ").append(time);
}

/**
 * /brief on_banUserButton_clicked definition
 */
void MainWindowContent::on_banUserButton_clicked()
{
    /* First of all check if the username is formally correct */
    QString username = ui->usernameLineEdit->text();
    username = username.simplified().replace(" ","");

    /* If username is incorrect do nothing */
    if (username.isEmpty())
    {
        return;
    }

    /* Despite the username the reason is allowed to leave empty */
    QString banReason = ui->reasonLineEdit->text();

    /* Despite the username the url is allowed to leave empty */
    QString urlReason = ui->urlReasonLineEdit->text();

    /* Perform ban on all connected channels */
    QList<QString> bannedChannels = banUser(username);

    /* If there is no channel ban performed, do not track the user in the list */
    if (bannedChannels.isEmpty())
    {
        return;
    }

    /* Check if the user is already in the list */
    int rowFound = -1;
    for (int i=0; i<mUserModel->rowCount(); i++)
    {
        QStandardItem* currentUser = mUserModel->item(i,1);

        if (currentUser->text().compare(username) == 0)
        {
            rowFound = i;
            break;
        }
    }

    /* Pointer to the cells we are using */
    QStandardItem* timestamp = nullptr;
    QStandardItem* user = nullptr;
    QStandardItem* channels = nullptr;
    QStandardItem* reason = nullptr;
    QStandardItem* url = nullptr;

    /* Based on if we found a user reuse the row, otherwise create a new row */
    if (rowFound != -1)
    {
        timestamp = mUserModel->item(rowFound,0);
             user = mUserModel->item(rowFound,1);
         channels = mUserModel->item(rowFound,2);
           reason = mUserModel->item(rowFound,3);
              url = mUserModel->item(rowFound,4);

           reason->setText(banReason);
           url->setText(urlReason);
    }
    else
    {
        timestamp = new QStandardItem();
             user = new QStandardItem(username);
         channels = new QStandardItem("");
           reason = new QStandardItem(banReason);
              url = new QStandardItem(urlReason);

           QList<QStandardItem*> newRow;

           newRow << timestamp << user << channels << reason << url;

           mUserModel->appendRow(newRow);
    }

    /* Update timestamp */
    timestamp->setText(getCurrentTimestamp());

    /* Update ban list --> ';' separated string of channels */
    QStringList currentChannels = channels->text().split(';',QString::SkipEmptyParts);

    for (int i=0; i<bannedChannels.count(); i++)
    {
        if ( !currentChannels.contains(bannedChannels.at(i)) )
        {
            currentChannels << bannedChannels.at(i);
        }
    }

    /* Add new channels to the list */
    QString newChannels;
    bool newChannelsAdded = false;
    for (int i=0; i<currentChannels.count(); i++)
    {
        newChannels.append(currentChannels.at(i)).append(";");
        newChannelsAdded = true;
    }

    /* If a new channel is added remove the last ';' */
    if (newChannelsAdded)
    {
        newChannels.remove(newChannels.count()-1,1);
    }

    /* Set the new channels to the user model */
    channels->setText(newChannels);

}

/**
 * /brief on_unbanUserButton_clicked definition
 */
void MainWindowContent::on_unbanUserButton_clicked()
{
    /* First of all check if the username is formally correct */
    QString username = ui->usernameLineEdit->text();
    username = username.simplified().replace(" ","");

    /* If username is incorrect do nothing */
    if (username.isEmpty())
    {
        return;
    }

    /* Perform unban on all connected channels */
    QList<QString> unbannedChannels = unbanUser(username);

    /* If there is no channel unban performed, do nothing */
    if (unbannedChannels.isEmpty())
    {
        return;
    }

    /* Check if the user is already in the list */
    int rowFound = -1;
    for (int i=0; i<mUserModel->rowCount(); i++)
    {
        QStandardItem* currentUser = mUserModel->item(i,1);

        if (currentUser->text().compare(username) == 0)
        {
            rowFound = i;
            break;
        }
    }

    /* If the user is not tracked in the list, tracking is not necessary */
    if (rowFound == -1)
    {
        return;
    }

    /* Get the channel from the found row */
    QStandardItem* channels = mUserModel->item(rowFound,2);

    /* Get ban list --> ';' separated string of channels */
    QStringList currentChannels = channels->text().split(';',QString::SkipEmptyParts);

    /* Remove all unbanned channels from the entry */
    for (int i=0; i<unbannedChannels.count(); i++)
    {
        if ( currentChannels.contains(unbannedChannels.at(i)) )
        {
            currentChannels.removeAll(unbannedChannels.at(i));
        }
    }

    /* If user isn't banned at at least one channel remove the entry completely */
    if (currentChannels.isEmpty())
    {
        mUserModel->removeRow(rowFound);
        return;
    }

    /* Add all remaining channels to the list */
    QString allChannels;
    for (int i=0; i<currentChannels.count(); i++)
    {
        allChannels.append(currentChannels.at(i)).append(";");
    }

    /* If a new channel is added remove the last ';' */
    allChannels.remove(allChannels.count()-1,1);

    /* Set the new channels to the user model */
    channels->setText(allChannels);
}

/**
 * /brief banUser definition
 */
QList<QString> MainWindowContent::banUser(QString username)
{
    /* Ban the user and return the channels where the user was banned */
    QSet<QString> channelsBannedSet;
    channelsBannedSet = mTwitchConnector->banUser(username);
    return channelsBannedSet.toList();

}

/**
 * /brief unbanUser definition
 */
QList<QString> MainWindowContent::unbanUser(QString username)
{
    /* Unban the user and return the channels where the user was unbanned */
    QSet<QString> channelsUnbannedSet;
    channelsUnbannedSet = mTwitchConnector->unbanUser(username);
    return channelsUnbannedSet.toList();
}

/**
 * /brief on_actionLogin_triggered definition
 */
void MainWindowContent::on_actionLogin_triggered()
{
    /* Connect to twitch service and force the login credential dialog */
    loginConnect(true);
}

/**
 * /brief on_actionQuit_triggered definition
 */
void MainWindowContent::on_actionQuit_triggered()
{
    /* Quit the application */
    exit(0);
}

/**
 * /brief on_connectButton_clicked definition
 */
void MainWindowContent::on_connectButton_clicked()
{
    /* Based on the twitch service connection state */
    if (mTwitchConnector->GetConnectionState() == DISCONNECTED)
    {
        /* Connect to twitch service */
        mTwitchConnector->Connect();
    }
    else if (mTwitchConnector->GetConnectionState() == CONNECTED)
    {
        /* Disconnect from twitch service */
        mTwitchConnector->Disconnect();
    }
    else
    {
        /* Intentionally do nothing */
    }
}
