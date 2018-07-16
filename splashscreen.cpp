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
 * /file    splashscreen.cpp
 * /author  Hackspider
 * /brief   Source file of the splash screen
 */

/* Necessary dependencies to Qt framework */
#include <QTimer>

/* Necessary internal dependencies */
#include "splashscreen.h"
#include "ui_splashscreen.h"

/**
 * /brief SplashScreen definition
 */
SplashScreen::SplashScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SplashScreen)
{

    /* Set the window option to frameless window to let dark style window to overtake */
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    /* Set no system background and translucent to have a transparent window */
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);

    /* Setup internal ui elements */
    ui->setupUi(this);

    /* Show the dialog for 2 seconds */
    QTimer::singleShot(2000,this,SLOT(close()));
}

/**
 * /brief ~SplashScreen definition
 */
SplashScreen::~SplashScreen()
{
    /* Delete internal ui elements from heap */
    delete ui;
}
