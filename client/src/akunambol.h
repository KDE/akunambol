/*
 * akunambol.h
 *
 * Copyright (C) 2008 Riccardo Iaconelli <riccardo@kde.org>
 */
#ifndef AKUNAMBOL_H
#define AKUNAMBOL_H


#include <kxmlguiwindow.h>

#include "ui_prefs_base.h"

class akunambolView;
class QPrinter;
class KToggleAction;
class KUrl;

/**
 * This class serves as the main window for akunambol.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Riccardo Iaconelli <riccardo@kde.org>
 * @version 0.1
 */
class akunambol : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    akunambol();

    /**
     * Default Destructor
     */
    virtual ~akunambol();

private slots:
    void fileNew();
    void optionsPreferences();

private:
    void setupActions();

private:
    Ui::prefs_base ui_prefs_base ;
    akunambolView *m_view;

    QPrinter   *m_printer;
    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
};

#endif // _AKUNAMBOL_H_
