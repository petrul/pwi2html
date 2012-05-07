/***************************************************************************
 *   Copyright (C) 2004 by  David Eriksson                                 *
 *   <twogood@users.sourceforge.net>                                       *
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
#ifndef PWIBINARYPWIPARSER_H
#define PWIBINARYPWIPARSER_H

#define PWI_MAGIC         "{\\pwi"
#define PWI_MAGIC_SIZE    5

#define FONT_ENTRY_SIZE   0x50

#define PARAGRAPH_ENTRY_SIZE  0x8


#include "pwidocument.h"
#include <istream>
#include <stdexcept>
#include <string>

using namespace std;

namespace pwi {

/**
 * Parser of the binary pwi format
 *
 */
class BinaryPwiParser {

public:
    BinaryPwiParser(istream& is);
    ~BinaryPwiParser();

    /**
     * parses a binary stream into a PwiDocument object.
     */
    void parse(PwiDocument& doc) throw (runtime_error);
    
    /**
     * TODO: what does this do ?
     */
    void align(streampos start_offset);

    
private:
    istream* inputStream;


private:
    /**
     * reads a 2-byte value
     */
    unsigned BinaryPwiParser::read16();
    
    /**
     * parse the current paragraph as a text
     * @returns the parsed text
     */
    void text(Paragraph* str);

    /**
     * parse the current paragraph as a drawing
     * @returns the parsed drawing
     */
    void drawing();

};

};

#endif
