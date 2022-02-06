/*
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

#include "kanagramconfigdialog.h"

#include "mainsettings.h"
#include "vocabsettings.h"

#include <KLocalizedString>

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(KANAGRAM)

KanagramConfigDialog::KanagramConfigDialog(QWidget *parent, const QString &name,
                             KCoreConfigSkeleton *config) :
    KConfigDialog(parent, name, config)
{
    // add the main settings page
    m_mainSettingsPage = new MainSettings( this );
    addPage(m_mainSettingsPage , i18nc("@title:group main settings page name", "General" ), QStringLiteral("preferences-other") );
    connect(m_mainSettingsPage, &MainSettings::widgetModified, this, &KanagramConfigDialog::settingsModified);

    // create and add the vocabsettings page
    m_vocabSettingsPage = new VocabSettings( this );
    addPage(m_vocabSettingsPage, i18n("Vocabularies"), QStringLiteral("document-properties") );
    connect(m_vocabSettingsPage, &VocabSettings::widgetModified, this, &KanagramConfigDialog::settingsModified);

    m_hasChanged = false;

    setHelp(QStringLiteral("configuring"), QStringLiteral("kanagram"));
}

KanagramConfigDialog::~KanagramConfigDialog()
{
}

void KanagramConfigDialog::updateSettings()
{
    if (m_mainSettingsPage->saveLanguage()) {
        Q_EMIT settingsChanged(objectName());
        m_hasChanged = false;
    }
    KConfigDialog::updateSettings();
}

void KanagramConfigDialog::updateWidgets()
{
}

void KanagramConfigDialog::updateWidgetsDefault()
{
}

bool KanagramConfigDialog::hasChanged()
{
    return m_hasChanged;
}

bool KanagramConfigDialog::isDefault()
{
    return true;
}

void KanagramConfigDialog::settingsModified()
{
    m_hasChanged = true;
    updateButtons();
}

#include "moc_kanagramconfigdialog.cpp"
