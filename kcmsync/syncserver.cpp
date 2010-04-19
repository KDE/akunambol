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

SyncServer::SyncServer() {
}

SyncServer::~SyncServer() {
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

void SyncServer::load(KConfigGroup config, const QString &syncUrl) {
    KConfigGroup serverGroup(&config, syncUrl);
    setSyncUrl(syncUrl);
    setUsername(serverGroup.readEntry("username"));
    setPassword(serverGroup.readEntry("password"));
    m_lastSyncTime = QDateTime::fromString(serverGroup.readEntry("LastSyncTime"));
    QString state = serverGroup.readEntry("LastSyncState");
    if(state == "Successful"){
        m_lastSyncState = Successful;
    } else if(state == "Failed"){
        m_lastSyncState = Failed;
    } else {
        m_lastSyncState = Never;
    }

}

void SyncServer::save(KConfigGroup config) {
    KConfigGroup serverGroup(&config, m_syncUrl);
    serverGroup.writeEntry("username", m_username);
    serverGroup.writeEntry("password", m_password);
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
    int spacing = option.rect.height() / 6;
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
    
}

QSize SyncServerDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
         return QStyledItemDelegate::sizeHint(option, index) * 3;
}