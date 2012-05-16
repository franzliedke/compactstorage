#ifndef _COMPACTSTORAGE_H
#define _COMPACTSTORAGE_H

class CompactStorage
{
	public:
		CompactStorage(int numberOfBytes = 1);
		~CompactStorage();

		void dump();

		void writeInt(int value, int bits);
		void writeBool(bool value);

		int readInt(int bits);
		bool readBool();

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