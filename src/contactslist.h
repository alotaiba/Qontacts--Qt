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

#ifndef CONTACTSLIST_H
#define CONTACTSLIST_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QMap>
#include <QProgressDialog>
#include "contactsmodel.h"


namespace Ui {
    class ContactsList;
}

class ContactPreview;

class ContactsList : public QMainWindow
{
    Q_OBJECT

public:
    explicit ContactsList(QWidget *parent = 0);
    ~ContactsList();
    QMap<int, ContactData> getSelectedContacts();

public:
    QList<ContactData>* iContacts;

private:
    Ui::ContactsList *ui;
    ContactPreview *iContactPreview;
    ContactsModel *iContactsModel;
    QStandardItemModel *iContactsDataModel;
    QItemSelectionModel *iContactsSelectionModel;
    QProgressDialog *iProgressDialog;
    QAction *aboutAction;

private:
    void setUpdateSelectButtonsState();

private slots:
    void updateList();
    void showPreview();
    void setCheckedItem( QStandardItem *item );
    void enablePreview(QModelIndex current, QModelIndex previous);
    void updateSelectedContacts();
    void setContactsListSelection();
    void showAbout();
};

#endif // CONTACTSLIST_H
