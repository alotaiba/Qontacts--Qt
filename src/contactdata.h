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

#ifndef CONTACTDATA_H
#define CONTACTDATA_H

#include <QList>
#include <QString>
#include <QContact>
#include <QContactPhoneNumber>

using namespace QtMobility;

class ContactData
{
public:
    //explicit ContactData(QObject *parent = 0);

public:
    QContactLocalId id;
    QString iName;
    QString iNumber;
    QString iManagerUri;
    QList<QContactPhoneNumber> iCurrentNumbers;
    QList<QContactPhoneNumber> iUpdatedNumbers;
    bool isChecked;

};

#endif // CONTACTDATA_H
