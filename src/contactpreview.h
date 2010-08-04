/**
 * Qontacts Mobile Application
 * Qontacts is a mobile application that updates the address book contacts
 * to the new Qatari numbering scheme.
 *
 * Copyright (C) 2010  Abdulrahman Saleh Alotaiba
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONTACTPREVIEW_H
#define CONTACTPREVIEW_H

#include <QMainWindow>
#include "contactsmodel.h"
#include <QContactManager>
#include <QProgressDialog>

namespace Ui {
    class ContactPreview;
}

class ContactPreview : public QMainWindow
{
    Q_OBJECT

public:
    explicit ContactPreview(QWidget *parent, QList<ContactData> *contacts, QModelIndex *contactItem);
    ~ContactPreview();

signals:
    void showContactsList();


private:
    Ui::ContactPreview *ui;
    QList<ContactData> *iContacts;
    QModelIndex *iContactItem;
    QContactLocalId iContactId;
    QString iContactName;
    QContactManager *iContactManager;
    QProgressDialog *iProgressDialog;
    QAction *aboutAction;

private slots:
    void updateContact();
    void showAbout();
};

#endif // CONTACTPREVIEW_H
