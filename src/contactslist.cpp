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

#include "contactslist.h"
#include "ui_contactslist.h"
#include "contactpreview.h"
#include "about.h"
#include <QStandardItem>
#include <QLabel>
#include <QMenuBar>
#include <QList>
#include <QString>
#include <QStringList>
#include <QContactManager>
#include <QDebug>
#ifdef Q_WS_MAEMO_5
#include <QMaemo5InformationBox>
#endif

ContactsList::ContactsList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ContactsList)
{
    ui->setupUi(this);

    setWindowTitle("Contacts List");

#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
    //Create actions
    aboutAction = new QAction(tr("About"), this);

    menuBar()->clear();
    menuBar()->addAction(aboutAction);

    iContactsModel = new ContactsModel();

    iContacts = iContactsModel->getUpdateableContacts();

    iContactsDataModel = new QStandardItemModel(ui->listView);
    iContactsSelectionModel = new QItemSelectionModel(iContactsDataModel);
    iProgressDialog = new QProgressDialog(this);

    ui->listView->setModel(iContactsDataModel);
    ui->listView->setSelectionModel(iContactsSelectionModel);

    connect(ui->selectListButton, SIGNAL(clicked()), this, SLOT(setContactsListSelection()));
    connect(ui->preivewButton, SIGNAL(clicked()), this, SLOT(showPreview()));
    connect(ui->listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(enablePreview(QModelIndex,QModelIndex)));
    connect(ui->multiUpdateButton, SIGNAL(clicked()), this, SLOT(updateSelectedContacts()));
    connect(ui->listView->model(), SIGNAL(itemChanged(QStandardItem*)), this, SLOT(setCheckedItem(QStandardItem*)));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));

    updateList();

    setUpdateSelectButtonsState();

    //Quick fix to drive the focus away from ListView to disable preivew button
    //TODO: Improve here plz!!!
    ui->selectListButton->setFocus();
}

ContactsList::~ContactsList()
{
    delete ui;
    delete iContactsModel;
}

void ContactsList::updateList()
{
    iContactsDataModel->clear();
    iContactsSelectionModel->clear();

    if (iContacts->count() > 0) {
        for (int i = 0; i < iContacts->count(); i++) {
            ContactData contact = iContacts->at(i);
            QStandardItem *item = new QStandardItem(QString("%1 (%2)").arg(contact.iName, contact.iNumber));
            if (contact.isChecked) {
                    item->setCheckState(Qt::Checked);
            } else {
                    item->setCheckState(Qt::Unchecked);
            }
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            iContactsDataModel->appendRow(item);
        }
    } else {
        QLabel *noContactsLabel = new QLabel("<p align=\"center\">No contacts to update.</p>", this);
        noContactsLabel->setWordWrap(true);
        setCentralWidget(noContactsLabel);
    }

    iContactsSelectionModel->clearSelection();
    ui->preivewButton->setEnabled(false);

    //Quick fix to drive the focus away from ListView to disable preivew button
    //TODO: Improve here plz!!!
    ui->selectListButton->setFocus();

    setUpdateSelectButtonsState();
}

void ContactsList::showPreview()
{
    QModelIndex *currentListViewIndex = new QModelIndex(ui->listView->currentIndex());
    iContactPreview = new ContactPreview(this, iContacts, currentListViewIndex);

    connect(iContactPreview, SIGNAL(showContactsList()), this, SLOT(updateList()));
#ifdef Q_WS_MAEMO_5
    iContactPreview->setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
    iContactPreview->show();
}

QMap<int, ContactData> ContactsList::getSelectedContacts()
{
    QMap<int, ContactData> selectedContacts = QMap<int, ContactData>();
    for (int i = 0; i < iContacts->count(); i++) {
        ContactData contact = iContacts->at(i);
        if (contact.isChecked) {
            selectedContacts.insert(i, contact);
        }
    }
    return selectedContacts;
}

void ContactsList::setCheckedItem(QStandardItem *item)
{
    ContactData contact = iContacts->at(item->row());
    contact.isChecked = !contact.isChecked;
    iContacts->replace(item->row(), contact);

    setUpdateSelectButtonsState();
}

