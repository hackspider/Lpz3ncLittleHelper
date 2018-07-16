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
 * /file    main.cpp
 * /author  Hackspider
 * /brief   Source file of the main application entry
 */

/* Necessary dependencies to Qt framework */
#include <QApplication>

/* Necessary internal dependencies */
#include "DarkStyle.h"
#include "framelesswindow.h"

#include "splashscreen.h"
#include "mainwindowcontent.h"

/**
 * /brief  Application main entry
 */
int main(int argc, char *argv[])
{
    /* Create a new QApplication */
    QApplication application(argc, argv);

    /* Set the dark style for the whole application */
    application.setStyle(new DarkStyle);

    /* Before the application is started show a splash screen */
    SplashScreen splashScreen;
    splashScreen.exec();

    /* Create a frameless window of the dark style */
    FramelessWindow mainWindow;

    /* Set the window title */
    mainWindow.setWindowTitle("Lpz3nc's Little Helper");

    /* Create the content widget (main logic happens here) */
    MainWindowContent* mainWindowContent = new MainWindowContent;

    /* Set the created content widget as content of the frameless window */
    mainWindow.setContent(mainWindowContent);

    /* Show the main window */
    mainWindow.show();

    /* Start the application */
    return application.exec();
}
