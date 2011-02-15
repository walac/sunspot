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

#include "cv.h"
#include "detect.hpp"
#include "utils.hpp"
#include <stdio.h>

const int INIT_SE_SIZE = 3;

#define APPLY_POST_PROCESSING
//#define POST_USE_EROSION

#ifdef APPLY_POST_PROCESSING
#define DIFF 40
#else
#define DIFF 10
#endif

extern int debug;

namespace {
typedef unsigned char pixel_type;

IplConvKernel *create_se(int se_size, int shape = CV_SHAPE_ELLIPSE)
{
    int half;
   
    if (se_size & 1)
        half = (se_size - 1) / 2;
    else
        half = se_size / 2;

    return cvCreateStructuringElementEx(se_size, se_size,
                            half, half, shape, NULL);
}

IplImage *threshold(IplImage *img)
{
    IplImage *dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    int diff = 0;
    int n_prev = 0;
    int thres = 0;
    IplImage *prev = 0;

    do {
        if (prev)
            cvReleaseImage(&prev);

        prev = cvCloneImage(dst);
        cvThreshold(img, dst, thres, 255, CV_THRESH_BINARY);
        //printf("\tThreshold = %d\n", thres);
        if (debug) {
            cvShowImage("Result", dst);
            cvWaitKey(50);
        }

        int n = count_pixels<pixel_type>(dst, 0);
        diff = n - n_prev;
        n_prev = n;
        ++thres;

    } while (diff > DIFF);

    cvReleaseImage(&dst);
    return prev;
}

}

IplImage *detect_sunspots(IplImage *img)
{
    int se_size = INIT_SE_SIZE;
    int n_prev;
    int n = 0;

    IplImage *src;

    if (IPL_DEPTH_8U != img->depth || 1 != img->nChannels) {
        src = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
        cvConvertImage(img, src);
    } else {
        src = cvCloneImage(img);
    }

    IplImage *dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
    IplImage *prev = 0;
    //cvSmooth(src, src, CV_GAUSSIAN, 3);
    //IplConvKernel *k = create_se(3);
    //cvErode(src, src, k);
    //cvEqualizeHist(src, src);
    //cvThreshold(src, src, 100, 255, CV_THRESH_TRUNC);
    //cut<pixel_type>(src, 120);
    //cvShowImage("Original", src);

    do {
        if (prev)
            cvReleaseImage(&prev);

        prev = cvCloneImage(dst);
        n_prev = n;

        IplConvKernel *se = create_se(se_size);
        cvMorphologyEx(src, dst, NULL, se, CV_MOP_BLACKHAT);
        //printf("SE_SIZE = %d\n", se_size);
        cvReleaseStructuringElement(&se);

        IplImage *tmp = threshold(dst);
        cvReleaseImage(&dst);
        dst = tmp;

        n = count_pixels<pixel_type>(dst, 255);
        se_size += 2;
    } while (n > n_prev);

    cvReleaseImage(&dst);
    dst = prev;

#ifdef APPLY_POST_PROCESSING
    IplConvKernel *kernel = create_se(2);
#ifdef POST_USE_EROSION
    cvErode(dst, dst, kernel);
#else
    cvMorphologyEx(dst, dst, NULL, kernel, CV_MOP_OPEN, 1);
    //cvErode(dst, dst, kernel2);
#endif
    cvReleaseStructuringElement(&kernel);
#endif

    cvReleaseImage(&src);

    return dst;
}

