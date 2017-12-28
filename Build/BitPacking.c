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

static inline IUINT16 Zig16(IINT16 value)
{
	return (IUINT16)((value << 1) ^ (value >> 15));
}

static inline IUINT32 Zig32(IINT32 value)
{
	return (IUINT32)((value << 1) ^ (value >> 31));
}

static inline IUINT64 Zig64(IINT64 value)
{
	return (IUINT64)((value << 1) ^ (value >> 63));
}

static inline IINT16 Zag16(IUINT16 ziggedValue)
{
	IINT16 value = (IINT16)ziggedValue;
	return (-(value & 0x01)) ^ ((value >> 1) & ~(1 << 15));
}

static inline IINT32 Zag32(IUINT32 ziggedValue)
{
	IINT32 value = (IINT32)ziggedValue;
	return (-(value & 0x01)) ^ ((value >> 1) & ~(1 << 31));
}

static inline IINT64 Zag64(IUINT64 ziggedValue)
{
	IINT64 value = (IINT64)ziggedValue;
	return (-(value & 0x01)) ^ ((value >> 1) & ~(1 << 63));
}

static int WriteUInt16Variant(IUINT16 value, char *data, int start)
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

static int WriteUInt64Variant(IUINT64 value, char *data, int start)
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

static int ReadUInt16Variant(IUINT16 *value, const char *data, int start)
{
	IUINT16 chunk;
	const char* pos = data + start;
	*value = 0;

	pos = decode8u(pos, (unsigned char*)value);
	if (((*value) & 0x80) == 0) return 1;
	(*value) &= 0x7F;

	pos = decode8u(pos, (unsigned char*)(&chunk));
	(*value) |= (chunk & 0x7F) << 7;
	if ((chunk & 0x80) == 0) return 2;

	pos = decode8u(pos, (unsigned char*)(&chunk));
	(*value) |= (chunk & 0x7F) << 14;
	if ((chunk & 0xFC) == 0) return 3; //can only use 2 bits from this chunk

	return -1;
}

static int ReadUInt32Variant(IUINT32 *value, const char *data, int start)
{
	IUINT32 chunk = 0;
	const char* pos = data + start;
	*value = 0;

	for (int i = 0; i < 4; i++)
	{
		pos = decode8u(pos, (unsigned char*)(&chunk));
		(*value) |= (chunk & 0x7F) << (i * 7);
		if ((chunk & 0x80) == 0) return i + 1;
	}

	pos = decode8u(pos, (unsigned char*)(&chunk));
	(*value) |= chunk << 28; // can only use 4 bits from this chunk
	if ((chunk & 0xF0) == 0) return 5;

	return -1;
}

static int ReadUInt64Variant(IUINT64 *value, const char *data, int start)
{
	IUINT64 chunk;
	const char* pos = data + start;
	*value = 0;

	for (int i = 0; i < 9; i++)
	{
		pos = decode8u(pos, (unsigned char*)(&chunk));
		(*value) |= (chunk & 0x7F) << i * 7;
		if ((chunk & 0x80) == 0) return i + 1;
	}

	pos = decode8u(pos, (unsigned char*)(&chunk));
	(*value) |= chunk << 63; // can only use 1 bits from this chunk
	if ((chunk & 0xFE) == 0) return 10;

	return -1;
}

int WriteInt8(IINT8 value, char *data, int start)
{
	encode8u(data, (unsigned char)(value));
	return 1;
}

int ReadInt8(IINT8 *value, const char *data, int start)
{
	decode8u(data, (unsigned char*)(value));
	return 1;
}

int WriteUInt8(IUINT8 value, char *data, int start)
{
	encode8u(data + start, (unsigned char)(value));
	return 1;
}

int ReadUInt8(IUINT8 *value, const char *data, int start)
{
	decode8u(data + start, (unsigned char*)(value));
	return 1;
}

int WriteInt16(IINT16 value, char *data, int start)
{
	return WriteUInt16Variant(Zig16(value), data, start);
}

int ReadInt16(IINT16 *value, const char *data, int start)
{
	IUINT16 tmp;
	int read = ReadUInt16Variant(&tmp, data, start);
	if (read > 0)
	{
		(*value) = Zag16(tmp);
	}
	return read;
}

int WriteUInt16(IUINT16 value, char *data, int start)
{
	return WriteUInt16Variant(value, data, start);
}

int ReadUInt16(IUINT16 *value, const char *data, int start)
{
	IUINT16 tmp;
	int read = ReadUInt16Variant(&tmp, data, start);
	if (read > 0)
	{
		(*value) = tmp;
	}
	return read;
}

int WriteInt32(IINT32 value, char *data, int start)
{
	return WriteUInt32Variant(Zig32(value), data, start);
}

int ReadInt32(IINT32 *value, const char *data, int start)
{
	IUINT32 tmp;
	int read = ReadUInt32Variant(&tmp, data, start);
	if (read > 0)
	{
		(*value) = Zag32(tmp);
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

int WriteInt64(IINT64 value, char *data, int start)
{
	return WriteUInt64Variant(Zig64(value), data, start);
}

int ReadInt64(IINT64 *value, const char *data, int start)
{
	IUINT64 tmp;
	int read = ReadUInt64Variant(&tmp, data, start);
	if (read > 0)
	{
		(*value) = Zag64(tmp);
	}
	return read;
}

int WriteUInt64(IUINT64 value, char *data, int start)
{
	return WriteUInt64Variant(value, data, start);
}

int ReadUInt64(IUINT64 *value, const char *data, int start)
{
	IUINT64 tmp;
	int read = ReadUInt64Variant(&tmp, data, start);
	if (read > 0)
	{
		(*value) = tmp;
	}
	return read;
}