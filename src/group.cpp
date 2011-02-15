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

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <cmath>
#include "blob.hpp"
#include "group.hpp"
#include "cv.h"
#include "highgui.h"
#include "utils.hpp"

#define SUNRADIUS_6GRAUS 12
#define DELTA 3
#define THRESHOLD 2
#define PI 3.14159265

using namespace std;

typedef unsigned char pixel_type;

IplImage *circle_sunspots(IplImage *img, blob_collection &blobs)
{
    IplImage *dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);

    //dst = cvCloneImage( img );

    cvCvtColor( img, dst, CV_GRAY2RGB );

    blob_collection::const_iterator iter;

    for (iter = blobs.begin(); iter != blobs.end(); ++iter) 
    {
        std::cout << "x:" << iter->second.center.x << " y:" <<iter->second.center.y << std::endl;
        cvCircle(dst, cvPoint(iter->second.center.x, iter->second.center.y), SUNRADIUS_6GRAUS , cvScalar(0,255,0), 1);

        std::vector<coordinate>::const_iterator iterl;
        for (iterl = iter->second.listPoints.begin(); iterl != iter->second.listPoints.end(); ++iterl) 
        {
            std::cout << "\tx:" << iterl->x << " y:" <<iterl->y << std::endl;
        }

    }

    return dst;
}


struct_sun center_sun(IplImage *img, int debug)
{
    int x0 = 0, y0 = 0, ido = 1;
    int radius = 0;

    x0 = img->width / 2;
    y0 = img->height / 2;

    int xd = 0, xe =0, yh=0, yl=0;


    radius = ((y0 + x0)/2)-10;

    if (debug)
        std::cout << "X0 = " << x0 << " Y0 = " << y0 << " Radius" << radius << std::endl;

    IplImage *src;

    //define centro

    while (ido) 
    {

        ido = 0;

        xd = 0; 
        xe = 0;
        yh = 0;
        yl = 0;

        if (IPL_DEPTH_8U != img->depth || 1 != img->nChannels) {
            src = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
            cvConvertImage(img, src);
        } else {
            src = cvCloneImage(img);
        }

        cvCircle(src, cvPoint(x0, y0), radius , cvScalar(255,255,255), 1);

        //x Direita;
        for (int i= (radius - 2) + x0; i >= x0; --i) 
        {
            const pixel_type p = (src->imageData + y0 * src->widthStep)[i];
            ++xd;
            if (p > THRESHOLD) 
            {
                break;
            }
        }

        //x esquerda;
        for (int i= x0 - (radius - 2) ; i <= x0; ++i) 
        {
            const pixel_type p = (src->imageData + y0 * src->widthStep)[i];
            ++xe;
            if (p > THRESHOLD) 
            {
                break;
            }
        }

        //y cima;
        for (int i= y0 - (radius - 2) ; i <= x0; ++i) 
        {
            const pixel_type p = (src->imageData + i * src->widthStep)[x0];
            ++yh;

            if (p > THRESHOLD)
            {
                break;
            }
        }

        //y baixo;
        for (int i= (radius - 2) + y0; i >= y0; --i) 
        {
            const pixel_type p = (src->imageData + i * src->widthStep)[x0];
            ++yl;

            if (p > THRESHOLD)
            {
                break;
            }


        }

        if ( abs(xd - xe) > DELTA) 
        {
            ido = 1;

            x0 = x0 + ((xd - xe)/2);

            if (debug)
                std::cout << "entrou x "  << abs(xd - xe) << "x0 = " << x0 << std::endl;

        }

        if ( abs(yh - yl) > DELTA)
        {
            ido = 1;

            y0 = y0 + ((yh - yl)/2);

            if (debug)
                std::cout << "entrou y"  << abs(yh - yl) << "y0 = " << y0 << std::endl;			
        }

        if (debug)
        {
            std::cout << "xd = " << xd << " xe = " << xe << std::endl;
            std::cout << "yh = " << yh << " yl = " << yl << std::endl << std::endl;
        }

        cvReleaseImage(&src);
    }

    radius = radius - ((yh + yl));

    if (debug)
    {

        if (IPL_DEPTH_8U != img->depth || 1 != img->nChannels) {
            src = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
            cvConvertImage(img, src);
        } else {
            src = cvCloneImage(img);
        }

        cvCircle(src, cvPoint(x0, y0), radius , cvScalar(255,255,255), 1);

        cvShowImage("Original", src);

        cvReleaseImage(&src);
    }

    struct_sun ss = {{x0,y0},radius};

    return ss;

}

