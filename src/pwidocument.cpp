/***************************************************************************
 *   Copyright (C) 2004 by Adrian Petru Dimulescu                          *
 *   adrian.dimulescu@free.fr                                              *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "pwidocument.h"

namespace pwi {

PwiDocument::PwiDocument()
{
}


PwiDocument::~PwiDocument()
{
    
    for (int i = 0; i < this->paragraphs.size(); i++)
        if (this->paragraphs [i] != NULL)
            delete this->paragraphs[i];
}


/*!
    \fn pwi::PwiDocument::addParagraph (Paragraph& para)
 */
void PwiDocument::addParagraph (Paragraph* para)
{
    this->paragraphs.insert(this->paragraphs.end(), para);
}


/*!
    \fn pwi::PwiDocument::getParagraphs()
 */
vector<Paragraph*>& PwiDocument::getParagraphs()
{
    return this->paragraphs;
}

};
