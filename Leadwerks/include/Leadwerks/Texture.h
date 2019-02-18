#pragma once
/*
 * This file is part of the LeadwerksRev distribution
 * https://github.com/ilodev/LeadwerksRev or http://LeadwerksRev.github.io).
 * Copyright (c) 2018 Inaki Lopez
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

#ifndef LEADWERKS_TEX
#define LEADWERKS_TEX

enum TEX_FORMAT {
	TEX_FORMAT_UNKNOWN = 0,
	TEX_FORMAT_RGBA = 1,
	TEX_FORMAT_DXT1 = 8,
	TEX_FORMAT_DXT3 = 5,
	TEX_FORMAT_DXT5 = 6,
	TEX_FORMAT_RGB = 7,
	TEX_FORMAT_DXT5n = 20
};

enum TEX_MODE {
	TEX_MODE_UNKNOWN = 0,
	TEX_MODE_IMAGE = 2,
	TEX_MODE_CUBEMAP = 4,
	TEX_MODE_VOLUME = 3,
};

enum TEX_FILTERMODE {
	TEX_FILTERMODE_PIXEL = 0,
	TEX_FILTERMODE_SMOOTH = 1
};

struct tex_header{
	unsigned char magic[4];
	unsigned int unknown04; // found 1
	enum TEX_FORMAT compression;
	enum TEX_MODE mode;
	unsigned int width;
	unsigned int height;
	unsigned int unknown18; // found 1
	enum TEX_FILTERMODE filter;
	float clampx;
	float clampy;
	float clampz;
	unsigned int unknown2c; // found 1
	unsigned int mipmaps; // mips?
};

struct tex_mipmap {
	unsigned int width;
	unsigned int height;
	unsigned int size; // total size in bytes of the mipmap
//	unsigned char data[size]; //not included in the struct for file reading purposes
};

#endif //LEADWERKS_TEX
