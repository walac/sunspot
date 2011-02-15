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
#include <stdio.h>
#include "blob.hpp"

#include "cv.h"
#include "highgui.h"

using namespace std;

struct lineBlob
{
    unsigned int min, max;
    unsigned int blobId;
    bool attached;
};

blob_collection detectBlobs(IplImage* frame)
{
    int blobCounter = 0;
    blob_collection blobs;
	
	IplImage* finalFrame;

	finalFrame = cvCloneImage(frame);

    vector< vector<lineBlob> > imgData(frame->width);

    for(int row = 0; row < frame->height; ++row)
    {
        for(int column = 0; column < frame->width; ++column)
        {
            //unsigned char byte = (unsigned char) imgStream.get();
            unsigned char byte = (unsigned char) frame->imageData[(row*frame->width)+ column];

            if(byte == 255)
            {
                int start = column;

                for(;byte == 255; byte = (unsigned char) frame->imageData[(row*frame->width)+ column], ++column);

                int stop = column-1;
                lineBlob lineBlobData = {start, stop, blobCounter, false};

                imgData[row].push_back(lineBlobData);
                blobCounter++;
            }
        }
    }

    /* Check lineBlobs for a touching lineblob on the next row */

    for(int row = 0; row < imgData.size()-1; ++row)
    {
        for(int entryLine1 = 0; entryLine1 < imgData[row].size(); ++entryLine1)
        {
            for(int entryLine2 = 0; entryLine2 < imgData[row+1].size(); ++entryLine2)
            {
                if(!((imgData[row][entryLine1].max < imgData[row+1][entryLine2].min) || (imgData[row][entryLine1].min > imgData[row+1][entryLine2].max)))
                {
                    if(imgData[row+1][entryLine2].attached == false)
                    {
                        imgData[row+1][entryLine2].blobId = imgData[row][entryLine1].blobId;
                        imgData[row+1][entryLine2].attached = true;
                    }
                    else
                    {
                        imgData[row][entryLine1].blobId = imgData[row+1][entryLine2].blobId;
                        imgData[row][entryLine1].attached = true;
                    }
                }
            }
        }
    }

    // Sort and group blobs

    for(int row = 0; row < imgData.size(); ++row)
    {
        for(int entry = 0; entry < imgData[row].size(); ++entry)
        {
            if(blobs.find(imgData[row][entry].blobId) == blobs.end()) // Blob does not exist yet
            {
                blob blobData = {{imgData[row][entry].min, row}, {imgData[row][entry].max, row}, {0,0}};
                blobs[imgData[row][entry].blobId] = blobData;
            }
            else
            {
                if(imgData[row][entry].min < blobs[imgData[row][entry].blobId].min.x)

                    blobs[imgData[row][entry].blobId].min.x = imgData[row][entry].min;

                else if(imgData[row][entry].max > blobs[imgData[row][entry].blobId].max.x)

                    blobs[imgData[row][entry].blobId].max.x = imgData[row][entry].max;

                if(row < blobs[imgData[row][entry].blobId].min.y)

                    blobs[imgData[row][entry].blobId].min.y = row;

                else if(row > blobs[imgData[row][entry].blobId].max.y)

                    blobs[imgData[row][entry].blobId].max.y = row;
            }
			
			for (int pp = imgData[row][entry].min; pp <= imgData[row][entry].max; ++pp) 
			{
				coordinate *p = new coordinate();
				
				p->x = pp;
				p->y = row;
				
				blobs[imgData[row][entry].blobId].listPoints.push_back(*p);
			}
			
        }
    }
	
	// Calculate center
	for(map<unsigned int, blob>::iterator i = blobs.begin(); i != blobs.end(); ++i)
	{
		(*i).second.center.x = (*i).second.min.x + ((*i).second.max.x - (*i).second.min.x) / 2;
		(*i).second.center.y = (*i).second.min.y + ((*i).second.max.y - (*i).second.min.y) / 2;
		
		int size = ((*i).second.max.x - (*i).second.min.x) * ((*i).second.max.y - (*i).second.min.y);
		
		// Print coordinates on image, if it is large enough
		if(size > 800)
		{
			CvFont font;
			cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, 0, 1, CV_AA);
			
			char textBuffer[128];
			
			// Draw crosshair and print coordinates (just for debugging, not necessary for later multi-touch use)
			cvLine(finalFrame, cvPoint((*i).second.center.x - 5, (*i).second.center.y), cvPoint((*i).second.center.x + 5, (*i).second.center.y), cvScalar(0, 0, 153), 1);
			
			cvLine(finalFrame, cvPoint((*i).second.center.x, (*i).second.center.y - 5), cvPoint((*i).second.center.x, (*i).second.center.y + 5), cvScalar(0, 0, 153), 1);
			
			sprintf(textBuffer, "(%d, %d)", (*i).second.center.x, (*i).second.center.y);
			
			cvPutText(finalFrame, textBuffer, cvPoint((*i).second.center.x + 5, (*i).second.center.y - 5), &font, cvScalar(0, 0, 153));
			
			cvRectangle(finalFrame, cvPoint((*i).second.min.x, (*i).second.min.y), cvPoint((*i).second.max.x, (*i).second.max.y), cvScalar(0, 0, 153), 1);
			
			// Show center point
			//cout << "(" << (*i).second.center.x << ", " << (*i).second.center.y << ")" << endl;
		}
	}

    return blobs;
}

