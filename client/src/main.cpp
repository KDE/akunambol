#include "akunambol.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>

static const char description[] =
    I18N_NOOP("A KDE 4 Application");

static const char version[] = "0.1";

int main(int argc, char **argv)
{
    KAboutData about("akunambol", 0, ki18n("akunambol"), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2007 Riccardo Iaconelli"), KLocalizedString(), 0, "riccardo@kde.org");
    about.addAuthor( ki18n("Riccardo Iaconelli"), KLocalizedString(), "riccardo@kde.org" );
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
