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
 * /file    loginwindow.cpp
 * /author  Hackspider
 * /brief   Source file of the login window to gather the twitch user login credentials
 */

/* Necessary dependencies to Qt framework */
#include <QDebug>
#include <QDesktopServices>
#include <QMessageBox>

/* Necessary internal dependencies */
#include "mainwindowcontent.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"

/**
 * /brief LoginWindow definition
 */
LoginWindow::LoginWindow( QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow),
    mLoginName(""),
    mOAuth2(""),
    mSaveCredentials(false)
{
    /* Setup the ui */
    ui->setupUi(this);
}

/**
 * /brief ~LoginWindow definition
 */
LoginWindow::~LoginWindow()
{
    /* Delete all ui elements */
    delete ui;
}

/**
 * /brief GetLoginName definition
 */
QString LoginWindow::GetLoginName()
{
    /* Return the internal login name */
    return mLoginName;
}

/**
 * /brief GetOAuth2 definition
 */
QString LoginWindow::GetOAuth2()
{
    /* Return the internal oauth2 */
    return mOAuth2;
}

/**
 * /brief GetSaveCredentials definition
 */
bool LoginWindow::GetSaveCredentials()
{
    /* Return if the credentials shall be saved */
    return mSaveCredentials;
}

/**
 * /brief SetLoginName definition
 */
void LoginWindow::SetLoginName(QString loginName)
{
    /* Set the login name to line edit */
    ui->usernameLineEdit->setText(loginName);
}

/**
 * /brief SetOAuth2Name definition
 */
void LoginWindow::SetOAuth2Name(QString oauth)
{
    /* Set the oauth2 to line edit */
    ui->oauthLineEdit->setText(oauth);
}

/**
 * /brief SetSaveCredentials definition
 */
void LoginWindow::SetSaveCredentials(bool save)
{
    /* Based on the parameter set the save credential checkbox */
    if (save == true)
    {
        ui->saveCredentialsCheckBox->setCheckState(Qt::Checked);
    }
    else
    {
        ui->saveCredentialsCheckBox->setCheckState(Qt::Unchecked);
    }
}

/**
 * /brief on_loginButtonBox_accepted definition
 */
void LoginWindow::on_loginButtonBox_accepted()
{
    /* Get the username from ui element */
    mLoginName = ui->usernameLineEdit->text();

    /* Get the OAuth2 from ui element */
    mOAuth2 = ui->oauthLineEdit->text();

    /* Get the save credentials information from ui element */
    if (ui->saveCredentialsCheckBox->checkState() == Qt::Checked)
    {
        mSaveCredentials = true;
    }
    else
    {
        mSaveCredentials = false;
    }

    /* Call the parents accept method to close the dialog */
    accept();
}

void LoginWindow::on_loginButtonBox_rejected()
{
    /* Empty the internal variables to prevent false data */
    mLoginName = "";
    mOAuth2 = "";
    mSaveCredentials = false;

    /* Call the parents reject method to close the dialog */
    reject();
}
