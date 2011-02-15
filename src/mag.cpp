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

#include <cassert>
#include <utility>
#include <cmath>
#include "mag.hpp"

using namespace std;

namespace {
typedef unsigned char pixel_type;
pixel_type max_value(IplImage *img, const blob &b)
{
    assert(img->depth == IPL_DEPTH_8U);
    pixel_type max_val = 0;

    typedef std::vector<coordinate>::const_iterator iter_t;

    for (iter_t it = b.listPoints.begin(); it != b.listPoints.end(); ++it) {
        const pixel_type *ptr = reinterpret_cast<pixel_type*> (img->imageData + it->y * img->widthStep);
        if (ptr[it->x] > max_val)
            max_val = ptr[it->x];
    }

    return max_val;
}
pixel_type min_value(IplImage *img, const blob &b)
{
    assert(img->depth == IPL_DEPTH_8U);
    pixel_type min_val = 255;

    typedef std::vector<coordinate>::const_iterator iter_t;

    for (iter_t it = b.listPoints.begin(); it != b.listPoints.end(); ++it) {
        const pixel_type *ptr = reinterpret_cast<pixel_type*> (img->imageData + it->y * img->widthStep);
        if (ptr[it->x] < min_val)
            min_val = ptr[it->x];
    }

    return min_val;
}
}

int num_sunspot_by_mag(IplImage *mag, const blob_collection &b)
{
    int total = 0;

    typedef blob_collection::const_iterator iter_t;

    for (iter_t it = b.begin(); it != b.end(); ++it) {
        int max_val = max_value(mag, it->second);
        int min_val = min_value(mag, it->second);
        max_val -= 128;
        min_val -= 128;
        if (max(abs(max_val), abs(min_val)) >= 75)
            ++total;
    }
    return total;
}
