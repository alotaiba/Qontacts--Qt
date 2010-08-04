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

#ifndef CONTACTSMODEL_H
#define CONTACTSMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QAction>
#include "contactdata.h"

class ContactsModel : public QObject
{
    Q_OBJECT
public:
    explicit ContactsModel(QObject *parent = 0);
    //ContactsModel();
    ~ContactsModel();
    QList<ContactData>* getUpdateableContacts();
    QList<QContactPhoneNumber> getCurrentNumbers(const QContact &contact);
    QList<QContactPhoneNumber> getUpdatedNumbers(const QList<QContactPhoneNumber> &currentNumbers);
    static QString getStringLabelFromSubTypes(const QStringList &subTypes);
    static bool updateContactNumbers(QContactManager *manager, QContactLocalId contactLocalId);
    void initAvailableManagers();

public:
    QList<ContactData>* iContacts;
    QMap<QString, QString>* iAvailableManagers;

private:
    QAction *aboutAction;

private:
    QString updateableNumber(const QString &number);

};

#endif // CONTACTSMODEL_H
