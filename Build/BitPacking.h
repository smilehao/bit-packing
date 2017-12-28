#ifndef __IBITPACKING_H__
#define __IBITPACKING_H__

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>


//=====================================================================
// 32BIT INTEGER DEFINITION 
//=====================================================================
#ifndef __INTEGER_32_BITS__
#define __INTEGER_32_BITS__
#if defined(_WIN64) || defined(WIN64) || defined(__amd64__) || \
	defined(__x86_64) || defined(__x86_64__) || defined(_M_IA64) || \
	defined(_M_AMD64)
typedef unsigned int ISTDUINT32;
typedef int ISTDINT32;
#elif defined(_WIN32) || defined(WIN32) || defined(__i386__) || \
	defined(__i386) || defined(_M_X86)
typedef unsigned long ISTDUINT32;
typedef long ISTDINT32;
#elif defined(__MACOS__)
typedef UInt32 ISTDUINT32;
typedef SInt32 ISTDINT32;
#elif defined(__APPLE__) && defined(__MACH__)
#include <sys/types.h>
typedef u_int32_t ISTDUINT32;
typedef int32_t ISTDINT32;
#elif defined(__BEOS__)
#include <sys/inttypes.h>
typedef u_int32_t ISTDUINT32;
typedef int32_t ISTDINT32;
#elif (defined(_MSC_VER) || defined(__BORLANDC__)) && (!defined(__MSDOS__))
typedef unsigned __int32 ISTDUINT32;
typedef __int32 ISTDINT32;
#elif defined(__GNUC__)
#include <stdint.h>
typedef uint32_t ISTDUINT32;
typedef int32_t ISTDINT32;
#else 
typedef unsigned long ISTDUINT32;
typedef long ISTDINT32;
#endif
#endif


//=====================================================================
// Integer Definition
//=====================================================================
#ifndef __IINT8_DEFINED
#define __IINT8_DEFINED
typedef char IINT8;
#endif

#ifndef __IUINT8_DEFINED
#define __IUINT8_DEFINED
typedef unsigned char IUINT8;
#endif

#ifndef __IUINT16_DEFINED
#define __IUINT16_DEFINED
typedef unsigned short IUINT16;
#endif

#ifndef __IINT16_DEFINED
#define __IINT16_DEFINED
typedef short IINT16;
#endif

#ifndef __IINT32_DEFINED
#define __IINT32_DEFINED
typedef ISTDINT32 IINT32;
#endif

#ifndef __IUINT32_DEFINED
#define __IUINT32_DEFINED
typedef ISTDUINT32 IUINT32;
#endif

#ifndef __IINT64_DEFINED
#define __IINT64_DEFINED
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef __int64 IINT64;
#else
typedef long long IINT64;
#endif
#endif

#ifndef __IUINT64_DEFINED
#define __IUINT64_DEFINED
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64 IUINT64;
#else
typedef unsigned long long IUINT64;
#endif
#endif

#ifndef INLINE
#if defined(__GNUC__)

#if (__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1))
#define INLINE         __inline__ __attribute__((always_inline))
#else
#define INLINE         __inline__
#endif

#elif (defined(_MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__))
#define INLINE __inline
#else
#define INLINE 
#endif
#endif

#if (!defined(__cplusplus)) && (!defined(inline))
#define inline INLINE
#endif

#ifdef _MSC_VER
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4996)
#endif

#endif


//---------------------------------------------------------------------
// WORD ORDER
//---------------------------------------------------------------------
#ifndef IWORDS_BIG_ENDIAN
#ifdef _BIG_ENDIAN_
#if _BIG_ENDIAN_
#define IWORDS_BIG_ENDIAN 1
#endif
#endif
#ifndef IWORDS_BIG_ENDIAN
#if defined(__hppa__) || \
            defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
            (defined(__MIPS__) && defined(__MIPSEB__)) || \
            defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
            defined(__sparc__) || defined(__powerpc__) || \
            defined(__mc68000__) || defined(__s390x__) || defined(__s390__)
#define IWORDS_BIG_ENDIAN 1
#endif
#endif
#ifndef IWORDS_BIG_ENDIAN
#define IWORDS_BIG_ENDIAN  0
#endif
#endif

#ifdef DLL_EXPORTS
#define DLLEXPORITS _declspec(dllexport)
#else
#define DLLEXPORITS
#endif

#ifdef __cplusplus
extern "C" {
#endif

	//---------------------------------------------------------------------
	// interface
	//---------------------------------------------------------------------
	// 所有操作需自行保证缓冲区足够
	// 读写8位有符号int，1字节
	DLLEXPORITS int WriteInt8(IINT8 val, char *data, int start);

	DLLEXPORITS int ReadInt8(IINT8* val, const char *data, int start);

	// 读写8位无符号int，1字节
	DLLEXPORITS int WriteUInt8(IUINT8 val, char *data, int start);

	DLLEXPORITS int ReadUInt8(IUINT8* val, const char *data, int start);

	// 读写16位有符号int，ZigZig压缩，写入时最大为3字节，返回操作的字节数
	DLLEXPORITS int WriteInt16(IINT16 val, char *data, int start);

	DLLEXPORITS int ReadInt16(IINT16* val, const char *data, int start);

	// 读写16位无符号int，写入时最大为2字节，返回操作的字节数
	DLLEXPORITS int WriteUInt16(IUINT16 val, char *data, int start);

	DLLEXPORITS int ReadUInt16(IUINT16* val, const char *data, int start);

	// 读写32位有符号int，ZigZig压缩，写入时最大为5字节，返回操作的字节数
	DLLEXPORITS int WriteInt32(IINT32 val, char *data, int start);

	DLLEXPORITS int ReadInt32(IINT32* val, const char *data, int start);

	// 读写32位无符号int，写入时最大为4字节，返回操作的字节数
	DLLEXPORITS int WriteUInt32(IUINT32 val, char *data, int start);

	DLLEXPORITS int ReadUInt32(IUINT32* val, const char *data, int start);

	// 读写64位有符号int，ZigZig压缩，写入时最大为10字节，返回操作的字节数
	DLLEXPORITS int WriteInt64(IINT64 val, char *data, int start);

	DLLEXPORITS int ReadInt64(IINT64* val, const char *data, int start);

	// 读写64位有符号int，写入时最大为8字节，返回操作的字节数
	DLLEXPORITS int WriteUInt64(IUINT64 val, char *data, int start);

	DLLEXPORITS int ReadUInt64(IUINT64* val, const char *data, int start);

#ifdef __cplusplus
}
#endif