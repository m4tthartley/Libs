
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

typedef union {
	struct {
		int x;
		int y;
	};
	struct {
		int u;
		int v;
	};
} int2;

typedef union {
	struct {
		float x;
		float y;
	};
	struct {
		float u;
		float v;
	};
} float2;

typedef union {
	struct {
		float x;
		float y;
		float z;
	};
	struct {
		float r;
		float g;
		float b;
	};
	float2 xy;
} float3;

typedef union {
	struct {
		float x;
		float y;
		float z;
		float w;
	};
	struct {
		float r;
		float g;
		float b;
		float a;
	};
	float3 xyz;
	float2 xy;
} float4;

typedef union {
	struct {
		float m00;
		float m01;
		float m02;
		float m03;
		float m10;
		float m11;
		float m12;
		float m13;
		float m20;
		float m21;
		float m22;
		float m23;
		float m30;
		float m31;
		float m32;
		float m33;
	};
	float e[16];
} mat4;

typedef float4 quaternion;

#ifdef _WIN32
#	include "win32.c"
// Undo the near/far bullshit that windows.h does
#	undef near // FUCK YOU WINDOWS!!!
#	undef far
#endif
#ifdef __linux__
#	include "linux.c"
#endif
#ifdef __APPLE__
#	error "Macos is not supported yet"
#endif
