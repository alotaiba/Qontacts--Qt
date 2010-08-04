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

#include "contactsmodel.h"
#include <QStandardItem>
#include <QMenuBar>
#include <QList>
#include <QString>
#include <QStringList>
#include <QContactManager>
#include <QContactName>
#include <QDebug>
#include <QTextCodec>

ContactsModel::ContactsModel(QObject *parent) :
    QObject(parent)
{
    iContacts = new QList<ContactData>();
    iAvailableManagers = new QMap<QString, QString>();
}

ContactsModel::~ContactsModel()
{
    //delete iContacts;
    //delete iAvailableManagers;
}

void ContactsModel::initAvailableManagers()
{
    QStringList availableManagers = QContactManager::availableManagers();
    availableManagers.removeAll("invalid");
    foreach (QString managerName, availableManagers) {
        QMap<QString, QString> params = QMap<QString, QString>();
        QString managerUri = QContactManager::buildUri(managerName, params);
        QContactManager *manager = QContactManager::fromUri(managerUri);
        if (manager) {
            iAvailableManagers->insert(managerName, managerUri);
        }
    }
}

QList<ContactData>* ContactsModel::getUpdateableContacts()
{
    initAvailableManagers();
    QMapIterator<QString, QString> i(*iAvailableManagers);
    while (i.hasNext()) {
        i.next();
        QContactManager *manager = QContactManager::fromUri(i.value());
        QList<QContactLocalId> contacts = manager->contactIds();
        for(int contactIdx = 0; contactIdx < contacts.count(); contactIdx++) {
            QContact contact = manager->contact(contacts.at(contactIdx));
            ContactData contactData;
            contactData.id = contact.localId();
            contactData.iName = contact.displayLabel();
            contactData.iManagerUri = i.value();
            contactData.iCurrentNumbers = getCurrentNumbers(contact);
            contactData.iUpdatedNumbers = getUpdatedNumbers(contactData.iCurrentNumbers);
            contactData.iNumber = QString::number(contactData.iUpdatedNumbers.count());
            contactData.isChecked = true;
            if (contactData.iUpdatedNumbers.count() > 0)
                iContacts->append(contactData);
        }
    }
    return iContacts;
}

QList<QContactPhoneNumber> ContactsModel::getCurrentNumbers(const QContact& contact)
{
    QList<QContactPhoneNumber> currentNumbers = contact.details<QContactPhoneNumber>();
    /*
    foreach (const QContactPhoneNumber& currentNumber, currentNumbers) {
        //qDebug() << "Current number key : " << currentNumber.key();
    }
    */
    return currentNumbers;
}

QList<QContactPhoneNumber> ContactsModel::getUpdatedNumbers(const QList<QContactPhoneNumber>& currentNumbers)
{
    QList<QContactPhoneNumber> updatedNumbers;

    foreach (const QContactPhoneNumber& currentNumber, currentNumbers) {
        QString updatedNumberString = updateableNumber(currentNumber.number());
        if (!updatedNumberString.isEmpty()) {
            QContactPhoneNumber updatedNumber = currentNumber;
            updatedNumber.setNumber(updatedNumberString);
            //qDebug() << "Updated number key : " << updatedNumber.key();
            updatedNumbers.append(updatedNumber);
        }
    }

    return updatedNumbers;
}

QString ContactsModel::updateableNumber(const QString &number)
{
    QString retNumber = "";

    if ( (!number.isEmpty()) && (number.length() > 0) ) {
        //TODO: Clean numbers from spaces and hyphens!!!!!!
        QString tempNumber       = "";
        QString tempNumberPrefix = "";

        if (number.startsWith("+974")) {
            tempNumberPrefix = "+974";
            tempNumber = number.mid(4);
        } else if (number.startsWith("00974")) {
            tempNumberPrefix = "00974";
            tempNumber = number.mid(5);
        } else {
            tempNumber = number;
        }

        if ( (tempNumber.length() == 7) && (
                    (tempNumber.startsWith("3")) ||
                    (tempNumber.startsWith("4")) ||
                    (tempNumber.startsWith("5")) ||
                    (tempNumber.startsWith("6")) ||
                    (tempNumber.startsWith("7"))
                ))
        {
            retNumber = QString("%1%2%3").arg(tempNumberPrefix, tempNumber.left(1),tempNumber);
        }
    }

    return retNumber;
}

