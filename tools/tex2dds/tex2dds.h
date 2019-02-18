#pragma once
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
#ifndef TEXT2DDS_H
#define TEXT2DDS_H

struct DDS {
	unsigned __int32 signature;
	unsigned __int32 header;
	unsigned __int32 dwflags;
	unsigned __int32 height;
	unsigned __int32 width;
	unsigned __int32 size;
	unsigned __int32 depth;
	unsigned __int32 mipcount;
	unsigned __int32 skip[11];
	unsigned __int32 ddspf;
	unsigned __int32 pfflags;
	unsigned __int32 pffourCC;
	unsigned __int32 rgbbitcount;
	unsigned __int32 rbitmask;
	unsigned __int32 gbitmask;
	unsigned __int32 bbitmask;
	unsigned __int32 abitmask;
	unsigned __int32 caps;
	unsigned __int32 caps2[4];
};

#endif //TEXT2DDS_H