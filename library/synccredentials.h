/*
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SYNCCONFIG_H
#define SYNCCONFIG_H

#include <QString>
#include "akunambol_macros.h"

/**
 * @brief SyncCredentials is a class created to allow easy sharing of credential data between SyncSources
 * 
 * It is mostly a container class, which has three parameters: username, password, and syncUrl.
 * 
 * None of these fields is checked in any way, except for not being empty(), so especially syncUrl
 * can be used by SyncSources of the same kind the way they prefer.
 * 
 **/
class KDE_EXPORT SyncCredentials
{
    // enum loglevel
    public:
        SyncCredentials();
        
        void setUsername(const QString &user);
        void setPassword(const QString &password);
        void setSyncUrl(const QString &url);
        
        const QString user() { return m_user; };
        const QString password() { return m_password; };
        const QString syncUrl() { return m_url; };
        
        /**
         * Check if this object is "complete". Useful instead of checking if
         * password, username and url are not empty.
         * @return true if is complete.
         * @return false if it is not complete.
         */
        bool isComplete();
        
    private:
        QString m_user, m_password, m_url;
};

#endif // SYNCCONFIG_H
