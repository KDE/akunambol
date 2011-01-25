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

#include "syncconfig.h"
#include <QString>

SyncCredentials::SyncCredentials()
{
    m_url = m_password = m_user = QString();
}

void SyncCredentials::setUsername(QString user)
{
    m_user = user;
}

void SyncCredentials::setPassword(QString password)
{
    m_password = password;
}

void SyncCredentials::setSyncUrl(QString url)
{
    m_url = url;
}

bool SyncCredentials::isComplete()
{
    bool complete = !m_user.isEmpty() and
                    !m_password.isEmpty() and
                    m_url.isEmpty();
    return complete;
}

