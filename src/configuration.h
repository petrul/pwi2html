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
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>

using namespace std;

/**
 * @author Adrian Petru Dimulescu
 */
class Configuration{
public:
    Configuration();

    ~Configuration();
    void parseCommandLine(int argc, char** argv);
    
    int SHOW_HELP;           // turned on by -h
    string IN_FILENAME;      // -i option, the input filename (psw)
    string OUT_FILENAME;     // -o option, the output filename
    string OUT_FORMAT;       // -f format
    
    static const char* OUT_FORMAT_XHTML;
    static const char* OUT_FORMAT_HTML;
    static const char* OUT_FORMAT_TEXT;
    static const char* OUT_FORMAT_SXW;
};

#endif
