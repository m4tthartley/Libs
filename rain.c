
#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <memory.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>

typedef int64_t int64;
typedef uint64_t uint64;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int8_t int8;
typedef uint8_t uint8;
//#ifdef _WIN32
typedef unsigned int uint;
//#endif

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
	bool multisample_window;

	bool software_video;
	int software_video_width;
	int software_video_height;
	int *video_memory;

	bool sound_disabled;

	double dt; // Seconds
	double dt60; // Out of 60th of a second
	int64 start_time;
	int64 time;
	float time_s;
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

void update_digital_button(digital_button *button, bool new_state) {
	button->pressed = new_state && !button->down;
	button->released = !new_state && button->down;
	button->down = new_state;
}

typedef struct {
	int channels;
	int samplesPerSec;
	int bitsPerSample;
	void *data;
	size_t size;
} Sound;

#pragma pack(push, 1)
typedef struct {
	char ChunkId[4];
	uint ChunkSize;
	char WaveId[4];
} WavHeader;
typedef struct {
	char id[4];
	uint size;
	uint16 formatTag;
	uint16 channels;
	uint samplesPerSec;
	uint bytesPerSec;
	uint16 blockAlign;
	uint16 bitsPerSample;
	uint16 cbSize;
	int16 validBitsPerSample;
	int channelMask;
	char subFormat[16];
} WavFormatChunk;
typedef struct {
	char id[4];
	uint size;
	void *data;
	char padByte;
} WavDataChunk;
/*typedef struct {
	WavHeader header;
	WavFormatChunk format;
	int16 *data;
	uint dataSize;
	file_data file;
} WavData;*/
#pragma pack(pop)

Sound LoadSoundFromMemory (void *data, size_t size) {
	WavHeader *header = (WavHeader*)data;
	WavFormatChunk *format = NULL;
	WavDataChunk *dataChunk = NULL;
	char *f = (char*)(header + 1);
	while (f < (char*)data + size) {
		int id = *(int*)f;
		uint size = *(uint*)(f+4);
		if (id == (('f'<<0)|('m'<<8)|('t'<<16)|(' '<<24))) {
			format = (WavFormatChunk*)f;
		}
		if (id == (('d'<<0)|('a'<<8)|('t'<<16)|('a'<<24))) {
			dataChunk = (WavDataChunk*)f;
			dataChunk->data = f + 8;
		}
		f += size + 8;
	}

	Sound sound;
	if (format && dataChunk) {
		sound.channels = format->channels;
		sound.samplesPerSec = format->samplesPerSec;
		sound.bitsPerSample = format->bitsPerSample;
		sound.data = dataChunk->data;
		sound.size = dataChunk->size;
	}
	return sound;
}

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
#	include "rain_mac.c"
// #	error "Macos is not supported yet"
#endif
