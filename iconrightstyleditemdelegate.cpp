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
 * /file    iconrightstyleditemdelegate.cpp
 * /author  Hackspider
 * /brief   Source file of the styled item delegate to show the icon on the right side of item
 */

/* Necessary internal dependencies */
#include "iconrightstyleditemdelegate.h"

/**
 * /brief  Definition of IconRightStyledItemDelegate constructor
 */
IconRightStyledItemDelegate::IconRightStyledItemDelegate(QObject* parent)
{
    /* Nothing to do here */
}

/**
 * /brief  Definition of IconRightStyledItemDelegate destructor
 */
IconRightStyledItemDelegate::~IconRightStyledItemDelegate()
{
    /* Nothing to do here */
}

/**
 * /brief  Definition of the overwritten paint method
 */
void IconRightStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    /* Create a new copy of the passed options */
    QStyleOptionViewItem iconRightSideOption(option);

    /* Set decoration position to the right side */
    iconRightSideOption.decorationPosition = QStyleOptionViewItem::Right;

    /* Call the method of the parent class and pass the new option */
    QStyledItemDelegate::paint(painter, iconRightSideOption, index);
}
