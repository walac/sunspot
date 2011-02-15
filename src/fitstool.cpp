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
#include "fitstool.hpp"
#include <CCfits/CCfits>

using namespace CCfits;

void getDatefit(const char * sfitsfile, std::string &sdate)
{

    fitsfile *fptr;       /* pointer to the FITS file; defined in fitsio.h */
    int status, ii, jj;
    long  fpixel = 1, naxis = 2, nelements, exposure;
    long naxes[2] = { 512, 512 };   /* image is 300 pixels wide by 200 rows */
    short array[512][512];
	
    status = 0;         /* initialize status before calling fitsio routines */
    ffopen(&fptr, sfitsfile, 0, &status);   /* create new file */
	
	char date[20];
	
	char comm[255];
	
	
	ffgkey(fptr, "DATE", date, comm, &status);
	
	ffclos(fptr, &status);
	
	sdate = std::string(date);
}
