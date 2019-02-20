/*
 * This file is part of the LeadwerksRev distribution
 * https://github.com/ilodev/LeadwerksRev or http://LeadwerksRev.github.io).
 * Copyright (c) 2019 Inaki Lopez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

//https://github.com/KellanHiggins/UnityFBXExporter/blob/master/Assets/Packages/UnityFBXExporter/FBXExporter.cs
//https://banexdevblog.wordpress.com/2014/06/23/a-quick-tutorial-about-the-fbx-ascii-format/
//https://www.ics.uci.edu/~djp3/classes/2014_03_ICS163/tasks/arMarker/Unity/arMarker/Assets/CactusPack/Meshes/Sprites/Rock_Medium_SPR.fbx
//http://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_8A69CD20_B8FB_4B23_925A_372E408390AA_htm

//http://www.martinreddy.net/gfx/3d/OBJ.spec
//https://en.wikipedia.org/wiki/Wavefront_.obj_file


#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

#include "mdl2fbx.h"
#include "leadwerks/model.h"

#ifndef SRCVERSION
#define SRCVERSION "UNKNOWN"
#endif

int error(const char *fname) {
	fprintf(stderr, "Error: Failed to load file \"%s\".\n", fname);
	return -1;
}

int main(int argc, char **argv) {
	const char   *fpath = 0;
	unsigned int  fsize = 0;
	unsigned char *fptr = 0;
	FILE *file = 0;

#ifdef DEBUG
	fprintf(stderr, "mdl2fbx (build: %s)\n", SRCVERSION);
#endif

	// Set the argument as file name
	if (argc > 1)
		fpath = argv[1];

	// check the file exists
	struct stat st;
	if (stat(fpath, &st) == -1 || st.st_size < sizeof(struct tex_header)) {
#ifdef DEBUG
		fprintf(stderr, "file not present\n");
#endif
		return error(fpath);
	}
	fsize = st.st_size;
#ifdef DEBUG
	fprintf(stderr, "mdl loading   : %s\n", fpath);
#endif

	// remark: we are reading binary data
	file = fopen(fpath, "r+b");
	if (!file) {
#ifdef DEBUG
		fprintf(stderr, "can't open file for reading\n");
#endif
		return error(fpath);
	}

	// Load the file contents to memory
	fptr = (unsigned char *)calloc(fsize, sizeof(char));
	int bread = fread(fptr, sizeof(char), fsize, file);
	fclose(file);

	if (bread != fsize) {
#ifdef DEBUG
		fprintf(stderr, "data read failed %d %d\n", bread, fsize);
#endif
		return error(fpath);
	}

	// Assert file is a tex file and we can at least read the tex header
	// todo: need this to match the file header
	if (fptr[0] != 1)) {
#ifdef DEBUG
		fprintf(stderr, "file is not mdl file\n");
#endif
		return error(fpath);
	}

	// build output file name
	unsigned char *ofname = (unsigned char *)calloc(strlen(fpath) + 1, sizeof(char));
	memcpy(ofname, fpath, strlen(fpath));
	int offset = strlen(fpath) - 3;
	ofname[offset + 0] = 'f';
	ofname[offset + 1] = 'b';
	ofname[offset + 2] = 'x';
#ifdef DEBUG
	fprintf(stderr, "dst file      : %s\n", ofname);
#endif

	// the previous file handle is free, we can use it safely
	file = fopen((const char *)ofname, "w+b");
	if (!file) {
#ifdef DEBUG
		fprintf(stderr, "can't open file for saving\n");
#endif
		return error(fpath);
	}




	// DO STUFF HERE
	fprintf(stderr, "Not yet implemented.\n\n");
	exit(-1);





	// finished saving the FBX.
	fclose(file);

	// free the ptr
	if (fptr) free(fptr);
	if (ofname) free(ofname);

	fprintf(stderr, "Successfully converted \"%s\".\n", fpath);

	return 1;
}