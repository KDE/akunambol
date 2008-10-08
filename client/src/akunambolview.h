/***************************************************************************
 *   Copyright (C) 2008 by Dario Freddi <drf54321@gmail.com>               *
 *   Copyright (C) 2008 by Riccardo Iaconelli <riccardo@kde.org>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#ifndef AKUNAMBOLVIEW_H
#define AKUNAMBOLVIEW_H

#include <QtGui/QWidget>

#include "ui_akunambolview_base.h"

class QPainter;
class KUrl;

/**
 * This is the main view class for akunambol.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author Riccardo Iaconelli <riccardo@kde.org>
 * @version 0.1
 */

class akunambolView : public QWidget, public Ui::akunambolview_base
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    akunambolView(QWidget *parent);

    /**
     * Destructor
     */
    virtual ~akunambolView();

private:
    Ui::akunambolview_base ui_akunambolview_base;

signals:
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString& text);

    /**
     * Use this signal to change the content of the caption
     */
    void signalChangeCaption(const QString& text);

private slots:
    void switchColors();
    void settingsChanged();
};

#endif // akunambolVIEW_H