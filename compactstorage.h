/*
 * CompactStorage
 *
 * A C++ library for creating a compact bitstream made up of multiple
 * different variables of variable length overlapping to save space.
 *
 * Copyright (C) 2012 Franz Liedke
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <fstream>
#ifndef _COMPACTSTORAGE_H
#define _COMPACTSTORAGE_H

class CompactStorage
{
	public:
		CompactStorage(int numberOfBytes = 1);
		~CompactStorage();

		void dump();
		void dump(std::fstream* fd);

		void writeInt(int value, int bits);
		void writeBool(bool value);

		int readInt(int bits);
		bool readBool();

		char* getContent(int & length);
		void reset();

	private:
		int m_numBytes;
		int m_usedBits;
		int m_curPos;
		char* m_bytes;

		void allocateBytes(int numberOfBytes);
		void reallocateBytes(int numberOfBytes);
		void freeBytes();

		void ensureRoomFor(int bits);

		int curByte();
		int curBit();
};

#endif /* _COMPACTSTORAGE_H */