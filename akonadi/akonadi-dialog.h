#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui/QDialog>

class KJob;

namespace Ui
{
    class Dialog;
}

namespace Akonadi {
    class Item;
    class Monitor;
}

class Dialog : public QDialog
{
    Q_OBJECT

  public:
      Dialog(QWidget *parent = 0);
      ~Dialog();

  private slots:
      void contactItemAdded(const Akonadi::Item & item);
      void fetchContactCollectionsDone(KJob* job);
      void wa();

  private:
      void initContacts();
      void fetchContactsCollections();
      Akonadi::Monitor *m_contactMonitor;
      Ui::Dialog *ui;
};

#endif // DIALOG_H
