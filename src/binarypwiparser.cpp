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
#include "binarypwiparser.h"
#include "paragraph.h"
#include <iostream>
#include <stdint.h>
#include <fstream>
#include <sstream>

#include <qtextcodec.h>
#include <assert.h>

namespace pwi {


BinaryPwiParser::BinaryPwiParser(istream& is) {
    this->inputStream = &is;
}


BinaryPwiParser::~BinaryPwiParser() {}



/*!
    \fn pwi::BinaryPwiParser::parse(PwiDocument& doc)
 */
void BinaryPwiParser::parse(PwiDocument& doc) throw (runtime_error) {

    //
    // Header
    //

    char magic[PWI_MAGIC_SIZE];
    this->inputStream->read(magic, PWI_MAGIC_SIZE);

    if (0 != memcmp(magic, PWI_MAGIC, PWI_MAGIC_SIZE)) {
        throw (runtime_error ("pwi document does not start with pwi string"));
    }

    // Skip to font count
    this->inputStream->seekg(0x13, std::ios_base::cur);

    unsigned font_count = read16();

#if VERBOSE
    cerr << "Font count: " << dec << font_count << endl;
#endif

    // Skip to font table
    this->inputStream->seekg(0xa, std::ios_base::cur);

#if VERBOSE
    std::cerr << "Font table starts at offset 0x" << hex << input.tellg() << std::endl;
#endif

    // Skip font table
    this->inputStream->seekg(font_count * FONT_ENTRY_SIZE, std::ios_base::cur);

#if VERBOSE

    cerr << "Font table ends at offset 0x" << hex << input.tellg() << endl;
#endif
    // Skip to paragraph count 1
    this->inputStream->seekg(0xba, std::ios_base::cur);
    unsigned paragraph_count = read16();

    // Skip to pa  input.seekg(2, ios_base::cur);ragraph count 2
    this->inputStream->seekg(0x2, std::ios_base::cur);
    /*unsigned paragraph_count_2 =*/
    read16();

    // Skip to paragraph count 3
    this->inputStream->seekg(0x6, std::ios_base::cur);
    /*unsigned paragraph_count_3 =*/
    read16();

#if VERBOSE
    std::cerr << "Paragraph count: " << std::dec <<
    paragraph_count /*<< " = " <<
                              paragraph_count_2 << " = " << 
                              paragraph_count_3*/ << endl;
#endif

    this->inputStream->seekg(0x6, std::ios_base::cur);

    //cerr << "Paragraph index starts at offset 0x" << hex << input.tellg() << endl;

#if 0

    for (unsigned i = 0; i < paragraph_count; i++) {
        cerr.fill(' ');
        cerr.width(2);
        cerr << dec << i << ": ";
        cerr.fill('0');
        for (int j = 0; j < 4; j++) {
            cerr.width(4);
            cerr << hex << read16(input) << ' ';
        }
        cerr << endl;
    }
#else
    this->inputStream->seekg(paragraph_count * PARAGRAPH_ENTRY_SIZE, std::ios_base::cur);
#endif

    //cerr << "Paragraph index ends at offset 0x" << hex << input.tellg() << endl;

    this->inputStream->seekg(2, std::ios_base::cur);

    if (0x0 != read16())
#if VERBOSE
        std::cerr << "Not 0x0!" << std::endl
#endif
    ;

    for (unsigned i = 0; i < paragraph_count; i++) {
#if VERBOSE
        std::cerr << "Paragraph " << std::dec << i << " starts at offset 0x" << std::hex << this->inputStream->tellg() << std::endl;
#endif

        string crtString;
        unsigned code = read16();

        streampos start_offset = this->inputStream->tellg();
        
        Paragraph* p = NULL;
        switch (code) {
        case 0x41:
            p = new Paragraph();
            text(p);
            doc.addParagraph(p);

            align(start_offset);

            code = read16();
            if (code != 0x42) {
                ostringstream tmpstr;
                tmpstr << "Not 0x42 but 0x" << std::hex << code << " at offset 0x"
                    << std::hex << ((unsigned)this->inputStream->tellg() - 2) << std::endl;
                throw runtime_error (tmpstr.str());
            }

            start_offset = this->inputStream->tellg();
            this->inputStream->seekg(0xe, std::ios_base::cur);
            break;

        case 0x43:
            drawing();
            break;

        default:
#if VERBOSE
            cerr << "Ignoring unknown code 0x" << hex << code << " at 0x" <<
            (this->inputStream->tellg() - (streampos)2) << endl;
#endif
            //abort();
            i--; // as this is a strange character, it is not really a paragraph so it doesn't count as one
            break;
        }

        // this->inputStream->seekg(4 - ((this->inputStream->tellg() - start_offset) & 3), std::ios_base::cur); // FIXME: what is this ?

#if VERBOSE
        cerr << "Paragraph " << dec << i << " ends at offset 0x" << hex << input.tellg() << endl;
#endif

    }


    if (0x82 != read16())
#if VERBOSE
        std::cerr << "Not 0x82!" << std::endl
#endif
    ;

#if VERBOSE
    cerr << "Decoding ends at offset 0x" << hex << input.tellg() << endl;
#endif

}

unsigned BinaryPwiParser::read16() {
    unsigned value = this->inputStream->get();
    value |= (unsigned)this->inputStream->get() << 8;
    return value;
}




/*!
    \fn pwi::BinaryPwiParser::text()
 */
void BinaryPwiParser::text(Paragraph* str) {
    // TODO:change QTextCodec this with something more standard from glibc
    QTextCodec* latin1Codec = QTextCodec::codecForName("ISO8859-1");
    assert(latin1Codec != NULL);
    QTextCodec* utf8Codec = QTextCodec::codecForName("utf8");
    assert(utf8Codec != NULL);

    this->inputStream->seekg(0x4, std::ios_base::cur);

    /*unsigned plain_text_size      =*/
    read16();
    unsigned total_size           = read16();

    //cerr << "Total sizeBinaryPwiParser::: 0x" << hex << total_size << endl;

    this->inputStream->seekg(0x14, std::ios_base::cur);

    uint8_t* data = new uint8_t[total_size];

    unsigned paragraph_data_offset = this->inputStream->tellg();
    this->inputStream->read((char*)data, total_size);

    
    QString unicodeString;
    QCString utf8Encoded;
    
    for (unsigned j = 0; j < total_size; j++) {
        if (data[j] == 0x00) {
            cerr << "Warning, data byte is NULL!" << std::endl;
        } else if (data[j] & 0x80) {
            switch (data[j]) {
            case 0xe5:    // Select font
                j+=2;
                break;

            case 0xe6:    // Font size
                j+=2;
                break;

            case 0xe7:    // Color
                j+=2;
                break;

            case 0xe8:    // Font weight: 0xe80700 = boldon; 0xe80400 = bold off
		j++;
		if (data[j] == 0x04)
		    str -> endMarkupBold();
		else
		    str -> startMarkupBold(); // it should normally be 0x07

//                 cerr << "Bold code at offset 0x" << hex << (paragraph_data_offset + j);
//                 cerr << "; argument: 0x" << hex << (int) data[j];
                j++;
// 		cerr << "; argument: 0x" << hex << (int) data[j] << endl;
                break;

            case 0xe9:    // Italic
//                cerr << "Italic code at offset 0x" << hex << (paragraph_data_offset + j);
                j++;
//                cerr << "; argument: 0x" << hex << (int) data[j] << endl;
		if (data[j] == 0)
		    str -> endMarkupItalic();
		else
		    str -> startMarkupItalic();
		    
                break;

            case 0xea:    // Underline
                j++;
                break;

            case 0xeb:    // Strikethrough
                j++;
                break;

            case 0xec:    // Highlight
                j++;
                break;

            case 0xef:    // XXX unknown
                j+=3;
                break;

            case 0xc1:    // Character code
                j++;
                
                unicodeString   = latin1Codec -> toUnicode((char*)(data + j), 1);
                utf8Encoded     = utf8Codec   -> fromUnicode(unicodeString);
                *str += utf8Encoded;

                break;

            case 0xc2:    // Unknown
                j+=2;
                break;

            case 0xc4:    // Control codeunicodeString   = latin1Codec -> toUnicode((char*)(data + j), 1);
                j++;
                switch (data[j]) {
                case 0x00:   // End of paragraph
                    break;

                case 0x04:   // Tab character
                    *str += "\t"; // FIXME: maybe we should use a real tab here
                    break;

                case 0x19:  // XXX unknown
                    j+=2;
                    break;

                case 0x1b:  // XXX unknown
                    j+=6;
                    break;

                default:
                    unicodeString   = latin1Codec -> toUnicode((char*)(data + j), 1);
                        cerr << "Ignoring unknown control char: 0x" << hex << (unsigned)data[j] <<
                        " at offset 0x" << paragraph_data_offset + j << endl;
                    break;
                }
                break;

            case 0xc5:  // Reference to embedded data?
                j+=2;
                break;

            case 0xf1:
                j+=7;
                break;

            default:
                //if ((data[j] & 0xc0) == 0xc0) {  // Latin extended characters are here
                    int firstChar = data[j++];
                    int secondChar = data[j];

                    cerr << "first 0x" << hex << (int)firstChar << "  -  second 0x" << hex << (int)secondChar << endl;

                    int transformed = secondChar << 6 | (firstChar & ~0xc0);

                    cerr << "transformed 0x" << hex << (int)transformed << endl;
                    
//                     char unicodeChar[3];
//                     unicodeChar[0] = (transformed & 0xFF00);
//                     unicodeChar[1] = (transformed & 0x00FF);
//                     unicodeChar[2] = '\0';

                    QChar qch = transformed;
                    
                    unicodeString = qch;
                    utf8Encoded     = utf8Codec   -> fromUnicode(unicodeString);
                    *str += utf8Encoded;

/*                } else
                    cerr << "Unknown code: 0x" << std::hex << (unsigned)data[j] <<
                    " at offset 0x" << paragraph_data_offset + j << std::endl;*/
                //abort();
            }
        } else {
            *str += (char) (data[j]);
        }
    }

    //std::cout << std::endl;

    delete[] data;

}


/*!
    \fn pwi::BinaryPwiParser::drawing()
 */
void BinaryPwiParser::drawing() {
    // Skip unknown
    this->inputStream->seekg(0xc, std::ios_base::cur);

    unsigned drawing_size = read16();

    std::cerr << "Drawing: 0x" << std::hex << drawing_size <<
    " bytes at offset 0x" << std::hex << this->inputStream->tellg() << std::endl;

#if 0
    // Skip drawing
    input.seekg(drawing_size, ios_base::cur);
#else

    char filename[32];
    snprintf(filename, sizeof(filename), "drawing-%04x.bin", (unsigned)this->inputStream->tellg());
    std::ofstream output(filename, std::ofstream::binary);

    char* drawing = new char[drawing_size];

    this->inputStream->read(drawing, drawing_size);
    output.write(drawing, drawing_size);
#endif
}


/*!
    \fn pwi::BinaryPwiParser::align(std::streampos start_offset)
 */
void BinaryPwiParser::align(std::streampos start_offset) {
    //cerr << "Start offset: 0x" << hex << start_offset << endl;
    //cerr << "Current offset: 0x" << hex << input.tellg() << endl;
    unsigned align = 4 - ((this->inputStream->tellg() - start_offset) & 3);
    //cerr << "Align: " << dec << align << endl;
    this->inputStream->seekg(align, std::ios_base::cur);

}

}
;
