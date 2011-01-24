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

class KDE_EXPORT SyncConfig2
{
    // enum loglevel
    public:
        SyncConfig2();
        
        void setUsername(QString user);
        void setPassword(QString password);
        void setSyncUrl(QString url);
        
        QString user() { return m_user; };
        QString password() { return m_password; };
        QString syncUrl() { return m_url; };
        
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
