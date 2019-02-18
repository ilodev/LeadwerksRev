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

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

#include "tex2dds.h"
#include "leadwerks/texture.h"

#ifndef SRCVERSION
#define SRCVERSION "UNKNOWN"
#endif

int error(const char *fname) {
	fprintf(stderr, "Error: Failed to load file \"%s\".\n", fname);
	return -1;
}

int tex2dds(const char *fpath) {
	unsigned int  fsize = 0;
	unsigned char *fptr = 0;
	FILE *file = 0;

#ifdef DEBUG
	fprintf(stderr, "tex2dds (build: %s)\n", SRCVERSION);
#endif

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
	fprintf(stderr, "tex loading   : %s\n", fpath);
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
	if (!strstr("TEX", (const char *)fptr)) {
#ifdef DEBUG
		fprintf(stderr, "file is not tex file\n");
#endif
		if (fptr) free(fptr);
		return error(fpath);
	}

	// Get a texture ptr 
	struct tex_header *texheader = (struct tex_header *)fptr;
#ifdef DEBUG
	fprintf(stderr, "tex size      : %d x %d\n", texheader->width, texheader->height);
	fprintf(stderr, "mipmap count  : %d\n", texheader->mipmaps);
#endif

	// make our dds header
	struct DDS ddsheader;
	memset(&ddsheader, 0, sizeof(struct DDS));
	ddsheader.signature = 0x20534444; // "DDS "
	ddsheader.header    = 0x7c;       // 124
	ddsheader.dwflags   = 0x00000107; // CAPS HEIGHT WIDTH PIXELFORMAT 
	ddsheader.height    = texheader->height;
	ddsheader.width     = texheader->width;
	ddsheader.mipcount  = texheader->mipmaps;
	ddsheader.ddspf     = 0x20;
	ddsheader.pfflags   = 0x04;  // fourCC

	// Time to process the compression
	switch (texheader->compression) {
		// todo: support other compressions: RGB and DXT5nm

		case TEX_FORMAT_RGBA:
			ddsheader.pffourCC = 0x0;        // 
			ddsheader.pfflags  = 0x042;      // RGB | ALPHA
			ddsheader.rgbbitcount = 32;
			ddsheader.rbitmask = 0xFF;
			ddsheader.gbitmask = 0xFF00;
			ddsheader.bbitmask = 0xFF0000;
			ddsheader.abitmask = 0xFF000000;
			break;
		case TEX_FORMAT_DXT1:
			ddsheader.pffourCC = 0x31545844; // DXT1
			break;
		case TEX_FORMAT_DXT3:
			ddsheader.pffourCC = 0x33545844; // DXT3
			break;
		case TEX_FORMAT_DXT5:
			ddsheader.pffourCC = 0x35545844; // DXT5
			break;
		default:
			fprintf(stderr, "Unsupported compression type: 0x%x - inform the developer\n", texheader->compression);
			if (fptr) free(fptr);
			return error(fpath);
			break;
	};

	if (texheader->mode == TEX_MODE_IMAGE)
		ddsheader.caps = 0x1000; // dwcaps.TEXTURE

	// todo: check cubemap flag on tex file.






	// Finished building the DDS file, prepare output
	unsigned char *ofname = (unsigned char *)calloc(strlen(fpath) + 1, sizeof(char));
	memcpy(ofname, fpath, strlen(fpath));
	int offset = strlen(fpath) - 3;
	ofname[offset + 0] = 'd';
	ofname[offset + 1] = 'd';
	ofname[offset + 2] = 's';
#ifdef DEBUG
	fprintf(stderr, "dst file      : %s\n", ofname);
#endif

	// the previous file handle is free, we can use it safely
	file = fopen((const char *)ofname, "w+b");
	if (!file) {
#ifdef DEBUG
		fprintf(stderr, "can't open file for saving\n");
#endif
		if (fptr) free(fptr);
		if (ofname) free(ofname);
		return error(fpath);
	}


	// write the header to the dds file
	fwrite(&ddsheader, 1, sizeof(struct DDS), file);
	
	// now loop the mipmaps and save the data to file
	unsigned char *memptr = (unsigned char *)fptr + sizeof(struct tex_header);
	for (int count = 0; count < texheader->mipmaps; count++) {
		struct tex_mipmap *mipmap = (struct tex_mipmap *)memptr;
#ifdef DEBUG
		fprintf(stderr, "mip: %d (%d x %d): %d bytes\n", count, mipmap->height, mipmap->width, mipmap->size);
#endif
		memptr += sizeof(struct tex_mipmap);
		memptr += fwrite(memptr, sizeof(char), mipmap->size, file);
	}

	// finished saving the DDS.
	fclose(file);

	// free the ptr
	if (fptr) free(fptr);
	if (ofname) free(ofname);

	fprintf(stderr, "Successfully converted \"%s\".\n", fpath);

	return 1;
}

int main(int argc, char **argv) {

#ifdef DEBUG
	fprintf(stderr, "tex2dds (build: %s)\n", SRCVERSION);
#endif

	// process all inputs
	unsigned int count = 1;
	while (count < argc)
		tex2dds(argv[count++]);

	return 1;
}
