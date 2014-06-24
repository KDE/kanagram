/******************************************************************************
 * This file is part of the Kanagram project
 * Copyright (c) 2012 Laszlo Papp <lpapp@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "kanagramenginehelper.h"

#include <kanagramsettings.h>

#include <sharedkvtmlfiles.h>

#include <Phonon/MediaObject>

#include <KDE/KStandardDirs>
#include <KDE/KLocale>
#include <KGlobalSettings>
#include <KConfigDialog>
#include <KAction>
#include <KActionCollection>
#include <KShortcutsEditor>

#include <kanagramsettings.h>
#include "mainsettings.h"
#include "vocabsettings.h"


#include <QtGui/QApplication>

KanagramEngineHelper::KanagramEngineHelper(KanagramGame* kanagramGame, QObject* parent)
    : QObject(parent)
    , m_kanagramGame(kanagramGame)
    ,m_speller(NULL)
    ,m_player(NULL)
    , m_insertCounter(0)
    ,m_totalScore(0)
{
    m_actionCollection = new KActionCollection(this);
    m_speller = new Sonnet::Speller();
    m_speller->setLanguage(m_kanagramGame->sanitizedDataLanguage());

    loadSettings();
}

KanagramEngineHelper::~KanagramEngineHelper()
{
    delete m_kanagramGame;
    delete m_speller;
    m_speller=NULL;
    delete m_player;
    m_player=NULL;
}

QString KanagramEngineHelper::createNextAnagram()
{
    m_kanagramGame->nextAnagram();
    QString anagram;
    anagram = m_kanagramGame->anagram();
    if (m_useSounds)
        {
            play("chalk.ogg");
        }
    return anagram;
}

QStringList KanagramEngineHelper::insertInCurrentOriginalWord(int index, const QString& letter)
{
    int anagramWordSize = m_kanagramGame->word().size();

    if (anagramWordSize != m_currentOriginalWord.size()
            || m_currentOriginalWord.size() == m_insertCounter)
    {
        m_currentOriginalWord.clear();
        m_insertCounter = 0;
    }

    while (m_currentOriginalWord.size() < anagramWordSize)
        m_currentOriginalWord.append("");

    m_currentOriginalWord.replace(index, letter);
    ++m_insertCounter;
    return m_currentOriginalWord;
}

QStringList KanagramEngineHelper::removeInCurrentOriginalWord(int index)
{
    m_currentOriginalWord.replace(index, "");
    --m_insertCounter;
    return m_currentOriginalWord;
}

QString KanagramEngineHelper::anagramOriginalWord() const
{
    QString originalWord = m_kanagramGame->word();
    return originalWord;
}

QString KanagramEngineHelper::showHint() const
{
    QString hint = m_kanagramGame->hint();
    return hint;
}

QString KanagramEngineHelper::categoryName() const
{
    QString categoryTitle = m_kanagramGame->documentTitle();
    return categoryTitle;
}

QString KanagramEngineHelper::nextVocabulary()
{
    m_kanagramGame->nextVocabulary();
    if (m_useSounds)
        {
            play("chalk.ogg");
        }
    return m_kanagramGame->documentTitle();
}

QString KanagramEngineHelper::previousVocabulary()
{
    m_kanagramGame->previousVocabulary();
    if (m_useSounds)
        {
            play("chalk.ogg");
        }
    return m_kanagramGame->documentTitle();
}


bool KanagramEngineHelper::checkWord(QString answer)
{
    QString enteredWord = answer.toLower().trimmed();
    QString word = m_kanagramGame->word().toLower().trimmed();
    if (!enteredWord.isEmpty())
    {
        if (enteredWord == word || stripAccents(enteredWord) == stripAccents(word) ||
           (m_speller->isCorrect(enteredWord) && isAnagram(enteredWord, word)))
        {
            if (m_enablePronunciation)
            {
                /*// User wants words spoken, but if there's no audio file, play right.ogg
                if (m_kanagramGame->audioFile().isEmpty())
                    say(m_kanagramGame->word());
                else
                    // otherwise play the sound associated with the word.
                    play(m_kanagramGame->audioFile().pathOrUrl());*/
            }
            else if (m_useSounds)
            {
                // Otherwise just play right.ogg
                play("right.ogg");
            }
            return true;
        }
        else
        {
            if (m_useSounds) play("wrong.ogg");
            return false;
        }
    }
    else
        return false;
}

bool KanagramEngineHelper::isAnagram(QString& enteredword, QString& word)
{
    QString test = word;
    if (enteredword.length() <= word.length())
    {
        for (int i=0; i < enteredword.length(); i++)
        {
            int found = test.indexOf(enteredword[i]);

            if (found != -1)
            {
                test.remove(found, 1);
            }
            else
                break;
        }

        if (test.isEmpty())
            return true;
        else
            return false;
    }
    else
        return false;
}

