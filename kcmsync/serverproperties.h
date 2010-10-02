/*
 * Copyright (C) 2010 Michael Zanetti <michael_zanetti@gmx.net>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License version 3 as published by
 * the Free Software Foundation with the addition of the following permission
 * added to Section 15 as permitted in Section 7(a): FOR ANY PART OF THE COVERED
 * WORK IN WHICH THE COPYRIGHT IS OWNED BY FUNAMBOL, FUNAMBOL DISCLAIMS THE
 * WARRANTY OF NON INFRINGEMENT  OF THIRD PARTY RIGHTS.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, see http://www.gnu.org/licenses or write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA.
 */

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
