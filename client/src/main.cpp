/***************************************************************************
 *   Copyright (C) 2008 by Dario Freddi <drf54321@gmail.com>               *
 *   Copyright (C) 2008 by Riccardo Iaconelli <riccardo@kde.org>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "akunambol.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>

static const char description[] =
    I18N_NOOP("A Connector to Akonadi for the Funambol Mobile Server");

static const char version[] = "0.1";

int main(int argc, char **argv)
{
    KAboutData about("akunambol", 0, ki18n("akunambol"), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2008 Riccardo Iaconelli, Dario Freddi"),
                     KLocalizedString(), 0, "riccardo@kde.org");
    about.addAuthor( ki18n("Riccardo Iaconelli"), KLocalizedString(), "riccardo@kde.org" );
    about.addAuthor( ki18n("Dario Freddi"), KLocalizedString(), "riccardo@kde.org" );
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[URL]", ki18n( "Document to open" ));
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    akunambol *widget = new akunambol;

    // see if we are starting with session management
    if (app.isSessionRestored())
    {
        RESTORE(akunambol);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0)
        {
            //akunambol *widget = new akunambol;
            widget->show();
        }
        else
        {
            int i = 0;
            for (; i < args->count(); i++)
            {
                //akunambol *widget = new akunambol;
                widget->show();
            }
        }
        args->clear();
    }

    return app.exec();
}
