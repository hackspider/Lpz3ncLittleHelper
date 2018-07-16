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
 * /file    splashscreen.h
 * /author  Hackspider
 * /brief   Header file of the splash screen
 */

/* Include Guard */
#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

/* Necessary dependencies to Qt framework */
#include <QDialog>

/* SplashScreen declaration as part of the Ui namespace */
namespace Ui
{
    class SplashScreen;
}

/**
 * /brief  The SplashScreen class declaraation. Derived from QDialog class.
 */
class SplashScreen : public QDialog
{
    Q_OBJECT

public:
    /**
     * /brief  Constructor of the SplashScreen class
     * /param  parent  Parent of the SplashScreen object according to Qt's object tree
     */
    explicit SplashScreen(QWidget *parent = 0);

    /**
     * /brief  Destructor of the SplashScreen class
     */
    ~SplashScreen();

private:
    /**
     * /brief Internal used variables
     */
    Ui::SplashScreen* ui;
};

#endif /* SPLASHSCREEN_H */
