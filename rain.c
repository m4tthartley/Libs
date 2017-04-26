
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

typedef struct {
	bool down;
	bool pressed;
	bool released;
} digital_button;
typedef struct {
	//RainWin32 win32;

	bool quit;
	int window_width;
	int window_height;
	char *window_title;

	bool software_video;
	int software_video_width;
	int software_video_height;
	int *video_memory;

	bool sound_disabled;

	double dt; // Seconds
	double dt60; // Out of 60th of a second
	int64 old_time;

	digital_button keys[256];

	struct {
		int2 position;
		int2 position_delta;
		digital_button left;
		digital_button right;
		digital_button middle;
		int wheel_delta;
	} mouse;
} Rain;

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
