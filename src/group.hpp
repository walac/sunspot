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

#ifndef GROUP_HPP
#define GROUP_HPP
#include "highgui.h"

struct struct_sun 
{
	coordinate center;
	int radius;
};

struct group_sunspot {
	int id_group;
	std::vector<unsigned int>  blobids;
};

typedef std::vector<group_sunspot> group_sunspot_vector;

IplImage *circle_sunspots(IplImage *img, blob_collection &blobs);

struct_sun center_sun(IplImage *img, int debug);

group_sunspot_vector count_groups(const struct_sun &sun, blob_collection &blobs, IplImage *img);

void put_group (group_sunspot_vector &groups, unsigned int blobida, unsigned int blobidb);


#endif // UTILS_HPP
