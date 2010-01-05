/***************************************************************************
 *   Copyright (C) 2005 by Joshua Keel <joshuakeel@gmail.com>              *
 *             (C) 2007 by Jeremy Whiting <jpwhiting@kde.org>              *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.           *
 ***************************************************************************/

#ifndef NEWSTUFF_H
#define NEWSTUFF_H

#include "ui_newstuffwidget.h"

namespace KNS
{
    class Engine;
}

/** settings page to get new vocabularies through KNewStuff */
class NewStuff : public QDialog, public Ui::NewStuffWidget
{
Q_OBJECT
    public:
        /** default constructor */
        explicit NewStuff(QWidget *parent);

        /** default destructor */
        ~NewStuff();

    private slots:
        /** get new vocabularies
          * invokes the KNewStuff dialog for downloading
          */
        void on_btnGetNew_clicked();

    private:
        KNS::Engine * m_engine;
};

#endif

