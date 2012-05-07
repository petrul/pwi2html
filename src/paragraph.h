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
#ifndef PWIPARAGRAPH_H
#define PWIPARAGRAPH_H

#include <string>
#include <map>

using namespace std;

namespace pwi {

/**
 * A PwiDocument paragraph. Manages inline formatting
 * @author Adrian Petru Dimulescu
 */
class Paragraph{
public:
    static const int MARKUP_ITALIC_BEGIN = 1;
    static const int MARKUP_ITALIC_END = 2;
    static const int MARKUP_BOLD_BEGIN = 3;
    static const int MARKUP_BOLD_END = 4;
    static const int MARKUP_UNDERLINE_BEGIN = 5;
    static const int MARKUP_UNDERLINE_END = 6;
    static const int MARKUP_STRIKETHROUGH_BEGIN = 7;
    static const int MARKUP_STRIKETHROUGH_END = 8;

public:
    Paragraph();
    Paragraph(string& str);
    ~Paragraph();
    string& getText();
    
    /**
     * delegate for string operator +=
     */
    void operator += (string& str);
    
    /**
     * delegate for string operator +=
     */
    void operator += (const char* str);
    
    /**
     * delegate for string operator +=
     */
    void operator += (char c);

    /**
     * start markup italic
     */
    void startMarkupItalic();
    
    /**
     * end markup italic
     */
    void endMarkupItalic();

    /**
     * start markup bold
     */
    void startMarkupBold();

    
    /**
     * end markup bold
     */
    void endMarkupBold();

    /**
     * accessor for markup
     */    
    map<int, int>& getMarkup();

    int isBoldON() { return isBoldOn; };
    int isItalicON() { return isItalicOn; };

private:
    string s_text;
    map<int, int> markup; // position => markup type
    
    int isItalicOn; // is italic markup on ?
    int isBoldOn;  // is bold markup on ?
   
};

};

#endif
