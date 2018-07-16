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
 * /file    addchanneldialog.h
 * /author  Hackspider
 * /brief   Header file of the dialog to add a new twitch channel
 */

/* Include Guard */
#ifndef ADDCHANNELDIALOG_H
#define ADDCHANNELDIALOG_H

/* Necessary dependencies to Qt framework */
#include <QDialog>

/* AddChannelDialog declaration as part of the Ui namespace */
namespace Ui
{
    class AddChannelDialog;
}

/**
 * /brief  The AddChannelDialog class declaraation. Derived from QDialog class.
 */
class AddChannelDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * /brief  Constructor of the AddChannelDialog class
     * /param  parent  Parent of the AddChannelDialog object according to Qt's object tree
     */
    explicit AddChannelDialog(QWidget *parent = 0);

    /**
     * /brief  Destructor of the AddChannelDialog class
     */
    ~AddChannelDialog(void);

    /**
     * /brief   After the dialog is closed the caller can get the new channel name by calling this method
     * /return  New channel name
     */
    QString GetChannel(void);

    /**
     * /brief   After the dialog is closed the caller can get the information if the channel should be connected on startup
     * /return  True if the channel should be connected on startup otherwise false
     */
    bool GetDefaultConnect(void);

private slots:
    /**
     * /brief Internal method that will be called if the user presses OK on the dialog
     */
    void on_dialogButtons_accepted(void);

    /**
     * /brief Internal method that will be called if the user presses Cancel on the dialog
     */
    void on_dialogButtons_rejected(void);

private:
    /**
     * /brief Internal used variables
     */
    Ui::AddChannelDialog *ui;
    QString mChannel;
    bool mDefaultConnect;
};

#endif /* ADDCHANNELDIALOG_H */
