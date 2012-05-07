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

#include <fstream>
#include "openofficepwiexporter.h"
#include "zip.h"


namespace pwi {

OpenofficePwiExporter::OpenofficePwiExporter(PwiDocument& doc)
 : AbstractPwiExporter()
{
    this->doc           = &doc;
    this->isBoldOn      = false;
    this->isItalicOn    = false;
    this->isUnderlineOn = false;
}


bool bFileExists(const string& sFileName)
{
        ifstream iFileToCheck;
        iFileToCheck.open(sFileName.c_str());
        return iFileToCheck.is_open();
}

OpenofficePwiExporter::~OpenofficePwiExporter()
{
    // delete temporary file
    if (bFileExists(this->zipFilename))
        remove(this->zipFilename);
}

istream* OpenofficePwiExporter::exportPwi () {
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
    xmlDtdPtr dtd = NULL;       /* DTD pointer */

    
    doc = xmlNewDoc(BAD_CAST "1.0");
    
    dtd = xmlCreateIntSubset(doc, BAD_CAST "office:document-content", BAD_CAST "-//OpenOffice.org//DTD OfficeDocument 1.0//EN", BAD_CAST "office.dtd");
    
    root_node = xmlNewNode(NULL, BAD_CAST "office:document-content");
    xmlDocSetRootElement(doc, root_node);
    xmlNewProp(root_node, BAD_CAST "xmlns:office", BAD_CAST "http://openoffice.org/2000/office");
    xmlNewProp(root_node, BAD_CAST "xmlns:style", BAD_CAST "http://openoffice.org/2000/style");
    xmlNewProp(root_node, BAD_CAST "xmlns:text", BAD_CAST "http://openoffice.org/2000/text");
    xmlNewProp(root_node, BAD_CAST "xmlns:table", BAD_CAST "http://openoffice.org/2000/table");
    xmlNewProp(root_node, BAD_CAST "xmlns:draw", BAD_CAST "http://openoffice.org/2000/drawing");
    xmlNewProp(root_node, BAD_CAST "xmlns:fo", BAD_CAST "http://www.w3.org/1999/XSL/Format");
    xmlNewProp(root_node, BAD_CAST "xmlns:xlink", BAD_CAST "http://www.w3.org/1999/xlink");
    xmlNewProp(root_node, BAD_CAST "xmlns:number", BAD_CAST "http://openoffice.org/2000/datastyle");
    xmlNewProp(root_node, BAD_CAST "xmlns:svg", BAD_CAST "http://www.w3.org/2000/svg");
    xmlNewProp(root_node, BAD_CAST "xmlns:chart", BAD_CAST "http://openoffice.org/2000/chart");
    xmlNewProp(root_node, BAD_CAST "xmlns:dr3d", BAD_CAST "http://openoffice.org/2000/dr3d");
    xmlNewProp(root_node, BAD_CAST "xmlns:math", BAD_CAST "http://www.w3.org/1998/Math/MathML");
    xmlNewProp(root_node, BAD_CAST "xmlns:form", BAD_CAST "http://openoffice.org/2000/form");
    xmlNewProp(root_node, BAD_CAST "xmlns:script", BAD_CAST "http://openoffice.org/2000/script");
    xmlNewProp(root_node, BAD_CAST "office:class", BAD_CAST "text");
    xmlNewProp(root_node, BAD_CAST "office:version", BAD_CAST "1.0");


    xmlNewChild(root_node, NULL, BAD_CAST "office:script", NULL);
    
    xmlNodePtr fontDeclarations = xmlNewChild(root_node, NULL, BAD_CAST "office:font-decls", NULL);
        xmlNewChild(fontDeclarations, NULL, BAD_CAST "style:font-decl", NULL);

    /// style section
    xmlNodePtr automaticStyles = xmlNewChild(root_node, NULL, BAD_CAST "office:automatic-styles",  NULL);
    
    /// italic inline style
    xmlNodePtr style = xmlNewChild(automaticStyles, NULL, BAD_CAST "style:style",  NULL);
        xmlNewProp(style, BAD_CAST "style:name", BAD_CAST "italic-inline");
        xmlNewProp(style, BAD_CAST "style:family", BAD_CAST "text");
        xmlNodePtr styleProperties = xmlNewChild(style, NULL, BAD_CAST "style:properties",  NULL);
            xmlNewProp(styleProperties, BAD_CAST "fo:font-style", BAD_CAST "italic");
            xmlNewProp(styleProperties, BAD_CAST "style:font-style-asian", BAD_CAST "italic");
            xmlNewProp(styleProperties, BAD_CAST "style:font-style-complex", BAD_CAST "italic");

    /// bold inline style
    style = xmlNewChild(automaticStyles, NULL, BAD_CAST "style:style",  NULL);
        xmlNewProp(style, BAD_CAST "style:name", BAD_CAST "bold-inline");
        xmlNewProp(style, BAD_CAST "style:family", BAD_CAST "text");
        styleProperties = xmlNewChild(style, NULL, BAD_CAST "style:properties",  NULL);
            xmlNewProp(styleProperties, BAD_CAST "fo:font-weight", BAD_CAST "bold");
            xmlNewProp(styleProperties, BAD_CAST "style:font-weight-asian", BAD_CAST "bold");
            xmlNewProp(styleProperties, BAD_CAST "style:font-weight-complex", BAD_CAST "bold");

    /// bold-italic inline style
    style = xmlNewChild(automaticStyles, NULL, BAD_CAST "style:style",  NULL);
        xmlNewProp(style, BAD_CAST "style:name", BAD_CAST "bolditalic-inline");
        xmlNewProp(style, BAD_CAST "style:family", BAD_CAST "text");
        styleProperties = xmlNewChild(style, NULL, BAD_CAST "style:properties",  NULL);
            xmlNewProp(styleProperties, BAD_CAST "fo:font-weight", BAD_CAST "bold");
            xmlNewProp(styleProperties, BAD_CAST "fo:font-style", BAD_CAST "italic");
            xmlNewProp(styleProperties, BAD_CAST "style:font-weight-asian", BAD_CAST "bold");
            xmlNewProp(styleProperties, BAD_CAST "style:font-style-asian", BAD_CAST "italic");
            xmlNewProp(styleProperties, BAD_CAST "style:font-weight-complex", BAD_CAST "bold");
            xmlNewProp(styleProperties, BAD_CAST "style:font-style-complex", BAD_CAST "italic");

            
    xmlNodePtr body = xmlNewChild(root_node, NULL, BAD_CAST "office:body", NULL);

        // the following algorithm works only for bold and italic markup. adding other formats requires rewriting the way things are done.
        vector<Paragraph*>& paragraphs = this->doc->getParagraphs();
        for (int i = 0; i < paragraphs.size(); i++) {
            Paragraph* crtPara = paragraphs[i];
	    string paragraphText = crtPara->getText();
	    
	    // TODO: here I get inside the guts of Paragraph from outside. This is not good but for now I'm not sure how inline markup should be properly implemented.
	    map<int, int>& markup = crtPara -> getMarkup(); // position => markup type
	    
            
            xmlNodePtr crtParagraphNode = xmlNewChild(body, NULL, BAD_CAST "text:p", NULL);

	    map<int, int>::iterator i = markup.begin();
	    int strIndex = 0;
            
            // i'll use this to build nodes (bold, italic nodes etc.)
            xmlNodePtr crtNode = crtParagraphNode;
            
	    while (i != markup.end()) {
		pair<const int, int>& elem = *i;
		int pos = elem.first;
		int whatMarkup = elem.second;
		
		xmlAddChild(crtNode,
                    xmlNewText(BAD_CAST paragraphText.substr(strIndex, pos - strIndex).c_str()));
                
                if (whatMarkup == Paragraph::MARKUP_ITALIC_BEGIN) {
                    this->isItalicOn = true;
                    crtNode = xmlNewChild(crtParagraphNode, NULL, BAD_CAST "text:span", NULL);
                    if (this->isBoldOn) 
                        xmlNewProp(crtNode, BAD_CAST "text:style-name", BAD_CAST "bolditalic-inline");
                    else
                        xmlNewProp(crtNode, BAD_CAST "text:style-name", BAD_CAST "italic-inline");
                } else
		
                if (whatMarkup == Paragraph::MARKUP_BOLD_BEGIN) {
                    this->isBoldOn = true;
                    crtNode = xmlNewChild(crtParagraphNode, NULL, BAD_CAST "text:span", NULL);
                    if (this->isItalicOn) {
                        xmlNewProp(crtNode, BAD_CAST "text:style-name", BAD_CAST "bolditalic-inline");
                    } else
                        xmlNewProp(crtNode, BAD_CAST "text:style-name", BAD_CAST "bold-inline");
                } else

                if (whatMarkup == Paragraph::MARKUP_UNDERLINE_BEGIN) {
//                 for now, ignore
//                     crtNode = xmlNewChild(crtNode, NULL, BAD_CAST "text:span", NULL);
//                     xmlNewProp(crtNode, BAD_CAST "text:style-name", BAD_CAST "underline-inline");
                } else                
                if (whatMarkup == Paragraph::MARKUP_STRIKETHROUGH_BEGIN) {
//                 for now, ignore
//                     crtNode = xmlNewChild(crtNode, NULL, BAD_CAST "text:span", NULL);
//                     xmlNewProp(crtNode, BAD_CAST "text:style-name", BAD_CAST "underline-inline");
                } else

                /// else, if the markup ends, go up
                if (whatMarkup == Paragraph::MARKUP_ITALIC_END) {
                    this->isItalicOn = false;
                    if (crtNode != crtParagraphNode) // don't go upper than crtParagraphNode
                        crtNode = crtNode->parent;
                        
                    if (this->isBoldOn) { // bold is still on, must restart bold inline markup
                        crtNode = xmlNewChild(crtParagraphNode, NULL, BAD_CAST "text:span", NULL);
                        xmlNewProp(crtNode, BAD_CAST "text:style-name", BAD_CAST "bold-inline");
                    }
                    
                } else
                if (whatMarkup == Paragraph::MARKUP_BOLD_END) {
                    this->isBoldOn = false;
                    if (crtNode != crtParagraphNode) // don't go upper than crtParagraphNode
                        crtNode = crtNode->parent;
                        
                    if (this->isItalicOn) { // italic is still on, must restart italic inline markup
                        crtNode = xmlNewChild(crtParagraphNode, NULL, BAD_CAST "text:span", NULL);
                        xmlNewProp(crtNode, BAD_CAST "text:style-name", BAD_CAST "italic-inline");
                    }

                } else
                if (whatMarkup == Paragraph::MARKUP_UNDERLINE_END) {
                    crtNode = crtNode->parent;
                } else
                if (whatMarkup == Paragraph::MARKUP_STRIKETHROUGH_END) {
                    crtNode = crtNode->parent;
                } else

                ;
                           
		strIndex = pos;
		i++;
	    }
            
            xmlAddChild(crtNode,
                    xmlNewText(BAD_CAST paragraphText.substr(strIndex, paragraphText.length()).c_str()));

        }

     xmlChar *xmlbuff;
     int buffersize;
     
    xmlDocDumpMemory(doc, &xmlbuff, &buffersize);
    
    istream* result = saveToTmpFileAndReturnIstream(xmlbuff, buffersize);
    
    xmlFree(xmlbuff);
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();



    return result;
}


/*
    As this zip stuff only knows how to save a zip into file, I have to write to a temporary file, 
    read its contents to memory buffer, delete and then return an istream* to it.
 */
istream* OpenofficePwiExporter::saveToTmpFileAndReturnIstream(const xmlChar* contents, int len) {
    char* tmpfilename = mktemp(strdup("/tmp/pwi2htmlXXXXXX"));
    snprintf (zipFilename, MAXFILENAMELEN - 1, "%s.sxw", tmpfilename);

    zipFile zip = zipOpen(zipFilename, APPEND_STATUS_CREATE);
    zipOpenNewFileInZip (zip, "content.xml", NULL, NULL, 0, NULL,0, "folium vertum", Z_DEFLATED, Z_DEFAULT_COMPRESSION);
    zipWriteInFileInZip (zip, contents, len);
    zipCloseFileInZip(zip);
    zipClose(zip, "Created by pwi2html, (c) Adrian Petru Dimulescu, 2004");
    
    delete tmpfilename;
    
    ifstream* result = new ifstream(zipFilename, ios::binary);
    return result;
}

// char* OpenofficePwiExporter::getMarkup(int code) {
//     switch (code) {
// 	case Paragraph::MARKUP_ITALIC_BEGIN:
// 	    return "<i>";
// 	case Paragraph::MARKUP_ITALIC_END:
// 	    return "</i>";
// 	case Paragraph::MARKUP_BOLD_BEGIN:
// 	    return "<b>";
// 	case Paragraph::MARKUP_BOLD_END:
// 	    return "</b>";
// 
// 	default:
// 	    return "";
//     }
//     return "";
// }


};