void put_group (group_sunspot_vector &groups, unsigned int blobida, unsigned int blobidb)
{
    group_sunspot_vector::iterator itera;
    group_sunspot_vector::iterator iterb;
    vector<unsigned int>::size_type i;
    vector<unsigned int>::iterator p;
    bool finda = false, findb = false;

    for (itera = groups.begin(); itera != groups.end(); ++itera) 
    {
        p = find(itera->blobids.begin(), itera->blobids.end(), blobida);

        if (p != itera->blobids.end()) 
        {
            finda = true;
            break;
        }
    }

    for (iterb = groups.begin(); iterb != groups.end(); ++iterb) 
    {
        p = find(iterb->blobids.begin(), iterb->blobids.end(), blobidb);

        if (p != iterb->blobids.end()) 
        {
            findb = true;
            break;
        }
    }

    if (!finda && !findb) 
    {
        if (blobida == blobidb)
        {
            i = groups.size();
            group_sunspot gs = {i};
            groups.push_back(gs);
            groups[i].blobids.push_back(blobida);
        }
        else 
        {
            i = groups.size();
            group_sunspot gs = {i};
            groups.push_back(gs);
            groups[i].blobids.push_back(blobida);
            groups[i].blobids.push_back(blobidb);		

        }
    }

    if (finda && !findb) 
    {
        itera->blobids.push_back(blobidb);		
    }

    if (!finda && findb) 
    {
        iterb->blobids.push_back(blobida);		
    }

    if (finda && findb) 
    {
        if (itera->id_group != iterb->id_group)
        {
            vector<unsigned int>::iterator iterblob;

            for (iterblob = iterb->blobids.begin(); iterblob != iterb->blobids.end(); ++iterblob) 
            {
                itera->blobids.push_back(*iterblob);
            }

            groups.erase(iterb);

        }

    }

}

group_sunspot_vector count_groups(const struct_sun &sun, blob_collection &blobs, IplImage *img)
{
    int x=0, y=0;

    blob_collection::iterator iter;

    group_sunspot_vector *groups = new group_sunspot_vector();

    for (iter = blobs.begin(); iter != blobs.end(); ++iter) 
    {
        x = iter->second.center.x - sun.center.x;
        y = iter->second.center.y - sun.center.y;

        double lat=asin(((double(y)/sun.radius) ) );
        double lon=asin(((double(x)/sqrt(( pow(double(sun.radius),2.0) - pow(y , 2.0)) ))));

        if (img != NULL)
            cout << iter->first << " lat:" << lat << "\tLon:" << lon << endl;

        iter->second.lat = lat;
        iter->second.lon = lon;

    }

    blob_collection::const_iterator citera;
    for (citera = blobs.begin(); citera != blobs.end(); ++citera) 
    {
        IplImage *dst;
        if (img != NULL) 
        {
            dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
            cvCvtColor( img, dst, CV_GRAY2RGB );

            cvCircle(dst, cvPoint(citera->second.center.x, citera->second.center.y), SUNRADIUS_6GRAUS , cvScalar(0,255,0), 1);

            cvShowImage("Result",dst);
        }

        put_group(*groups, citera->first, citera->first);

        blob_collection::const_iterator citerb;
        for (citerb = blobs.begin(); citerb != blobs.end(); ++citerb)
        {
            if (citera->first != citerb->first) 
            {				
                double coslamb = sin(citera->second.lat) * sin(citerb->second.lat) + 
                    cos(citera->second.lat)*cos(citerb->second.lat) * 
                    cos(citerb->second.lon - citera->second.lon);
                double lamb = acos(coslamb);

                if (lamb <= ((6.0/180.0)*PI)) 
                {
                    put_group(*groups, citera->first, citerb->first);
                    if (img !=NULL)
                        cvCircle(dst, cvPoint(citerb->second.center.x, citerb->second.center.y), SUNRADIUS_6GRAUS , cvScalar(255,0,0), 1);					
                }
                else
                {
                    if (img !=NULL)
                        cvCircle(dst, cvPoint(citerb->second.center.x, citerb->second.center.y), SUNRADIUS_6GRAUS , cvScalar(255,255,255), 1);
                }
                if (img !=NULL)
                {
                    cvShowImage("Result",dst);
                    cvWaitKey(10);
                }
            }
        }
        if (img !=NULL)
            cvReleaseImage(&dst);
    }
    return *groups;
}
