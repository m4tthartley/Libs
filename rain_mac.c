
#include <dlfcn.h>
#include <assert.h>

#include "sdl_def.c"

#define SDL_PROCS\
	SDL_PROC(void, SDL_SetMainReady, void)\
	SDL_PROC(int, SDL_Init, Uint32 flags)\
	SDL_PROC(SDL_Window*, SDL_CreateWindow, const char* title, int x, int y, int w, int h, Uint32 flags)\
	SDL_PROC(SDL_GLContext, SDL_GL_CreateContext, SDL_Window* window)\
	SDL_PROC(void, SDL_GL_SwapWindow, SDL_Window* window)\
	SDL_PROC(int, SDL_PollEvent, SDL_Event* event)\
	SDL_PROC(const Uint8*, SDL_GetKeyboardState, int* numkeys)\
	SDL_PROC(Uint64, SDL_GetPerformanceCounter, void)\
	SDL_PROC(Uint64, SDL_GetPerformanceFrequency, void)\

	//SDL_PROC()\

#define SDL_PROC(ret, name, ...) typedef ret (name##_proc)(__VA_ARGS__); name##_proc *name;
SDL_PROCS
#undef SDL_PROC

void load_sdl_procs() {
	void *sdl_lib = dlopen("libSDL2.dylib", RTLD_LAZY);
	#define SDL_PROC(ret, name, ...) name = dlsym(sdl_lib, #name);
	SDL_PROCS
	#undef SDL_PROC
}


// #if 1
// #define SDL_MAIN_HANDLED
// #include <SDL2/SDL.h>
// #endif

void debug_print(char *str, ...) {
	va_list args;
	va_start(args, str);

	char buf[256];
	vsnprintf(buf, 256, str, args);
	printf("%s", buf);

	va_end(args);
}

typedef enum {
	KEY_0 = SDL_SCANCODE_0,
	KEY_1 = SDL_SCANCODE_1,
	KEY_2 = SDL_SCANCODE_2,
	KEY_3 = SDL_SCANCODE_3,
	KEY_4 = SDL_SCANCODE_4,
	KEY_5 = SDL_SCANCODE_5,
	KEY_6 = SDL_SCANCODE_6,
	KEY_7 = SDL_SCANCODE_7,
	KEY_8 = SDL_SCANCODE_8,
	KEY_9 = SDL_SCANCODE_9,

	KEY_A = SDL_SCANCODE_A,
	KEY_B = SDL_SCANCODE_B,
	KEY_C = SDL_SCANCODE_C,
	KEY_D = SDL_SCANCODE_D,
	KEY_E = SDL_SCANCODE_E,
	KEY_F = SDL_SCANCODE_F,
	KEY_G = SDL_SCANCODE_G,
	KEY_H = SDL_SCANCODE_H,
	KEY_I = SDL_SCANCODE_I,
	KEY_J = SDL_SCANCODE_J,
	KEY_K = SDL_SCANCODE_K,
	KEY_L = SDL_SCANCODE_L,
	KEY_M = SDL_SCANCODE_M,
	KEY_N = SDL_SCANCODE_N,
	KEY_O = SDL_SCANCODE_O,
	KEY_P = SDL_SCANCODE_P,
	KEY_Q = SDL_SCANCODE_Q,
	KEY_R = SDL_SCANCODE_R,
	KEY_S = SDL_SCANCODE_S,
	KEY_T = SDL_SCANCODE_T,
	KEY_U = SDL_SCANCODE_U,
	KEY_V = SDL_SCANCODE_V,
	KEY_W = SDL_SCANCODE_W,
	KEY_X = SDL_SCANCODE_X,
	KEY_Y = SDL_SCANCODE_Y,
	KEY_Z = SDL_SCANCODE_Z,

	KEYBOARD_LEFT = SDL_SCANCODE_LEFT,
	KEYBOARD_RIGHT = SDL_SCANCODE_RIGHT,
	KEYBOARD_UP = SDL_SCANCODE_UP,
	KEYBOARD_DOWN = SDL_SCANCODE_DOWN,

	KEYBOARD_LCTRL = SDL_SCANCODE_LCTRL,
	KEYBOARD_RCTRL = SDL_SCANCODE_RCTRL,
	KEYBOARD_CTRL = SDL_SCANCODE_LCTRL,
	KEYBOARD_LSHIFT = SDL_SCANCODE_LSHIFT,
	KEYBOARD_RSHIFT = SDL_SCANCODE_RSHIFT,
	KEYBOARD_SHIFT = SDL_SCANCODE_LSHIFT,

	KEYBOARD_ALT = SDL_SCANCODE_MENU,
	KEYBOARD_CAPS = SDL_SCANCODE_CAPSLOCK,
	KEYBOARD_TAB = SDL_SCANCODE_TAB,
	KEYBOARD_SPACE = SDL_SCANCODE_SPACE,
	KEYBOARD_RETURN = SDL_SCANCODE_RETURN,
	KEYBOARD_BACKSPACE = SDL_SCANCODE_BACKSPACE,
	KEYBOARD_ESCAPE = SDL_SCANCODE_ESCAPE,

	KEYBOARD_F1 = SDL_SCANCODE_F1,
	KEYBOARD_F2 = SDL_SCANCODE_F2,
	KEYBOARD_F3 = SDL_SCANCODE_F3,
	KEYBOARD_F4 = SDL_SCANCODE_F4,
	KEYBOARD_F5 = SDL_SCANCODE_F5,
	KEYBOARD_F6 = SDL_SCANCODE_F6,
	KEYBOARD_F7 = SDL_SCANCODE_F7,
	KEYBOARD_F8 = SDL_SCANCODE_F8,
	KEYBOARD_F9 = SDL_SCANCODE_F9,
	KEYBOARD_F10 = SDL_SCANCODE_F10,
	KEYBOARD_F11 = SDL_SCANCODE_F11,
	KEYBOARD_F12 = SDL_SCANCODE_F12,
} KeyID;

