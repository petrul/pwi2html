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
#ifndef PWIABSTRACTPWIEXPORTER_H
#define PWIABSTRACTPWIEXPORTER_H

#include <string>
#include <iostream>

#include "pwidocument.h"

using namespace std;

namespace pwi {

/**
 * Interface for an Exporter (an Exporter is a class which 
 * can tranform a PWI document to some other format
 * @author Adrian Petru Dimulescu
 *
 */
class AbstractPwiExporter{
protected:
    PwiDocument* doc;

public:
    AbstractPwiExporter();

    ~AbstractPwiExporter();
    
    /**
     * Gets the exported content
     */
    virtual istream* exportPwi() = 0;
    
    //virtual char* getMarkup(int code) = 0;
};

};

#endif
