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

#include "detect.hpp"
#include "blob.hpp"
#include "group.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <getopt.h>
#include <unistd.h>
#include "fitstool.hpp"
#include "highgui.h"
#include "cv.h"
#include "utils.hpp"

int debug = 0;

int main(int argc, char **argv)
{
    create_window("Original");
    create_window("Result");

    cvMoveWindow("Original", 10, 200);
    cvMoveWindow("Result", 20 + WND_WIDTH, 200);

    const char *optstr = "d";
    int ch;

    while ((ch = getopt(argc, argv, optstr)) != EOF) {
        switch (ch)
        {
            case 'd':
                debug = 1;
                break;
        }
    }

    if ((argc - optind) > 2) {

        std::ofstream myfile;
        myfile.open ("Wolfs.txt");

        for (int i = optind; i < argc; ++i) {
            IplImage *src = cvLoadImage(argv[i]);
            if (debug)
                cvShowImage("Original", src);
            IplImage *dst = detect_sunspots(src);

            blob_collection b = detectBlobs(dst);

            std::cout << "Found " << b.size() << " sunspots in image " << argv[i] << std::endl; 

            struct_sun sun = center_sun(src, debug);

            std::cout << "X0 = " << sun.center.x << " Y0 = " << sun.center.y << " Radius = " << sun.radius << std::endl;

            group_sunspot_vector groups = count_groups(sun, b, debug? dst : NULL);

            std::cout << "groups found:" << groups.size() << std::endl;

            std::cout << "Wolf number " << b.size() + (groups.size() * 10) << " sunspots in image " << argv[i] << std::endl; 

            cvShowImage("Original", src);
            cvShowImage("Result", dst);
            cvWaitKey(3000);
            cvReleaseImage(&src);
            cvReleaseImage(&dst);

        }
    } else {
        IplImage *src = cvLoadImage(argv[optind]);
        if (debug)
            cvShowImage("Original", src);
        IplImage *dst = detect_sunspots(src);

        blob_collection b = detectBlobs(dst);

        std::cout << "Found " << b.size() << " sunspots in image " << argv[optind] << std::endl; 

        struct_sun sun = center_sun(src, debug);

        std::cout << "X0 = " << sun.center.x << " Y0 = " << sun.center.y << " Radius = " << sun.radius << std::endl;

        group_sunspot_vector groups = count_groups(sun, b, debug? dst : NULL);

        std::cout << "groups found:" << groups.size() << std::endl;

        std::cout << "Wolf number: " << b.size() + (groups.size() * 10) << " sunspots in image " << argv[optind] << std::endl; 

        if (!debug) {
            cvShowImage("Original", src);
            cvShowImage("Result", dst);
        }

        while (27 == cvWaitKey(0) & 0xff);

        cvDestroyAllWindows();
        cvReleaseImage(&src);
        cvReleaseImage(&dst);

    }

    return 0;
}

