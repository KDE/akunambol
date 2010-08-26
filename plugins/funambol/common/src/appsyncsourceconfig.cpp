/*
 * Funambol is a mobile platform developed by Funambol, Inc.
 * Copyright (C) 2010 Funambol, Inc.
 * Copyright (C) 2010 Riccardo Iaconelli <riccardo@kde.org>
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
 *
 * You can contact Funambol, Inc. headquarters at 643 Bair Island Road, Suite
 * 305, Redwood City, CA 94063, USA, or at email address info@funambol.com.
 *
 * The interactive user interfaces in modified source and object code versions
 * of this program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU Affero General Public License version 3.
 *
 * In accordance with Section 7(b) of the GNU Affero General Public License
 * version 3, these Appropriate Legal Notices must retain the display of the
 * "Powered by Funambol" logo. If the display of the logo is not reasonably
 * feasible for technical reasons, the Appropriate Legal Notices must display
 * the words "Powered by Funambol".
 */

#include<QWidget>

#include <base/util/StringBuffer.h>
#include <base/Log.h>
#include <base/adapter/PlatformAdapter.h>
#include <spdm/DeviceManagementNode.h>

#include "akonadisource.h"
#include "appsyncsourceconfig.h"

using namespace Funambol;

AppSyncSourceConfig::AppSyncSourceConfig(const char* srcName) : 
    remoteUri(""),
    collectionId(-1),
    sourceName(srcName)
{
}

void AppSyncSourceConfig::setCollectionId(qint64 id)
{
    collectionId = id;
}

qint64 AppSyncSourceConfig::getCollectionId()
{
    return collectionId;
}

void AppSyncSourceConfig::setRemoteUri(const char* uri)
{
    remoteUri.assign(uri);
}

const char* AppSyncSourceConfig::getRemoteUri() const {
    return remoteUri.c_str();
}

bool AppSyncSourceConfig::save() {
    DeviceManagementNode* node = new DeviceManagementNode("Funambol/Akunambol/appsourceconfig", sourceName.c_str());
    StringBuffer id;
    id.append(collectionId);
    node->setPropertyValue("collection-id", id.c_str());
    node->setPropertyValue("remote-uri", remoteUri.c_str());
    delete node;

    return true;
}

bool AppSyncSourceConfig::load() {
    DeviceManagementNode* node = new DeviceManagementNode("Funambol/Akunambol/appsourceconfig", sourceName.c_str());
    const char* remoteUriStr = node->readPropertyValue("remote-uri");
    if (remoteUriStr != NULL) {
        remoteUri.assign(remoteUriStr);
    }
    const char* idStr = node->readPropertyValue("collection-id");
    collectionId = (qint64) strtol(idStr, NULL, 0);

    return true;
}

