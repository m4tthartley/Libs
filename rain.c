
#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <memory.h>
#include <stdint.h>
#include <stdarg.h>

typedef int64_t int64;
typedef uint64_t uint64;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int8_t int8;
typedef uint8_t uint8;
#ifdef _WIN32
typedef unsigned int uint;
#endif

#define Assert(expr) if (!(expr)) { *(int*)0 = 0; }
#define ZeroStruct(s) memset(&(s), 0, sizeof(s))
#define array_size(arr) (sizeof(arr)/sizeof(arr[0]))

#include "math.c"

#ifdef _WIN32
#	include "rain_win32.c"
// Undo the near/far bullshit that windows.h does
#	undef near // FUCK YOU WINDOWS!!!
#	undef far
#endif
#ifdef __linux__
#	include "rain_linux.c"
#endif
#ifdef __APPLE__
#	error "Macos is not supported yet"
#endif