typedef struct {
	SDL_Window *window;
} RainSDL;

RainSDL _sdl;

double GetSeconds () {
	int64 time = SDL_GetPerformanceCounter();
	double seconds = (double)time / (double)SDL_GetPerformanceFrequency();
	return seconds;
}

int64 GetTime () {
	return SDL_GetPerformanceCounter();
}

float ConvertToSeconds (int64 time) {
	float seconds = (double)time / (double)SDL_GetPerformanceFrequency();
	return seconds;
}

void rain_init(Rain *rain) {
	if (!rain->window_width || !rain->window_height) {
		rain->window_width = 1280;
		rain->window_height = 720;
	}

	load_sdl_procs();

	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO);
	
	_sdl.window = SDL_CreateWindow("The Street Life",
										  SDL_WINDOWPOS_CENTERED,
										  SDL_WINDOWPOS_CENTERED,
										  rain->window_width, rain->window_height,
										  SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
	
	/*SDL_GLContext gl_context =*/ SDL_GL_CreateContext(_sdl.window);

	rain->old_time = GetTime();
}

void rain_update(Rain *rain) {
	SDL_GL_SwapWindow(_sdl.window);

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				rain->quit = true;
				break;
		}
	}
	
	int numkeys;
	const unsigned char *keys = SDL_GetKeyboardState(&numkeys);
	for (int i = 0; i < 256; ++i) {
		// int keycode = SDL_GetKeyFromScancode(i);
		update_digital_button(&rain->keys[i], keys[i]);
	}
	// printf("num keys %i \n", numkeys);


	int64 time = GetTime();
	rain->dt = ConvertToSeconds(time - rain->old_time);
	rain->dt60 = rain->dt*60.0;
	rain->old_time = time;
}

void InitSound(Rain *rain) {

}

void UpdateSound(Rain *rain) {
	
}
