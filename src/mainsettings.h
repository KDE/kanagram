/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel                                     *
 *   joshuakeel@gmail.com                                                  *
 *                                                                         *
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

class KConfigDialog;

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
	
	public slots:
		
		/** save the language setting that has been chosen */
		void slotUpdateLanguage();
		
		/** enable the apply button on the config dialog because something has been changed */
		void slotSetDirty();
                
                /** toggle advanced settings */
                void slotToggleAdvancedSettings();
        
    signals:

        /** signifies slotUpdateLanguage has completed, so language setting has been saved */
        void settingsChanged();
	
	private:
	
		/** get languages from data folders
		  * populate the language combobox with the names
		  * also puts the folder name in the userData of the combobox for quick retrieval
		  */
		void populateLanguageBox();
		
		/** cache pointer to config dialog so we can enable the apply button in slotSetDirty */
		KConfigDialog *m_parent;
};

#endif

