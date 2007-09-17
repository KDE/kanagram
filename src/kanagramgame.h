/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007 by Jeremy Whiting <jeremy@scitools.com>            *
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

#include <qobject.h>
#include <qstringlist.h>

#include <krandomsequence.h>

class KEduVocDocument;

/** @brief game api
 * @author Joshua Keel <joshuakeel@gmail.com>
 * @author Jeremy Whiting <jeremy@scitools.com>
 */
class KanagramGame : public QObject
{
    Q_OBJECT
    public:
        /** default constructor */
        explicit KanagramGame();

        /** default destructor */
        ~KanagramGame();

        /** get the anagram to show */
        QString getAnagram();

        /** get this anagram's hint */
        QString getHint();

        /** get this anagram's answer */
        QString getWord();

        /** get the current vocabulary file's title */
        QString getDocTitle();

        /** get the current vocabulary file's filename */
        QString getFilename();

        /** get the list of vocabularies */
        QStringList getVocabsList();

    public slots:

        /** set the vocab to use */
        void useVocab(const QString &vocabname);

        /** refresh the list of vocabulary files available
         * from what we find on disk
         *@returns true if the current vocabulary has changed so the ui can refresh
         */
        bool refreshVocabList();

        /** load the default vocab file */
        void loadDefaultVocab();

        /** set the index to the next word */
        void nextAnagram();

        /** use the next vocab file in the list */
        void nextVocab();

        /** use the previous vocab file in the list */
        void previousVocab();

        /** restore the word, set the anagram to the answer */
        void restoreWord();

    signals:

        /** signal the ui that a there's a file error of some kind */
        void fileError(const QString &filename);

    private:

        /** make the word into an anagram */
        void createAnagram();

        /** check the current file */
        void checkFile();

        /** find the current file in the list of files available */
        void updateIndex();

        /** random sequence used to scramble the letters */
        KRandomSequence m_random;

        /** the current scrambled word */
        QString m_anagram;

        /** the current anagram's hint */
        QString m_hint;

        /** the current anagram's answer */
        QString m_originalWord;

        /** which index the current filename is in m_fileList */
        int m_index;

        /** the list of vocabulary files */
        QStringList m_fileList;

        /** the list of words that have been answered */
        QStringList m_answeredWords;

        /** the current vocabulary's filename */
        QString m_filename;

        /** the current document */
        KEduVocDocument* m_doc;
};

#endif

