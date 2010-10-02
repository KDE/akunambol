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