QString ContactsModel::getStringLabelFromSubTypes(const QStringList &subTypes)
{
    QString numberLabel = "Unknown";

    //no subtype
    if(!subTypes.count())
    {
        return numberLabel;
    }

    //Take the first value
    QString subType = subTypes.at(0);

    if (subType == QContactPhoneNumber::SubTypeLandline)
    {
        numberLabel = "Landline";
    }
    else if (subType == QContactPhoneNumber::SubTypeMobile)
    {
        numberLabel = "Mobile";
    }
    else if (subType == QContactPhoneNumber::SubTypeFax)
    {
        numberLabel = "Fax";
    }
    else if (subType == QContactPhoneNumber::SubTypePager)
    {
        numberLabel = "Pager";
    }
    else if (subType == QContactPhoneNumber::SubTypeVoice)
    {
        numberLabel = "Voice";
    }
    else if (subType == QContactPhoneNumber::SubTypeModem)
    {
        numberLabel = "Modem";
    }
    else if (subType == QContactPhoneNumber::SubTypeVideo)
    {
        numberLabel = "Video";
    }
    else if (subType == QContactPhoneNumber::SubTypeCar)
    {
        numberLabel = "Car";
    }
    else if (subType == QContactPhoneNumber::SubTypeBulletinBoardSystem)
    {
        numberLabel = "BulletinBoardSystem";
    }
    else if (subType == QContactPhoneNumber::SubTypeMessagingCapable)
    {
        numberLabel = "MessagingCapable";
    }
    else if (subType == QContactPhoneNumber::SubTypeAssistant)
    {
        numberLabel = "Assistant";
    }
    else if (subType == QContactPhoneNumber::SubTypeDtmfMenu)
    {
        numberLabel = "DtmfMenu";
    }
    else
    {
        numberLabel = "Unknown";
    }

    return numberLabel;
}

bool ContactsModel::updateContactNumbers(QContactManager *manager, QContactLocalId contactLocalId)
{
    //This is a static function, so I'll have to reference ContactsModel here
    ContactsModel mContactModel;
    QContact currentContact;
    if (contactLocalId != QContactLocalId(0)) {
        currentContact = manager->contact(contactLocalId);
    } else {
        return false;
    }

    QMap<int, QContactPhoneNumber> updatedNumbersMap;

    QList<QContactPhoneNumber> currentNumbers = mContactModel.getCurrentNumbers(currentContact);
    QList<QContactPhoneNumber> updatedNumbers = mContactModel.getUpdatedNumbers(currentNumbers);

    foreach (const QContactPhoneNumber& updatedNumber, updatedNumbers) {
        updatedNumbersMap.insert(updatedNumber.key(), updatedNumber);
    }

    foreach (const QContactPhoneNumber& currentNumber, currentNumbers) {
        if ( updatedNumbersMap.contains(currentNumber.key()) ) {
            QContactPhoneNumber newNumber = updatedNumbersMap.value(currentNumber.key());
            QContactPhoneNumber curNumber = currentNumber;
            curNumber.setNumber(newNumber.number());
            if (!currentContact.saveDetail(&curNumber)) {
                return false;
            }
        }
    }

    QContactName contactName = currentContact.detail<QContactName>();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    if ( !contactName.prefix().isEmpty() ) {
        QByteArray contactPrefixLatin1 = contactName.prefix().toLatin1();
        QString contactPrefix = codec->toUnicode(contactPrefixLatin1);
        contactName.setPrefix(contactPrefix);
    }

    if ( !contactName.firstName().isEmpty() ) {
        QByteArray contactFirstNameLatin1 = contactName.firstName().toLatin1();
        QString contactFirstName = codec->toUnicode(contactFirstNameLatin1);
        contactName.setFirstName(contactFirstName);
    }

    if ( !contactName.middleName().isEmpty() ) {
        QByteArray contactMiddleNameLatin1 = contactName.middleName().toLatin1();
        QString contactMiddleName = codec->toUnicode(contactMiddleNameLatin1);
        contactName.setMiddleName(contactMiddleName);
    }

    if ( !contactName.lastName().isEmpty() ) {
        QByteArray contactLastNameLatin1 = contactName.lastName().toLatin1();
        QString contactLastName = codec->toUnicode(contactLastNameLatin1);
        contactName.setLastName(contactLastName);
    }

    if ( !contactName.suffix().isEmpty() ) {
        QByteArray contactSuffixLatin1 = contactName.suffix().toLatin1();
        QString contactSuffix = codec->toUnicode(contactSuffixLatin1);
        contactName.setSuffix(contactSuffix);
    }

    if ( !contactName.customLabel().isEmpty() ) {
        QByteArray contactCustomLabelLatin1 = contactName.customLabel().toLatin1();
        QString contactCustomLabel = codec->toUnicode(contactCustomLabelLatin1);
        contactName.setCustomLabel(contactCustomLabel);
    }

    currentContact.saveDetail(&contactName);

    manager->synthesizeContactDisplayLabel(&currentContact);

    currentContact = manager->compatibleContact(currentContact);

    if (manager->saveContact(&currentContact)) {
        return true;
    } else {
        return false;
    }
}
