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

#include <KDebug>
#include <KStandardDirs>

#include "base/posixlog.h"
#include "base/util/StringBuffer.h"

#include "qtgui/KDELog.h"

KDELog::KDELog() : logToFile(true),
                   logFileInitialized(false)
{
}

KDELog::~KDELog() {
}

void KDELog::enableFileLogging() {
    logToFile = true;
}

void KDELog::disableFileLogging() {
    logToFile = false;
}

void KDELog::setLogFile(const char * /* path */, const char* /* name */, bool /*redirectStderr*/) {
    // Do nothing here
}

void KDELog::printLine(bool firstLine, time_t time, const char *fullTime,
                       const char *shortTime, const char *utcTime, LogLevel level,
                       const char *levelPrefix, const char *line)
{
    if (logToFile) {
        // If file logging is enable we must first check if the log file was
        // initialized
        if (!logFileInitialized) {

            logFileInitialized = true;

            QString fName = KStandardDirs::locateLocal("appdata", "synclog.txt");
            const char* s = fName.toLatin1();
            StringBuffer buf(s);
            POSIXLog::setLogFile(NULL, buf.c_str(), false);
        }
        POSIXLog::printLine(firstLine, time, fullTime, shortTime, utcTime, level, levelPrefix, line);
    }

    if (level == LOG_LEVEL_NONE) {
        kError() << shortTime << levelPrefix << line << "\n";
    } else {
        kDebug() << shortTime << levelPrefix << line << "\n";
    }
}
