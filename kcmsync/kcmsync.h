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


#ifndef KCMSYNC_H
#define KCMSYNC_H

#include "ui_kcmsync.h"
#include "syncserver.h"

#include <kcmodule.h>

class KCMSync: public KCModule
{
    Q_OBJECT

private:
    Ui::KCMSync ui;
    
public:
    virtual void load();
    virtual void save();
    explicit KCMSync(QWidget *parent = 0, const QVariantList &args = QVariantList());
    ~KCMSync();
    
private slots:
    void updateButtons();
    void addServer();
    void removeServer();
    void editServer();
    void syncNow();
    void syncStarted(SyncServer *syncServer);
    void syncFinished(SyncServer *syncServer);
    void autosyncStarted(const QString &syncUrl);
    void autosyncCompleted(const QString &syncUrl, bool success);

};

#endif
