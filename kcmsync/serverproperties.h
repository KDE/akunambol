/*****************************************************************************
 *  Copyright (C) 2010  Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 *  This program is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *****************************************************************************/

#ifndef SERVERPROPERTIES_H
#define SERVERPROPERTIES_H

#include "ui_serverproperties.h"
#include "syncserver.h"

#include <KDialog>


class ServerProperties : public KDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(ServerProperties)
public:
    explicit ServerProperties(SyncServer *server, QWidget *parent = 0);
    virtual ~ServerProperties();

private:
    Ui::ServerProperties ui;
    SyncServer *m_syncServer;
    
    void setSyncUrl(const QString &syncUrl);
    void setUsername(const QString &username);
    void setPassword(const QString &password);
    
    QString syncUrl();
    QString username();
    QString password();

private slots:
    void recurranceChanged();
    void slotButtonClicked(int button);
    void checkForComplete();
//    void autoSyncChanged();
};

#endif // SERVERPROPERTIES_H
