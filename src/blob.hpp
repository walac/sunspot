/*
 * Copyright 2010-2011
 * Adilson Eduardo Spagiari (e.spagiari@gmail.com)
 * Israel Florentino (learsi@gmail.com)
 * Wander Lairson Costa aka walac (wander.lairson@gmail.com)
 *
 * This is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * version 2 of the License.
 * 
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * source code. If not, see http://www.gnu.org/licenses/.
 */

#ifndef BLOB_HPP
#define BLOB_HPP

#include <map>
#include <list>
#include "highgui.h"

struct coordinate
{
    unsigned int x, y;
    void * data;
};

struct blob
{
    //unsigned int blobId;
    coordinate min, max;

    coordinate center;

    double lat;
    double lon;

    std::vector<coordinate> listPoints;
};

typedef std::map<unsigned int, blob> blob_collection;

blob_collection detectBlobs(IplImage* frame);

#endif // BLOB_HPP
