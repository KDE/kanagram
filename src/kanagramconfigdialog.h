/*
 *  This file is part of the Kanagram
 *  Copyright (C) 2014 Jeremy Whiting <jpwhiting@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 ***************************************************************************/

#ifndef KANAGRAM_CONFIG_DIALOG_H
#define KANAGRAM_CONFIG_DIALOG_H

#include <KConfigDialog>

class KConfig;
class KCoreConfigSkeleton;

class MainSettings;
class VocabSettings;

class KanagramConfigDialog : public KConfigDialog
{
    Q_OBJECT

public:
    /**
     * @param parent - The parent of this object.  Even though the class
     * deletes itself the parent should be set so the dialog can be centered
     * with the application on the screen.
     *
     * @param name - The name of this object.  The name is used in determining if
     * there can be more than one dialog at a time.  Use names such as:
     * "Font Settings" or "Color Settings" and not just "Settings" in
     * applications where there is more than one dialog.
     *
     * @param config - Config object containing settings.
     */
    KanagramConfigDialog(QWidget *parent, const QString &name,
                  KCoreConfigSkeleton *config);

    /**
     * Deconstructor, removes name from the list of open dialogs.
     * Deletes private class.
     * @see exists()
     */
    ~KanagramConfigDialog();

protected Q_SLOTS:
    /**
     * Update the settings from the dialog.
     * Virtual function for custom additions.
     *
     * Example use: User clicks Ok or Apply button in a configure dialog.
     */
    virtual void updateSettings();

    /**
     * Update the dialog based on the settings.
     * Virtual function for custom additions.
     *
     * Example use: Initialisation of dialog.
     * Example use: User clicks Reset button in a configure dialog.
     */
    virtual void updateWidgets();

    /**
     * Update the dialog based on the default settings.
     * Virtual function for custom additions.
     *
     * Example use: User clicks Defaults button in a configure dialog.
     */
    virtual void updateWidgetsDefault();

protected:

    /**
     * Returns whether the current state of the dialog is
     * different from the current configuration.
     * Virtual function for custom additions.
     */
    virtual bool hasChanged();

    /**
     * Returns whether the current state of the dialog is
     * the same as the default configuration.
     */
    virtual bool isDefault();

private slots:
    void settingsModified();

private:
    MainSettings *m_mainSettingsPage;
    VocabSettings *m_vocabSettingsPage;
    bool m_hasChanged;
};

#endif //KANAGRAM_CONFIG_DIALOG_H

