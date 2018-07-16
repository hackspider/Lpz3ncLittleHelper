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
 * /file    mainwindowcontent.h
 * /author  Hackspider
 * /brief   Header file of the main window
 */

/* Include Guard */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* Necessary dependencies to Qt framework */
#include <QMainWindow>
#include <QStandardItemModel>
#include <QSettings>

/* Necessary internal dependencies */
#include "iconrightstyleditemdelegate.h"
#include "twitchconnector.h"

/* Configuration file that is used to store the channels, users, login permanently */
#define CONFIG_FILE "config.ini"

/* MainWindowContent declaration as part of the Ui namespace */
namespace Ui
{
    class MainWindowContent;
}

/**
 * /brief  The MainWindowContent class declaraation. Derived from QMainWindow class.
 */
class MainWindowContent : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * /brief  Constructor of the MainWindowContent class
     * /param  parent  Parent of the MainWindowContent object according to Qt's object tree
     */
    explicit MainWindowContent(QWidget *parent = 0);

    /**
     * /brief  Destructor of the MainWindowContent class
     */
    ~MainWindowContent();

private slots:

    /**
     * /brief  Method called if user clicks on the add channel button
     */
    void on_addChannelButton_clicked();

    /**
     * /brief  Method called if user clicks on the remove channel button
     */
    void on_removeChannelButton_clicked();

    /**
     * /brief  Method called if user clicks into the user panel
     * /param  index  Index of the item where the user clicked on
     */
    void on_users_clicked(const QModelIndex &index);

    /**
     * /brief  Method called if user clicks on the ban user button
     */
    void on_banUserButton_clicked();

    /**
     * /brief  Method called if user clicks on the unban user button
     */
    void on_unbanUserButton_clicked();

    /**
     * /brief  Method called if the channel model (MVC) changed
     * /param  item  Pointer to the item which has been changed
     */
    void channelModelItemChanged(QStandardItem* item);

    /**
     * /brief  Method called if a row of the channel model (MVC) changed
     */
    void channelRowsChanged(const QModelIndex & parent, int start, int end);

    /**
     * /brief  Method called if the user model (MVC) changed
     * /param  item  Pointer to the item which has been changed
     */
    void userModelItemChanged(QStandardItem* item);

    /**
     * /brief  Method called if a row of the user model (MVC) changed
     */
    void userRowsChanged(const QModelIndex & parent, int start, int end);

    /**
     * /brief  Internally used to write channel settings into config file
     */
    void rewriteChannelSettings(void);

    /**
     * /brief  Internally used to write user settings into config file
     */
    void rewriteUserSettings(void);

    /**
     * /brief  Internally used to get user credentials and connect to twitch
     * /param  forceDialog  To force the display of the dialog set to true
     */
    void loginConnect(bool forceDialog = false);

    /**
     * /brief   Helper function to get a formatted timestamp
     * /return
     */
    QString getCurrentTimestamp(void);

    /**
     * /brief   Internal method to ban a user
     * /param   username  Name of the user that should be banned
     * /return  List of channels where the user has been banned
     */
    QList<QString> banUser(QString username);

    /**
     * /brief   Internal method to unban a user
     * /param   username  Name of the user that should be unbanned
     * /return  List of channels where the user has been unbanned
     */
    QList<QString> unbanUser(QString username);

    /**
     * /brief  Method called if user clicks on the login menu item
     */
    void on_actionLogin_triggered();

    /**
     * /brief  Method called if user clicks on the quit menu item
     */
    void on_actionQuit_triggered();

    /**
     * /brief  Method called if user clicks on the connect button
     */
    void on_connectButton_clicked();

private:
    /**
     * /brief Internal used variables
     */
    Ui::MainWindowContent* ui;
    QSettings* mSettings;
    QString mLoginName;
    QString mOauth2;
    bool mSaveCredentials;
    IconRightStyledItemDelegate* mIconRightDelegate;
    QStandardItemModel* mChannelModel;
    QStandardItemModel* mUserModel;
    TwitchConnector* mTwitchConnector;

};

#endif /* MAINWINDOW_H */
