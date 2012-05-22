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

#include "compactstorage.h"

#include <iostream>

using namespace std;

CompactStorage::CompactStorage(int numberOfBytes)
{
	if (numberOfBytes < 1) {
		numberOfBytes = 1;
	}

	m_numBytes = numberOfBytes;
	m_usedBits = 0;
	m_curPos = 0;
	allocateBytes(numberOfBytes);
}

CompactStorage::~CompactStorage()
{
	freeBytes();
}

void CompactStorage::allocateBytes(int numberOfBytes)
{
	m_bytes = new char[numberOfBytes];
}

void CompactStorage::freeBytes()
{
	delete[] m_bytes;
}

void CompactStorage::reallocateBytes(int numberOfBytes)
{
	m_numBytes = numberOfBytes;
    char* temp = new char[numberOfBytes];
    *temp = *m_bytes;
    freeBytes();
    m_bytes = temp;
}

void CompactStorage::ensureRoomFor(int bits)
{
	cout << "make sure we have room for " << bits << " bits..." << endl;
	int newLength = m_usedBits + bits;
	if (newLength > m_numBytes * 8) {
		reallocateBytes(newLength / 8 + 1);
	}
}

int CompactStorage::curByte()
{
	return m_curPos / 8;
}

int CompactStorage::curBit()
{
	return m_curPos % 8;
}

void CompactStorage::dump()
{
	for (int curByte = 0; curByte < m_numBytes; curByte++) {
		char str = m_bytes[curByte];
		for (int i = 128; i > 0; i = i/2) {
			cout << (bool) ((int) str & i);
		}
		cout << " ";
	}
	cout << endl;
	for (int i = 0; i < m_curPos + m_numBytes - 1; i++) {
		cout << " ";
	}
	cout << "^" << endl;
}

void CompactStorage::writeInt(int value, int bits)
{
	ensureRoomFor(bits);

	// Iterate over the data bits that we need to set
	for (int i = curBit(); i < curBit() + bits; i++) {
		int byte = i / 8;
		int bit = i % 8;

		// Create bitmask for the correspondent bit of the integer to store here
		int mask = 1 << (bits + curBit() - i - 1);

		// Whether to set the current bit or not
		bool set = (bool) (value & mask);
		m_bytes[byte] |= (int) set << (7 - bit);
	}

	m_curPos += bits;
	m_usedBits += bits;
}

void CompactStorage::writeBool(bool value)
{
	char byte = m_bytes[curByte()];
	byte |= (int) value << (7 - curBit());
	m_bytes[curByte()] = byte;

	m_curPos += 1;
	m_usedBits += 1;
}

int CompactStorage::readInt(int bits)
{
	// TODO: Signed vs. unsigned might cause problems here
	int value = 0;

	// Iterate over the data bits that we need to set
	for (int i = curBit(); i < curBit() + bits; i++) {
		int byte = i / 8;
		int bit = i % 8;

		// Create bitmask for the correspondent bit of the data that we want to read
		int mask = 1 << (7 - bit);

		// Whether to set the current bit or not
		bool set = (bool) (m_bytes[byte] & mask);
		value |= (int) set << (bits + curBit() - i - 1);
	}

	m_curPos += bits;
	return value;
}

bool CompactStorage::readBool()
{
	unsigned int byte = m_bytes[curByte()];
	unsigned int mask = 1 << (7 - curBit());

	m_curPos += 1;
	return (byte & mask) > 0;
}

char* CompactStorage::getContent(int & length)
{
	char* result = new char[m_numBytes];
	*result = *m_bytes;

	length = m_numBytes;

	return result;
}

void CompactStorage::reset()
{
	m_curPos = 0;
}
