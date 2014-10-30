/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *   Copyright (C) 2014 by Jeremy Whiting <jpwhiting@kde.org>              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/


#ifndef MAINSETTINGS_H
#define MAINSETTINGS_H

#include "ui_mainsettingswidget.h"

/** user preferences page of the config dialog
  * allows choosing a hint hide time,
  * the language, and the sound settings*/
class MainSettings : public QWidget, Ui::MainSettingsWidget
{
Q_OBJECT
public:
    /** default constructor */
    explicit MainSettings(QWidget *parent);

    /** default destructor */
    ~MainSettings();

    /** save the language setting that has been chosen
     * @returns whether the language has changed
     */
    bool saveLanguage();

public slots:

    /** toggle advanced settings */
    void slotToggleAdvancedSettings();

signals:

    /** signifies the language widget has been changed */
    void widgetModified();

private:

    /** get languages from data folders
      * populate the language combobox with the names
      * also puts the folder name in the userData of the combobox for quick retrieval
      */
    void populateLanguageBox();
};

#endif

