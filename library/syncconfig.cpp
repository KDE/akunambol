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

SyncConfig2::SyncConfig2()
{
    m_url = m_password = m_user = QString();
}

void SyncConfig2::setUsername(QString user)
{
    m_user = user;
}

void SyncConfig2::setPassword(QString password)
{
    m_password = password;
}

void SyncConfig2::setSyncUrl(QString url)
{
    m_url = url;
}

bool SyncConfig2::isComplete()
{
    bool complete = !m_user.isEmpty() and
                    !m_password.isEmpty() and
                    m_url.isEmpty();
    return complete;
}

