#include "BitPacking.h"

static inline char *encode8u(char *p, unsigned char c)
{
	*(unsigned char*)p++ = c;
	return p;
}

static inline const char *decode8u(const char *p, unsigned char *c)
{
	*c = *(unsigned char*)p++;
	return p;
}

static inline IUINT32 Zig(IINT32 value)
{
	return (IUINT32)((value << 1) ^ (value >> 31));
}

static inline IINT32 Zag(IUINT32 ziggedValue)
{
	IINT32 value = (IINT32)ziggedValue;
	return (-(value & 0x01)) ^ ((value >> 1) & ~(1 << 31));
}

static int WriteUInt32Variant(IUINT32 value, char *data, int start)
{
	char* pos = data + start;
	int count = 0;
	do {
		pos = encode8u(pos, (unsigned char)((value & 0x7F) | 0x80));
		count++;
	} while ((value >>= 7) != 0);
	*(pos - 1) &= 0x7F;
	return count;
}

static int ReadUInt32Variant(IUINT32 *value, const char *data, int start)
{
	IUINT32 chunk;
	const char* pos = data + start;

	pos = decode8u(pos, (unsigned char*)value);
	if (((*value) & 0x80) == 0) return 1;
	(*value) &= 0x7F;

	pos = decode8u(pos, (unsigned char*)(&chunk));
	(*value) |= (chunk & 0x7F) << 7;
	if ((chunk & 0x80) == 0) return 2;

	pos = decode8u(pos, (unsigned char*)(&chunk));
	(*value) |= (chunk & 0x7F) << 14;
	if ((chunk & 0x80) == 0) return 3;

	pos = decode8u(pos, (unsigned char*)(&chunk));
	(*value) |= (chunk & 0x7F) << 21;
	if ((chunk & 0x80) == 0) return 4;

	pos = decode8u(pos, (unsigned char*)(&chunk));
	(*value) |= chunk << 28; // can only use 4 bits from this chunk
	if ((chunk & 0xF0) == 0) return 5;

	return -1;
}

int WriteInt32(IINT32 value, char *data, int start)
{
	return WriteUInt32Variant(Zig(value), data, start);
}

int ReadInt32(IINT32 *value, const char *data, int start)
{
	IUINT32 tmp;
	int read = ReadUInt32Variant(&tmp, data, start);
	if (read > 0)
	{
		(*value) = Zag(tmp);
	}
	return read;
}

int WriteUInt32(IUINT32 value, char *data, int start)
{
	return WriteUInt32Variant(value, data, start);
}

int ReadUInt32(IUINT32 *value, const char *data, int start)
{
	IUINT32 tmp;
	int read = ReadUInt32Variant(&tmp, data, start);
	if (read > 0)
	{
		(*value) = tmp;
	}
	return read;
}