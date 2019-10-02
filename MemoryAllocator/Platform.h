#ifndef __PLATFORM_H__
#define __PLATFORM_H__

// 64-Bit arch
#if (defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__) || defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_X64))
#define __64BIT 1

// 32-Bit arch
#elif (defined(_WIN32) && !defined(_WIN64)) || defined(_M_IX86)
#define __32BIT 1
#endif

// Platform includes
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <limits>

#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <algorithm>
#include <functional>

#include <assert.h>
#include <memory>

#include <string>
#include <math.h>

#include <chrono>

#define BIT(x) 1 << x

#define GLOBAL_MEMORY_CAPACITY 10485760 // 10 MB

namespace r1l00
{

	// signed integer type
	using I8 = int8_t;
	using I16 = int16_t;
	using I32 = int32_t;

#ifdef __64BIT
	using I64 = int64_t;
#else
	using I64 = int32_t;
#endif

	// unsigned integer type
	using U8 = uint8_t;
	using U16 = uint16_t;
	using U32 = uint32_t;
#ifdef __64BIT
	using U64 = uint64_t;
#else
	using U64 = uint32_t;
#endif

	// floating point
	using F32 = float_t;
	using F64 = double_t;

	// pointer
	using IPtr = intptr_t;
	using UPtr = uintptr_t;

}

#endif //__PLATFORM_H__
