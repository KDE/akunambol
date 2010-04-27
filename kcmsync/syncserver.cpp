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



#include "syncserver.h"

#include <QtGui/QPainter>

#include <KLocalizedString>
#include <KGlobalSettings>
#include <KConfigGroup>
#include <KDebug>

SyncServer::SyncServer() {
    m_wallet = KWallet::Wallet::openWallet(KWallet::Wallet::LocalWallet(), 0, KWallet::Wallet::Synchronous);
    if(m_wallet){
        if(!m_wallet->hasFolder("Akunambolsync")){
            if(m_wallet->createFolder("Akunambolsync")){
                m_wallet->setFolder("Akunambolsync");
            } else {
                delete m_wallet;
                m_wallet = 0;
            }
        }
    }
}

SyncServer::~SyncServer() {
    delete m_wallet;
}

void SyncServer::setSyncUrl(const QString &url) {
    m_syncUrl = url;
}

QString SyncServer::syncUrl() const {
    return m_syncUrl;
}

void SyncServer::setUsername(const QString &username) {
    m_username = username;
}

QString SyncServer::username() const {
    return m_username;
}

void SyncServer::setPassword(const QString &password) {
    m_password = password;
}

QString SyncServer::password() const {
    return m_password;
}

void SyncServer::setAutoSyncEnabled(bool enable) {
    m_autoSyncEnabled = enable;
}

bool SyncServer::autoSyncEnabled() {
    return m_autoSyncEnabled;
}

void SyncServer::setAutoSyncDay(int day) {
    m_autoSyncDay = day;
}

int SyncServer::autoSyncDay() {
    return m_autoSyncDay;
}

void SyncServer::setAutoSyncMinute(int minute) {
    m_autoSyncMinute = minute;
}

int SyncServer::autoSyncMinute() {
    return m_autoSyncMinute;
}

void SyncServer::setAutoSyncTime(const QTime& time) {
    m_autoSyncTime = time;
}

QTime SyncServer::autoSyncTime() {
    return m_autoSyncTime;
}

void SyncServer::syncing() {
    m_lastSyncState = Running;
}

void SyncServer::synced(bool successful) {
    m_lastSyncTime = QDateTime::currentDateTime();
    m_lastSyncState = successful ? Successful : Failed;
}

QDateTime SyncServer::lastSyncTime() {
    return m_lastSyncTime;
}

SyncServer::SyncState SyncServer::lastSyncState() {
    return m_lastSyncState;
}

QDateTime SyncServer::nextSyncTime() {
    QDateTime nextSync = QDateTime::currentDateTime();
    if(m_autoSyncEnabled){
        switch(m_autoSyncRecurrance){
            case Hourly:
                if(m_lastSyncTime.time().minute() < m_autoSyncMinute){
                    nextSync = m_lastSyncTime.addSecs((m_autoSyncMinute - m_lastSyncTime.time().minute()) * 60);
                } else {
                    nextSync = m_lastSyncTime.addSecs((60 - m_lastSyncTime.time().minute() + m_autoSyncMinute) * 60);
                }
                nextSync = nextSync.addSecs(-nextSync.time().second()); // Set to seconds to 0
                break;
            case Daily:
                if(m_lastSyncTime.time() > m_autoSyncTime){
                    nextSync = m_lastSyncTime.addDays(1);
                }
                nextSync.setTime(m_autoSyncTime);
                break;
            case Weekly:
                if(m_lastSyncTime.date().dayOfWeek() <= m_autoSyncDay && m_lastSyncTime.time() < m_autoSyncTime){
                    nextSync = m_lastSyncTime.addDays(m_autoSyncDay - m_lastSyncTime.date().dayOfWeek());
                } else {
                    nextSync = m_lastSyncTime.addDays(7 - m_lastSyncTime.date().dayOfWeek() + m_autoSyncDay);
                }
                nextSync.setTime(m_autoSyncTime);
                break;
            case Monthly:
                if(m_lastSyncTime.date().day() <= m_autoSyncDay && m_lastSyncTime.time() < m_autoSyncTime){
                    nextSync = m_lastSyncTime.addDays(m_autoSyncDay - m_lastSyncTime.date().day());
                } else {
                    nextSync = m_lastSyncTime.addDays(QDate::currentDate().daysInMonth() - m_lastSyncTime.date().day() + m_autoSyncDay);
                }
                nextSync.setTime(m_autoSyncTime);
                break;
        }
        if(nextSync < QDateTime::currentDateTime()){
            return QDateTime::currentDateTime();
        }
    }
    return nextSync;
}

void SyncServer::setAutoSyncRecurrance(SyncServer::SyncRecurrance recurrance) {
    m_autoSyncRecurrance = recurrance;
}

SyncServer::SyncRecurrance SyncServer::autoSyncRecurrance() {
    return m_autoSyncRecurrance;
}

