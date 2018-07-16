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
 * /file    loginwindow.h
 * /author  Hackspider
 * /brief   Header file of the login window to gather the twitch user login credentials
 */

/* Include Guard */
#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

/* Necessary dependencies to Qt framework */
#include <QDialog>

/* MainWindowContent declaration as part of the Ui namespace */
namespace Ui {
    class LoginWindow;
}

/**
 * /brief  The LoginWindow class declaraation. Derived from QDialog class.
 */
class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * /brief  Constructor of the LoginWindow class
     * /param  parent  Parent of the LoginWindow object according to Qt's object tree
     */
    explicit LoginWindow(QWidget *parent = 0);

    /**
     * /brief  Destructor of the IconRightStyledItemDelegate class
     */
    ~LoginWindow();

    /**
     * /brief   After the dialog is closed the caller can get the login name information
     * /return  Returns login name of the form
     */
    QString GetLoginName(void);

    /**
     * /brief   After the dialog is closed the caller can get the OAuth2 information
     * /return  Returns OAuth2 of the form
     */
    QString GetOAuth2(void);

    /**
     * /brief   After the dialog is closed the caller can get the information if the user credentials shall be saved
     * /return  Returns true if the credentiasl shall be saved
     */
    bool GetSaveCredentials(void);

    /**
     * /brief  Method to set the login name before the dialog is shown
     * /param  loginName  Name to be set to line edit
     */
    void SetLoginName(QString loginName);

    /**
     * /brief  Method to set the oauth2 before the dialog is shown
     * /param  oauth  OAuth2 to be set to line edit
     */
    void SetOAuth2Name(QString oauth);

    /**
     * /brief  Method to set the save credentials checkbox before the dialog is shown
     * /param  save  Indication to safe credentials
     */
    void SetSaveCredentials(bool save);

private slots:

    /**
     * /brief  Method called if user clicks on the dialogs ok button
     */
    void on_loginButtonBox_accepted();

    /**
     * /brief  Method called if user clicks on the dialogs cancel button
     */
    void on_loginButtonBox_rejected();

private:
    /**
     * /brief Internal used variables
     */
    Ui::LoginWindow *ui;
    QString mLoginName;
    QString mOAuth2;
    bool mSaveCredentials;
};

#endif // LOGINWINDOW_H
