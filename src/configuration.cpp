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
#include "configuration.h"
#include <getopt.h>
#include <iostream>

using namespace std;

const char* Configuration::OUT_FORMAT_XHTML = "xhtml";
const char* Configuration::OUT_FORMAT_HTML = "html";
const char* Configuration::OUT_FORMAT_TEXT = "text";
const char* Configuration::OUT_FORMAT_SXW = "sxw";


Configuration::Configuration() {
    SHOW_HELP = 0;             // by default do not show help
    OUT_FORMAT = OUT_FORMAT_SXW;      // by default transform to OpenOffice
    OUT_FILENAME = "";         // none, by default
    IN_FILENAME = "";         // none, by default
    

    // don't know how to initialize these static strings in a nicer way. god i want my jdk back :(
//     OUT_FORMAT_XHTML    = "xhtml";
//     OUT_FORMAT_TEXT     = "text";
//     OUT_FORMAT_SXW      = "sxw";

}


Configuration::~Configuration() {}




/*!
    \fn Configuration::parseCommandLine(int argc, char** argv)
 */
void Configuration::parseCommandLine(int argc, char** argv) {
    int ich;

    extern char *optarg;
    extern int optind, opterr, optopt;

    while ((ich = getopt (argc, argv, "hf:i:o:")) != EOF) {
        switch (ich) {
        case 'o':
            this->OUT_FILENAME = optarg;
            break;
        case 'i':
            this->IN_FILENAME = optarg;
            break;
        case 'f':
            this->OUT_FORMAT = optarg;
            break;
        case 'h':
            this->SHOW_HELP = 1;
            break;
        case '?':
            this->SHOW_HELP = 1;
            break;
        default:
            break;
        }
    }

    if (optind < argc) {
        // cout << "non-option ARGV-elements: ";
        // while (optind < argc)
        //    cout << argv[optind++] << " " << endl;

        // if the output filename was not already specified with -f and there are more arguments, 
        // take the first as filename
        if (this->IN_FILENAME.size() == 0) 
            this->IN_FILENAME = argv[optind++];
    }
}
