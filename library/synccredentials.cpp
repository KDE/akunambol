/*
    Copyright (C) 2010-2011  Riccardo Iaconelli <riccardo@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "synccredentials.h"
#include <QString>

SyncCredentials::SyncCredentials()
{
}

void SyncCredentials::setUsername(const QString& user)
{
    m_user = user;
}

void SyncCredentials::setPassword(const QString& password)
{
    m_password = password;
}

void SyncCredentials::setSyncUrl(const QString& url)
{
    m_url = url;
}

bool SyncCredentials::isComplete()
{
    bool complete = !m_user.isEmpty() and !m_user.isNull() and
                    !m_password.isEmpty() and !m_password.isNull() and
                    !m_url.isEmpty() and !m_url.isNull();
    return complete;
}

