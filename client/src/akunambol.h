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

#ifndef AKUNAMBOL_H
#define AKUNAMBOL_H


#include <kmainwindow.h>

#include "ui_akunambolMainWindow.h"

/**
 * This class serves as the main window for akunambol.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Riccardo Iaconelli <riccardo@kde.org>
 * @version 0.1
 */
class AkunambolMainWindow : public KMainWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    AkunambolMainWindow();

    /**
     * Default Destructor
     */
    virtual ~AkunambolMainWindow();

private slots:
    void optionsPreferences();

private:
    void setupActions();

private:
    Ui::akunambolMainWindow ui;
    QWidget * m_widget;
};

#endif // _AKUNAMBOL_H_