void SyncServer::load(KConfigGroup config, const QString &syncUrl) {
    kDebug() << "loading config";
    KConfigGroup serverGroup(&config, syncUrl);
    setSyncUrl(syncUrl);
    setUsername(serverGroup.readEntry("username"));

    if(m_wallet){
        m_wallet->readPassword(syncUrl, m_password);
    } else {
        setPassword(serverGroup.readEntry("password"));
    }
    m_lastSyncTime = QDateTime::fromString(serverGroup.readEntry("LastSyncTime"));
    QString state = serverGroup.readEntry("LastSyncState");
    if(state == "Successful"){
        m_lastSyncState = Successful;
    } else if(state == "Failed"){
        m_lastSyncState = Failed;
    } else {
        m_lastSyncState = Never;
    }
    
    m_autoSyncEnabled = serverGroup.readEntry("AutoSyncEnabled", false);
    QString recurrance = serverGroup.readEntry("AutoSyncRecurrance", "Daily");
    if(recurrance == "Hourly"){
        m_autoSyncRecurrance = Hourly;
    } else if(recurrance == "Daily") {
        m_autoSyncRecurrance = Daily;
    } else if(recurrance == "Weekly") {
        m_autoSyncRecurrance = Weekly;
    } else if(recurrance == "Monthly") {
        m_autoSyncRecurrance = Monthly;
    }
    
    m_autoSyncMinute = serverGroup.readEntry("AutoSyncMinute", 0);
    m_autoSyncDay = serverGroup.readEntry("AutoSyncDay", 0);
    m_autoSyncTime = QTime::fromString(serverGroup.readEntry("AutoSyncTime"));
}

void SyncServer::save(KConfigGroup config) {
    KConfigGroup serverGroup(&config, m_syncUrl);
    serverGroup.writeEntry("username", m_username);
    if(m_wallet){
        m_wallet->writePassword(m_syncUrl, m_password);
    } else {
        serverGroup.writeEntry("password", m_password);
    }
    serverGroup.writeEntry("LastSyncTime", m_lastSyncTime.toString());
    switch(m_lastSyncState){
        case Successful:
            serverGroup.writeEntry("LastSyncState", "Successful");
            break;
        case Failed:
            serverGroup.writeEntry("LastSyncState", "Failed");
            break;
        default:
            serverGroup.writeEntry("LastSyncState", "Never");
    }
    
    serverGroup.writeEntry("AutoSyncEnabled", m_autoSyncEnabled);
    
    if(m_autoSyncEnabled){
        switch(m_autoSyncRecurrance){
            case Hourly:
                serverGroup.writeEntry("AutoSyncRecurrance", "Hourly");
                break;
            case Daily:
                serverGroup.writeEntry("AutoSyncRecurrance", "Daily");
                break;
            case Weekly:
                serverGroup.writeEntry("AutoSyncRecurrance", "Weekly");
                break;
            case Monthly:
                serverGroup.writeEntry("AutoSyncRecurrance", "Monthly");
                break;
        }

        serverGroup.writeEntry("AutoSyncMinute", m_autoSyncMinute);
        serverGroup.writeEntry("AutoSyncDay", m_autoSyncDay);
        serverGroup.writeEntry("AutoSyncTime", m_autoSyncTime.toString());
    }
}


void SyncServerDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter, option, index);
    
    SyncServer *syncServer = qVariantValue<SyncServer*>(index.data(Qt::UserRole));
   
    if(option.state & QStyle::State_Selected) {
        painter->setPen(option.palette.highlightedText().color());
    } else {
        painter->setPen(option.palette.text().color());
    }
    
    QFont font = painter->font();
    QFontMetrics fm(font);
    int spacing = option.rect.height() / 8;
    QRect paintRect = option.rect;
    paintRect.adjust(spacing, spacing, 0, 0);
    
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(paintRect, syncServer->syncUrl());
    
    font.setBold(false);
    QString lastSyncString(i18n("Last synced:"));
    paintRect.adjust(0, fm.height(), 0, 0);
    painter->setFont(font);
    painter->drawText(paintRect, lastSyncString);
    
    QString lastSyncTime;
    if(syncServer->lastSyncState() == SyncServer::Running){
        lastSyncTime = i18n("Running...");
    } else if(syncServer->lastSyncState() == SyncServer::Never){
        lastSyncTime = i18n("Never");
    } else {
        lastSyncTime = syncServer->lastSyncTime().toString();
    }
    paintRect.adjust(fm.width(lastSyncString + ' '), 0, 0, 0);
    painter->drawText(paintRect, lastSyncTime);
    
    QString lastSyncStateLabel(i18n("State:"));
    paintRect.adjust(fm.width(lastSyncTime + "   "), 0, 0, 0);
    painter->drawText(paintRect, lastSyncStateLabel);
    
    QString lastSyncState;
    switch(syncServer->lastSyncState()){
        case SyncServer::Successful:
            lastSyncState = i18nc("Last sync state", "Successful");
            break;
        case SyncServer::Failed:
            lastSyncState = i18nc("Last sync state", "Failed");
            break;
        default:
            lastSyncState = '-';
    }
        
    paintRect.adjust(fm.width(lastSyncStateLabel + ' '), 0, 0, 0);
    painter->drawText(paintRect, lastSyncState);
    
 
    // Reset paintRect for next line
    paintRect = option.rect;
    paintRect.adjust(spacing, spacing + fm.height() * 2, 0, 0);
    
    if(syncServer->autoSyncEnabled()){
        QString nextSyncTimeLabel(i18n("Next scheduled sync:"));
        painter->drawText(paintRect, nextSyncTimeLabel);
        
        QString nextSyncTime = syncServer->nextSyncTime().toString();
        paintRect.adjust(fm.width(nextSyncTimeLabel + ' '), 0, 0, 0);
        painter->drawText(paintRect, nextSyncTime);
    }

}

QSize SyncServerDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QStyledItemDelegate::sizeHint(option, index) * 4;
}