int KanagramEngineHelper::getNumericSetting(QString settingString)
{
    int indexFound_setting = settingString.size();
    for (int k = 0; k < indexFound_setting; ++k)
    {
        if (!settingString.at(k).isDigit())
        {
            indexFound_setting = k;
            break;
        }
    }
    return settingString.left(indexFound_setting).toInt();
}

void KanagramEngineHelper::loadSettings() 
{
    QString hideTime = KanagramSettings::hintHideTime();

    if (hideTime.at(0).isDigit())
    {
        // because the choices are 0, 3, 5, 7, 9
        m_hintHideTime = (hideTime.at(0).digitValue() * 2) + 1;

        // reset to 0 if it's 1 to allow for the don't hide option
        if (m_hintHideTime == 1)
        {
            m_hintHideTime = 0;
        }
    }
    else
    {
        m_hintHideTime = 0;
    }

    QString resolveTime = KanagramSettings::resolveTime();

    if (resolveTime.at(0).isDigit())
    {

        // because the choices are 0, 15, 30, 45, 60 seconds
        m_resolveTime = (resolveTime.at(0).digitValue()) * 15;
    }
    else
    {
        m_resolveTime = 0;
    }

    QString scoreTime = KanagramSettings::scoreTime();

    m_scoreTime = getNumericSetting(scoreTime);
    m_scoreTime = (m_scoreTime + 1)*15;

    QString correctAnswerScore = KanagramSettings::correctAnswerScore();

    m_correctAnswerScore = getNumericSetting(correctAnswerScore);
    m_correctAnswerScore = (m_correctAnswerScore + 1)*5;

    QString incorrectAnswerScore = KanagramSettings::incorrectAnswerScore();

    m_incorrectAnswerScore = getNumericSetting(incorrectAnswerScore);
    m_incorrectAnswerScore = (m_incorrectAnswerScore + 1)*(-1);
 
    QString revealAnswerScore = KanagramSettings::revealAnswerScore();

    m_revealAnswerScore = getNumericSetting(revealAnswerScore);
    m_revealAnswerScore = (m_revealAnswerScore + 1)*(-2);

    QString skippedWordScore = KanagramSettings::skippedWordScore();
 
    m_skippedWordScore = getNumericSetting(skippedWordScore);
    m_skippedWordScore = (m_skippedWordScore + 1)*(-2);

    if (KanagramSettings::dataLanguage().isEmpty())
    {
        QStringList userLanguagesCode = KGlobal::locale()->languageList();
        QStringList sharedKvtmlFilesLanguages = SharedKvtmlFiles::languages();
        QString foundLanguage;
        foreach (const QString &userLanguageCode, userLanguagesCode)
        {
            if (sharedKvtmlFilesLanguages.contains(userLanguageCode))
            {
                foundLanguage = userLanguageCode;
                break;
            }
        }

        KanagramSettings::setDataLanguage(!foundLanguage.isEmpty() ? foundLanguage : "en");
    }

    m_useSounds = KanagramSettings::useSounds();
    m_enablePronunciation = KanagramSettings::enablePronunciation();
}

void KanagramEngineHelper::refreshVocabularies()
{
    if (m_kanagramGame->refreshVocabularyList())
    {
        // vocab/word are no longer valid, so get new ones and hide the hint
        m_kanagramGame->nextVocabulary();
        m_kanagramGame->nextAnagram();
        //hideHint();

        if (m_useSounds)
        {
            play("chalk.ogg");
        }

        // save the default vocab now that it's changed
        KanagramSettings::setDefaultVocabulary(m_kanagramGame->filename());
        KanagramSettings::self()->writeConfig();
        m_vocabSettings->refreshView();
    }
}

void KanagramEngineHelper::reloadSettings()
{
    loadSettings();
    refreshVocabularies();
}

void KanagramEngineHelper::play(const QString& filename)
{
    if (!filename.isEmpty())
    {
        QString soundFile = KStandardDirs::locate("appdata", "sounds/" + filename);
        if (soundFile.isEmpty())
            soundFile = filename;

        if (!m_player)
        {
            m_player = Phonon::createPlayer(Phonon::GameCategory, soundFile);
        }
        else
        {
            m_player->setCurrentSource(soundFile);
        }
        m_player->play();
    }
}

QString KanagramEngineHelper::stripAccents(QString& original)
{
    QString noAccents;
    QString decomposed = original.normalized(QString::NormalizationForm_D);
    for (int i = 0; i < decomposed.length(); ++i) {
        if ( decomposed[i].category() != QChar::Mark_NonSpacing ) {
            noAccents.append(decomposed[i]);
        }
    }
    return noAccents;
}

bool KanagramEngineHelper::compareWords() const
{
    return m_currentOriginalWord.join("") == m_kanagramGame->word();
}

