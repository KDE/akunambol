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

#include<QList>
#include<QWidget>
#include<QTabWidget>

#include<base/Log.h>

#include "settings.h"
#include "ui_settings.h"

#include<client/appsyncsourcemanager.h>
#include<client/appsyncsource.h>
#include<qtgui/standardsourcesettings.h>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Settings)
{
    m_ui->setupUi(this);

    // Add a tab for each existing source
    QTabWidget *tabs = m_ui->tabs;
    AppSyncSourceManager *manager = AppSyncSourceManager::getInstance();
    QList<AppSyncSource*> sources = manager->getRegisteredSources();
    foreach (AppSyncSource* source, sources) {
        LOG.debug("Creating a tab for source: %s", source->getName());

        QWidget* sourceTab = source->getSettingsTab();
        if (sourceTab == NULL) {
            LOG.debug("Source %s has no settings");
        } else {
            tabs->addTab(sourceTab, source->getName());
            StandardSourceSettings* settings = static_cast<StandardSourceSettings*>(sourceTab);
            if (settings != NULL) {
                settings->load();
            }
        }
    }
}

Settings::~Settings()
{
    delete m_ui;
}

void Settings::setUser(const QString &text)
{
    m_ui->user->setText(text);
}

void Settings::setPassword(const QString &text)
{
    m_ui->password->setText(text);
}

void Settings::setSyncUrl(const QString& text)
{
    m_ui->syncUrl->setText(text);
}

QString Settings::user()
{
    return m_ui->user->text();
}

QString Settings::password()
{
    return m_ui->password->text();
}

QString Settings::syncUrl()
{
    return m_ui->syncUrl->text();
}

void Settings::accept()
{
    QDialog::accept();
    // Now save all the tabs
    QTabWidget *tabs = m_ui->tabs;
    int count = tabs->count();
    for(int i=0;i<count;++i) {
        StandardSourceSettings* settings = dynamic_cast<StandardSourceSettings*>(tabs->widget(i));
        if (settings != NULL) {
            settings->save();
        }
    }
}

void Settings::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
