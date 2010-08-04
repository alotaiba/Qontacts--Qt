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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "contactslist.h"
#include "about.h"
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Qontacts");

    iProgressDialog = new QProgressDialog(this);

    //Create actions
    aboutAction = new QAction(tr("About"), this);

    menuBar()->clear();
    menuBar()->addAction(aboutAction);

    connect(iProgressDialog, SIGNAL(canceled()), iProgressDialog, SLOT(hide()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(ui->analyzeButton, SIGNAL(clicked()), this, SLOT(analyzeContacts()));

#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::analyzeContacts()
{
    iProgressDialog->setModal(true);
    iProgressDialog->setWindowTitle("Qontacts");
    iProgressDialog->setLabelText("Analyzing Contacts...");
    iProgressDialog->setMaximum(0);
    iProgressDialog->setMinimum(0);
    iProgressDialog->setValue(0);
    iProgressDialog->setCancelButtonText(QString());
    iProgressDialog->show();
    iContactsList = new ContactsList(this);
#ifdef Q_WS_MAEMO_5
    iContactsList->setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
    iProgressDialog->hide();
    iContactsList->show();
}

void MainWindow::showAbout()
{
    About::showAbout(this);
}
