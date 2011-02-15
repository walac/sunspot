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

#ifndef UTILS_HPP
#define UTILS_HPP

#include "highgui.h"
#include <cassert>

#define WND_WIDTH 600
#define WND_HEIGHT 600

inline void create_window(const char *name)
{
    cvNamedWindow(name, 0);
    cvResizeWindow(name, WND_WIDTH, WND_HEIGHT);
}

template<typename T>
int count_pixels(IplImage *img, T val)
{
    int total = 0;

    assert(img->depth == IPL_DEPTH_8U);

    for (int i = 0; i < img->height; ++i) {
        const T *ptr = reinterpret_cast<T*> (img->imageData + i * img->widthStep);

        for (int j = 0; j < img->width; ++j)
            if (static_cast<T> (ptr[j]) == val)
                ++total;
    }

    return total;
}

template<typename T>
void cut(IplImage *img, T threshold, T val = static_cast<T> (255))
{
    int total = 0;

    assert(img->depth == IPL_DEPTH_8U);

    for (int i = 0; i < img->height; ++i) {
        T *ptr = reinterpret_cast<T*> (img->imageData + i * img->widthStep);

        for (int j = 0; j < img->width; ++j)
            if (static_cast<T> (ptr[j]) >= threshold)
                ptr[j] = val;
    }
}

#endif // UTILS_HPP
