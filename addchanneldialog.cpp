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
 * /file    addchanneldialog.cpp
 * /author  Hackspider
 * /brief   Source file of the dialog to add a new twitch channel
 */

/* Necessary internal dependencies */
#include "addchanneldialog.h"
#include "ui_addchanneldialog.h"

/**
 * /brief AddChannelDialog definition
 */
AddChannelDialog::AddChannelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChannelDialog),
    mChannel(""),
    mDefaultConnect(false)
{
    /* Setup internal ui elements */
    ui->setupUi(this);
}

/**
 * /brief Destructor definition
 */
AddChannelDialog::~AddChannelDialog(void)
{
    /* Remove interanl ui elements from heap */
    delete ui;
}

/**
 * /brief GetChannel definition
 */
QString AddChannelDialog::GetChannel(void)
{
    /* Return internally stored channel name */
    return mChannel;
}

/**
 * /brief GetDefaultConnect definition
 */
bool AddChannelDialog::GetDefaultConnect(void)
{
    /* Return internally stored default connect information */
    return mDefaultConnect;
}

/**
 * /brief on_dialogButtons_accepted definition
 */
void AddChannelDialog::on_dialogButtons_accepted(void)
{
    /* Get the channel name from ui element and store it internally */
    mChannel = ui->channelLineEdit->text();

    /* Get the default connect information from ui element and store it internally */
    if ( Qt::Checked == ui->defaultConnectCheckBox->checkState())
    {
        mDefaultConnect = true;
    }
    else
    {
        mDefaultConnect = false;
    }

    /* Call the accept method from parent QDialog */
    accept();
}

/**
 * /brief on_dialogButtons_rejected definition
 */
void AddChannelDialog::on_dialogButtons_rejected(void)
{
    /* Call the reject method from parent QDialog */
    reject();
}
