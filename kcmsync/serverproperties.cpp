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

#include "serverproperties.h"

#ifndef CONFIG_H
#define CONFIG_H

#include <QtGui/QDialog>

ServerProperties::ServerProperties(SyncServer *syncServer, QWidget *parent): KDialog(parent), m_syncServer(syncServer) {
    QWidget *widget = new QWidget(this);
    ui.setupUi(widget);
    setMainWidget(widget);
    
    setSyncUrl(syncServer->syncUrl());
    setUsername(syncServer->username());
    setPassword(syncServer->password());

    ui.cbRecurrance->setCurrentIndex(syncServer->autoSyncRecurrance());
    ui.groupBox->setChecked(syncServer->autoSyncEnabled());
    
    connect(ui.cbRecurrance, SIGNAL(currentIndexChanged(int)), SLOT(recurranceChanged()));
    connect(ui.groupBox, SIGNAL(clicked(bool)), SLOT(recurranceChanged()));
    connect(ui.leSyncUrl, SIGNAL(textChanged(QString)), SLOT(checkForComplete()));

    recurranceChanged();
    checkForComplete();
    
    
    for(int i = 0; i < 60; i++){
        ui.cbHourlyMinute->addItem(QString::number(i));
    }
    
    ui.cbHourlyMinute->setCurrentIndex(syncServer->autoSyncMinute());
    ui.teDailyTime->setTime(syncServer->autoSyncTime());
    ui.cbWeeklyDay->setCurrentIndex(qMin(syncServer->autoSyncDay() - 1, 6)); // Qt::DayOfWeek starts at 1
    ui.teWeeklyTime->setTime(syncServer->autoSyncTime());
    ui.cbMonthlyDay->setCurrentIndex(syncServer->autoSyncDay() - 1); // Days start at 1
    ui.teMonthlyTime->setTime(syncServer->autoSyncTime());
    
}

ServerProperties::~ServerProperties() {

}

void ServerProperties::setSyncUrl(const QString& syncUrl) {
    ui.leSyncUrl->setText(syncUrl);
}

void ServerProperties::setUsername(const QString& username) {
    ui.leUsername->setText(username);
}

void ServerProperties::setPassword(const QString& password) {
    ui.lePassword->setText(password);
}

QString ServerProperties::syncUrl() {
    return ui.leSyncUrl->text();
}

QString ServerProperties::username() {
    return ui.leUsername->text();
}

QString ServerProperties::password() {
    return ui.lePassword->text();
}

void ServerProperties::recurranceChanged() {
    if(ui.groupBox->isChecked()){
        switch(ui.cbRecurrance->currentIndex()){
            case SyncServer::Hourly:
                ui.swRecurrance->setCurrentIndex(0);
                break;
            case SyncServer::Daily:
                ui.swRecurrance->setCurrentIndex(1);
                break;
            case SyncServer::Weekly:
                ui.swRecurrance->setCurrentIndex(2);
                break;
            case SyncServer::Monthly:
                ui.swRecurrance->setCurrentIndex(3);
                break;
                
        }
    }    
}

void ServerProperties::slotButtonClicked(int button) {
    if(button == KDialog::Ok){
        m_syncServer->setSyncUrl(ui.leSyncUrl->text());
        m_syncServer->setUsername(ui.leUsername->text());
        m_syncServer->setPassword(ui.lePassword->text());
        m_syncServer->setAutoSyncEnabled(ui.groupBox->isChecked());
        m_syncServer->setAutoSyncRecurrance((SyncServer::SyncRecurrance)ui.cbRecurrance->currentIndex());
        switch(m_syncServer->autoSyncRecurrance()){
            case SyncServer::Hourly:
                m_syncServer->setAutoSyncMinute(ui.cbHourlyMinute->currentIndex());
                break;
            case SyncServer::Daily:
                m_syncServer->setAutoSyncTime(ui.teDailyTime->time());
                break;
            case SyncServer::Weekly:
                m_syncServer->setAutoSyncDay(ui.cbWeeklyDay->currentIndex() + 1); //Qt::DayOfWeek starts at 1
                m_syncServer->setAutoSyncTime(ui.teWeeklyTime->time());
                break;
            case SyncServer::Monthly:
                m_syncServer->setAutoSyncDay(ui.cbMonthlyDay->currentIndex() + 1); // days start at 1
                m_syncServer->setAutoSyncTime(ui.teMonthlyTime->time());
                break;
        }
    }
    KDialog::slotButtonClicked(button);
}

void ServerProperties::checkForComplete() {
    if(ui.leSyncUrl->text().isEmpty()){
        enableButtonOk(false);
        return;
    }
    enableButtonOk(true);
}

#endif
