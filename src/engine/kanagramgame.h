/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007 by Jeremy Whiting <jpwhiting@kde.org>              *
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

#ifndef KANAGRAMGAME_H
#define KANAGRAMGAME_H

#include "kanagram_engine_export.h"

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include <krandomsequence.h>

class KEduVocDocument;

/** @brief game api
 * @author Joshua Keel <joshuakeel@gmail.com>
 * @author Jeremy Whiting <jpwhiting@kde.org>
 */
class KANAGRAM_ENGINE_EXPORT KanagramGame : public QObject
{
    Q_OBJECT
    public:
        /** Default constructor */
        explicit KanagramGame();

        /** Default destructor */
        ~KanagramGame();

        /** Get the anagram to show */
        QString anagram() const;

        /** Get this anagram's hint */
        QString hint() const;

        /** Get this anagram's answer */
        QString word() const;

        /** Get the current vocabulary file's title */
        QString documentTitle() const;

        /** Get the current vocabulary file's filename */
        QString filename() const;

        /** Get the list of vocabularies */
        QStringList vocabularyList() const;

    public slots:

        /** Set the vocabulary to use */
        void useVocabulary(const QString &vocabularyname);

        /** Refresh the list of vocabulary files available
         * from what we find on disk
         *@returns true if the current vocabulary has changed so the ui can refresh
         */
        bool refreshVocabularyList();

        /** Load the default vocabulary file */
        void loadDefaultVocabulary();

        /** Set the index to the next word */
        void nextAnagram();

        /** Use the next vocabulary file in the list */
        void nextVocabulary();

        /** Use the previous vocabulary file in the list */
        void previousVocabulary();

        /** Restore the word, set the anagram to the answer */
        void restoreWord();

    signals:

        /** Signal the ui that a there's a file error of some kind */
        void fileError(const QString &filename);

    private:

        /** Make the word into an anagram */
        void createAnagram();

        /** Check the current file */
        bool checkFile();

        /** Random sequence used to scramble the letters */
        KRandomSequence m_random;

        /** The current scrambled word */
        QString m_anagram;

        /** The current anagram's hint */
        QString m_hint;

        /** The current anagram's answer */
        QString m_originalWord;

        /** Which index the current filename is in m_fileList */
        int m_index;

        /** The list of vocabulary files */
        QStringList m_fileList;

        /** The list of words that have been answered */
        QStringList m_answeredWords;

        /** The current vocabulary's filename */
        QString m_filename;

        /** The current document */
        KEduVocDocument* m_document;
};

#endif