void KanagramEngineHelper::slotShowSettings()
{
    if (!KConfigDialog::showDialog("settings"))
    {
        m_configDialog = new KConfigDialog( NULL, "settings", KanagramSettings::self() );
        //m_configDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(m_configDialog, SIGNAL(finished()), this, SLOT(reloadSettings()));

        // add the main settings page
        MainSettings * mainSettingsPage = new MainSettings( m_configDialog );
        connect (mainSettingsPage, SIGNAL(settingsChanged()), this, SLOT(reloadSettings()));
        m_configDialog->addPage(mainSettingsPage , i18nc("@title:group main settings page name", "General" ), "preferences-other" );

        // create and add the vocabsettings page
        m_vocabSettings = new VocabSettings( m_configDialog );
        m_configDialog->addPage(m_vocabSettings, i18n("Vocabularies"), "document-properties" );

        // now make and add the shortcuts page
        m_shortcutsEditor = new KShortcutsEditor(m_actionCollection, m_configDialog);
        m_configDialog->addPage(m_shortcutsEditor, i18n("Shortcuts"), "preferences-desktop-keyboard");
        connect(m_configDialog, SIGNAL(accepted()), this, SLOT(slotSaveSettings()));
        connect(m_configDialog, SIGNAL(rejected()), this, SLOT(slotSettingsCancelled()));
        connect(m_shortcutsEditor, SIGNAL(keyChange()), this, SLOT(slotEnableApplyButton()));

        m_configDialog->setHelp("kanagram/index.html");
        m_configDialog->resize(600, 500);
        m_configDialog->show();
    }
}

void KanagramEngineHelper::resetTotalScore()
{
    m_totalScore=0;
}

int KanagramEngineHelper::totalScore(int points)
{
    m_totalScore+=points;
    return m_totalScore;
}

int KanagramEngineHelper::correctAnswerScore()
{
    QString correctAnswerScore = KanagramSettings::correctAnswerScore();

    m_correctAnswerScore = getNumericSetting(correctAnswerScore);
    return ((m_correctAnswerScore + 1)*5);
}

int KanagramEngineHelper::incorrectAnswerScore()
{
    QString incorrectAnswerScore = KanagramSettings::incorrectAnswerScore();

    m_incorrectAnswerScore = getNumericSetting(incorrectAnswerScore);
    return ((m_incorrectAnswerScore + 1)*(-1));
}

int KanagramEngineHelper::revealAnswerScore()
{
    QString revealAnswerScore = KanagramSettings::revealAnswerScore();

    m_revealAnswerScore = getNumericSetting(revealAnswerScore);
    return ((m_revealAnswerScore + 1)*(-2));
}

int KanagramEngineHelper::skippedWordScore()
{
    QString skippedWordScore = KanagramSettings::skippedWordScore();
 
    m_skippedWordScore = getNumericSetting(skippedWordScore);
    return ((m_skippedWordScore + 1)*(-2));
}

void KanagramEngineHelper::slotSaveSettings()
{
    m_shortcutsEditor->save();
    // TODO: Update the current puzzle based on the new settings
}

void KanagramEngineHelper::slotSettingsCancelled()
{
    m_shortcutsEditor->undoChanges();
}

void KanagramEngineHelper::slotEnableApplyButton()
{
    m_configDialog->enableButtonApply(true);
}

int KanagramEngineHelper::hintHideTime()
{
    return KanagramSettings::hintHideTime().toInt();
}

void KanagramEngineHelper::setHintHideTime(int hintHideTime)
{
    KanagramSettings::setHintHideTime(QString::number(hintHideTime));
    emit hintHideTimeChanged();
}

int KanagramEngineHelper::resolveTime()
{
    return KanagramSettings::resolveTime().toInt();
}

void KanagramEngineHelper::setResolveTime(int resolveTime)
{
    KanagramSettings::setResolveTime(QString::number(resolveTime));
    emit resolveTimeChanged();
}

int KanagramEngineHelper::scoreTime()
{
    QString scoreTime = KanagramSettings::scoreTime();

    m_scoreTime = getNumericSetting(scoreTime);
    return ((m_scoreTime + 1)*15);
}

bool KanagramEngineHelper::useSounds()
{
    return KanagramSettings::useSounds();
}

void KanagramEngineHelper::setUseSounds(bool useSounds)
{
    KanagramSettings::setUseSounds(useSounds);
    emit useSoundsToggled();
}

QString KanagramEngineHelper::defaultVocabulary()
{
    return KanagramSettings::defaultVocabulary();
}

void KanagramEngineHelper::setDefaultVocabulary(const QString& defaultVocabulary)
{
    KanagramSettings::setDefaultVocabulary(defaultVocabulary);
    emit defaultVocabularyChanged();
}

void KanagramEngineHelper::saveSettings()
{
    KanagramSettings::self()->writeConfig();
    m_kanagramGame->refreshVocabularyList();
}

#include "kanagramenginehelper.moc"
