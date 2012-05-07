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
#include "xhtmlpwiexporter.h"
#include "paragraph.h"
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

using namespace std;

namespace pwi {

XhtmlPwiExporter::XhtmlPwiExporter(PwiDocument& doc)
 : AbstractPwiExporter()
{
	this->doc = &doc;
}


XhtmlPwiExporter::~XhtmlPwiExporter()
{
}

istream* XhtmlPwiExporter::exportPwi () {
        string* resstr = new string();
        string& res = *resstr;

        res += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        res += "<html>";
        res += "<head>\n<meta http-equiv=\"content-type\" content=\"text/html; utf-8\"/>\n</head>\n";
	res += "<body>";
        
        vector<Paragraph*>& paragraphs = this->doc->getParagraphs();
        for (int i = 0; i < paragraphs.size(); i++) {
            Paragraph* crtPara = paragraphs[i];
	    string paragraphText = crtPara->getText();

	    // TODO: here I get inside the guts of Paragraph from outside. This is not good but for now I'm not sure how inline markup should be properly implemented.
	    map<int, int>& markup = crtPara -> getMarkup(); // position => markup type

	    res += "\n<p>";
	    map<int, int>::iterator i = markup.begin();
	    int strIndex = 0;
	    while (i != markup.end()) {
		pair<const int, int>& elem = *i;
		int pos = elem.first;
		int whatMarkup = elem.second;
		
		res += string(paragraphText.substr(strIndex, pos - strIndex));
		res += this->getMarkup(whatMarkup);
		strIndex = pos;
		i++;
	    }
	    res += paragraphText.substr(strIndex, paragraphText.length());

	    // close formatting tags if not already closed. FIXME:
/*	    if (crtPara->isBoldON())
		res += this->getMarkup(Paragraph::MARKUP_BOLD_END);

	    if (crtPara->isItalicON())
		res += this->getMarkup(Paragraph::MARKUP_ITALIC_END);*/
	    
            res += "</p>";
        }
        
        res += "\n</body>\n</html>";
        
	return new istringstream(res);
}

char* XhtmlPwiExporter::getMarkup(int code) {
    switch (code) {
	case Paragraph::MARKUP_ITALIC_BEGIN:
	    return "<i>";
	case Paragraph::MARKUP_ITALIC_END:
	    return "</i>";
	case Paragraph::MARKUP_BOLD_BEGIN:
	    return "<b>";
	case Paragraph::MARKUP_BOLD_END:
	    return "</b>";


	default:
	    return "";
    }
}

};
