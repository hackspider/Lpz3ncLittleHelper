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
 * /file    iconrightstyleditemdelegate.h
 * /author  Hackspider
 * /brief   Header file of the styled item delegate to show the icon on the right side of item
 */

/* Include Guard */
#ifndef ICONRIGHTSTYLEDITEMDELEGATE_H
#define ICONRIGHTSTYLEDITEMDELEGATE_H

/* Necessary dependencies to Qt framework */
#include <QStyledItemDelegate>

/**
 * /brief  The IconRightStyledItemDelegate class declaraation. Derived from QStyledItemDelegate class.
 */
class IconRightStyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    /**
     * /brief  Constructor of the IconRightStyledItemDelegate class
     * /param  parent  Parent of the IconRightStyledItemDelegate object according to Qt's object tree
     */
    IconRightStyledItemDelegate(QObject* parent);

    /**
     * /brief  Destructor of the IconRightStyledItemDelegate class
     */
    virtual ~IconRightStyledItemDelegate();

    /**
     * /brief Overwritten function of the parent QStyledItemDelegate to show the icon on the right side
     */
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif /* ICONRIGHTSTYLEDITEMDELEGATE_H */
