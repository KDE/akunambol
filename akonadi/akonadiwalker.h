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


#ifndef AKONADIWALKER_H
#define AKONADIWALKER_H

#include <QObject>

#include <Akonadi/Item>
#include <Akonadi/Monitor>

#include <kmime/kmime_message.h>
#include <kabc/addressee.h>

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<KMime::Message> MessagePtr;

class KJob;

class AkonadiWalker : public QObject
{
        Q_OBJECT

    public:
        AkonadiWalker( QObject* parent, const QVariantList& args );
        ~AkonadiWalker();
        QStringList sources() const;

    protected:
        bool sourceRequestEvent(const QString &name);

    private slots:

        void stopMonitor(const QString &name);

        void fetchEmailCollectionDone(KJob* job); // done retrieving whole collection
        void fetchContactCollectionDone(KJob* job); // done retrieving a whole contact collection
        void fetchMicroBlogDone(KJob* job);

        void emailItemsReceived(const Akonadi::Item::List &items);

        void fetchEmailCollectionsDone(KJob* job); // got list of collections
        void fetchContactCollectionsDone(KJob* job);
        void fetchMicroBlogCollectionsDone(KJob* job);

        void emailItemAdded(const Akonadi::Item &item, const QString &collection = QString());
        void contactItemAdded(const Akonadi::Item & item);
        void microBlogItemAdded(const Akonadi::Item &item);

    private:
        void initEmailMonitor();
        void initContactMonitor();
        void initMicroBlogMonitor();
        // useful for debugging
        void printMessage(MessagePtr msg);
        void printContact(const QString &source, const KABC::Addressee &a);

        Akonadi::Monitor* m_emailMonitor;
        Akonadi::Monitor* m_contactMonitor;
        Akonadi::Monitor* m_microBlogMonitor;

        QHash<KJob*, QString> m_jobCollections;
};

#endif // AKONADIWALKER_H
