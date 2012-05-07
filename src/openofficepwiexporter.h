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
#ifndef PWIOPENOFFICEPWIEXPORTER_H
#define PWIOPENOFFICEPWIEXPORTER_H

#include "abstractpwiexporter.h"
#include "pwidocument.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

#define MAXFILENAMELEN (256)

namespace pwi {

/**
Exporter in OpenOffice SXW format

@author Adrian Petru Dimulescu
*/
class OpenofficePwiExporter : public AbstractPwiExporter
{
public:
    OpenofficePwiExporter(PwiDocument& doc);

    virtual ~OpenofficePwiExporter();
    
    virtual istream* exportPwi ();

private:
    istream* saveToTmpFileAndReturnIstream(const xmlChar*, int);

    char zipFilename[MAXFILENAMELEN]; // the name of the temp zip file. declared at class level because the destructor deletes it   

    bool isBoldOn;
    bool isItalicOn;
    bool isUnderlineOn;
};

};

#endif
