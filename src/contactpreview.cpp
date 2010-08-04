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

#include "contactpreview.h"
#include "ui_contactpreview.h"
#include "contactsmodel.h"
#include "about.h"
#include <QMenuBar>
#include <QDebug>
#ifdef Q_WS_MAEMO_5
#include <QMaemo5InformationBox>
#endif

ContactPreview::ContactPreview(QWidget *parent, QList<ContactData> *contacts, QModelIndex *contactItem) :
    QMainWindow(parent),
    ui(new Ui::ContactPreview),
    iContacts(contacts),
    iContactItem(contactItem)
{
    ui->setupUi(this);

    setWindowTitle("Preview Contact");

    //Create actions
    aboutAction = new QAction(tr("About"), this);

    menuBar()->clear();
    menuBar()->addAction(aboutAction);

    iProgressDialog = new QProgressDialog(this);

    ContactData contact = iContacts->at(iContactItem->row());

    iContactId = contact.id;
    iContactManager = QContactManager::fromUri(contact.iManagerUri);
    iContactName = contact.iName;

    ui->previewLabel->setText(contact.iName);

    ui->currentNumbersGroup->setTitle(QString("Current Numbers (%1)").arg(contact.iCurrentNumbers.count()));
    foreach (const QContactPhoneNumber& currentNumber, contact.iCurrentNumbers) {
        QString numberLabel = ContactsModel::getStringLabelFromSubTypes(currentNumber.subTypes());
        QString labelText = QString("%1: %2").arg(numberLabel, currentNumber.number());
        QLabel *number = new QLabel(labelText);
        ui->currentNumbersLayout->addWidget(number);
    }

    ui->updatedNumbersGroup->setTitle(QString("Updated Numbers (%1)").arg(contact.iUpdatedNumbers.count()));
    foreach (const QContactPhoneNumber& updatedNumber, contact.iUpdatedNumbers) {
        QString numberLabel = ContactsModel::getStringLabelFromSubTypes(updatedNumber.subTypes());
        QString labelText = QString("%1: %2").arg(numberLabel, updatedNumber.number());
        QLabel *number = new QLabel(labelText);
        ui->updatedNumbersLayout->addWidget(number);
    }

    connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(updateContact()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
}

ContactPreview::~ContactPreview()
{
    delete ui;
}

void ContactPreview::updateContact()
{
    QString qUpdateQuestion = QString("Are you sure you want to update %1's numbers?").arg(iContactName);
    int qUpdateAnswer = QMessageBox::question(this, "Confirm Update", qUpdateQuestion, QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);
    if( qUpdateAnswer == QMessageBox::Yes ) {
        QString progressLabelText = QString("Updating %1's phone numbers...").arg(iContactName);
        iProgressDialog->setModal(true);
        iProgressDialog->setWindowTitle("Updating Phone Numbers");
        iProgressDialog->setLabelText(progressLabelText);
        iProgressDialog->setMaximum(0);
        iProgressDialog->setMinimum(0);
        iProgressDialog->setValue(0);
        iProgressDialog->setCancelButtonText(QString());
        iProgressDialog->show();
        if ( !ContactsModel::updateContactNumbers(iContactManager, iContactId) ) {
            iProgressDialog->hide();
            #ifdef Q_WS_MAEMO_5
            QMaemo5InformationBox::information(this, "Operation Failed!", QMaemo5InformationBox::DefaultTimeout);
            #else
            QMessageBox::critical(this, "Operation Failed", "Updating contact failed!");
            #endif
        } else {
            iProgressDialog->hide();
            iContacts->removeAt(iContactItem->row());
            QString updateMsg = QString("You have updated %1's numbers sucessfully").arg(iContactName);
            #ifdef Q_WS_MAEMO_5
            QMaemo5InformationBox::information(this, updateMsg, QMaemo5InformationBox::DefaultTimeout);
            #else
            QMessageBox::information(this, "Contacts Updated", updateMsg);
            #endif
            emit showContactsList();
            close();
        }
    }
}

void ContactPreview::showAbout()
{
    About::showAbout(this);
}
