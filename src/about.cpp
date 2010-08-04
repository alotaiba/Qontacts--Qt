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

#include "about.h"

About::About(QWidget *parent) :
    QWidget(parent)
{
}


void About::showAbout(QWidget *parent)
{
    QString version = VERSION;
    QString aboutTitle = "About Qontacts";
    QString aboutText = QString("Qontacts v.%1\n" \
                                "Developed by: Abdulrahman Alotaiba\n" \
                                "http://www.qontactsapp.com/\n" \
                                "(c) 2010 Abdulrahman Alotaiba.").arg(version);
    QMessageBox::about(parent, aboutTitle, aboutText);
}

void About::showAboutQt(QWidget *parent)
{
    QMessageBox::aboutQt(parent);
}