void ContactsList::enablePreview(QModelIndex current, QModelIndex previous)
{
    if (current.row() >= 0) {
        ui->preivewButton->setEnabled(true);
    } else {
        ui->preivewButton->setEnabled(false);
    }
}

void ContactsList::updateSelectedContacts()
{
    QMap<int, ContactData> selectedContacts = getSelectedContacts();

    QList<int> indexesToRemove = QList<int>();

    bool failed = false;

    int maxValue = selectedContacts.count();
    int minValue = 0;

    if ( maxValue > 0 ) {
        QString qUpdateQuestion = QString("Are you sure you want to update %1 contacts?").arg(maxValue);
        int qUpdateAnswer = QMessageBox::question(this, "Confirm Update", qUpdateQuestion, QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);
        if( qUpdateAnswer == QMessageBox::Yes ) {
            iProgressDialog->setModal(true);
            iProgressDialog->setWindowTitle("Qontacts");
            iProgressDialog->setLabelText("Preparing to update your contacts...");
            iProgressDialog->setMaximum(minValue);
            iProgressDialog->setMinimum(minValue);
            iProgressDialog->setValue(minValue);
            iProgressDialog->setCancelButtonText(QString());
            iProgressDialog->show();
            int steps = 0;

            QMapIterator<int, ContactData> contactMap(selectedContacts);
            while (contactMap.hasNext()) {
                //qApp->processEvents();

                contactMap.next();

                ContactData contact = contactMap.value();
                int listItemRow = contactMap.key();

                QContactLocalId contactId = contact.id;
                QContactManager *contactManager = QContactManager::fromUri(contact.iManagerUri);
                if (!ContactsModel::updateContactNumbers(contactManager, contactId)) {
                    qDebug() << "Fail!";
                    iProgressDialog->cancel();
                    failed = true;
                    break;
                } else {
                    indexesToRemove.append(listItemRow);
                }
                steps++;
                iProgressDialog->setValue(steps);
                QString labelText = QString("Updating Contacts (%1/%2)").arg(QString::number(steps), QString::number(maxValue));
                iProgressDialog->setLabelText(labelText);

            }
            iProgressDialog->hide();

            for(int i = iContacts->count() - 1; i > -1; --i){
                if ( indexesToRemove.contains(i) ) {
                    iContacts->removeAt(i);
                }
            }

            updateList();

            if ( failed ) {
                #ifdef Q_WS_MAEMO_5
                QMaemo5InformationBox::information(this, "Operation Failed!", QMaemo5InformationBox::DefaultTimeout);
                #else
                QMessageBox::critical(this, "Operation Failed", "Updating contacts failed!");
                #endif
            } else {
                QString updateMsg = QString("You have updated %1 contacts sucessfully").arg(QString::number(maxValue));
                #ifdef Q_WS_MAEMO_5
                QMaemo5InformationBox::information(this, updateMsg, QMaemo5InformationBox::DefaultTimeout);
                #else
                QMessageBox::information(this, "Contacts Updated", updateMsg);
                #endif
            }
        }
    }
}

void ContactsList::setUpdateSelectButtonsState()
{
    QMap<int, ContactData> selectedContacts = getSelectedContacts();
    if ( selectedContacts.count() > 0 ){
        ui->multiUpdateButton->setEnabled(true);
        ui->multiUpdateButton->setText(QString("Update (%1)").arg(QString::number(selectedContacts.count())));
    } else {
        ui->multiUpdateButton->setEnabled(false);
        ui->multiUpdateButton->setText("Update");
    }

    if ( iContacts->count() == selectedContacts.count() ) {
        ui->selectListButton->setText("Select None");
    } else {
        ui->selectListButton->setText("Select All");
    }
}

void ContactsList::setContactsListSelection()
{
    for (int i = 0; i < iContacts->count(); i++) {
        ContactData contact = iContacts->at(i);
        if ( ui->selectListButton->text() == "Select None" ) {
            contact.isChecked = false;
        } else {
            contact.isChecked = true;
        }
        iContacts->replace(i, contact);
    }
    updateList();
}

void ContactsList::showAbout()
{
    About::showAbout(this);
}
