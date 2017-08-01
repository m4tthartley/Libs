
#include <dlfcn.h>
#include <assert.h>
#include <OpenGL/gl.h>

#include "sdl.c"

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
	
	KEY_LEFT = SDL_SCANCODE_LEFT,
	KEY_RIGHT = SDL_SCANCODE_RIGHT,
	KEY_UP = SDL_SCANCODE_UP,
	KEY_DOWN = SDL_SCANCODE_DOWN,
	KEY_LCTRL = SDL_SCANCODE_LCTRL,
	KEY_RCTRL = SDL_SCANCODE_RCTRL,
	KEY_CTRL = SDL_SCANCODE_LCTRL,
	KEY_LSHIFT = SDL_SCANCODE_LSHIFT,
	KEY_RSHIFT = SDL_SCANCODE_RSHIFT,
	KEY_SHIFT = SDL_SCANCODE_LSHIFT,
	KEY_ALT = SDL_SCANCODE_MENU,
	KEY_CAPS = SDL_SCANCODE_CAPSLOCK,
	KEY_TAB = SDL_SCANCODE_TAB,
	KEY_SPACE = SDL_SCANCODE_SPACE,
	KEY_RETURN = SDL_SCANCODE_RETURN,
	KEY_BACKSPACE = SDL_SCANCODE_BACKSPACE,
	KEY_ESCAPE = SDL_SCANCODE_ESCAPE,
	KEY_F1 = SDL_SCANCODE_F1,
	KEY_F2 = SDL_SCANCODE_F2,
	KEY_F3 = SDL_SCANCODE_F3,
	KEY_F4 = SDL_SCANCODE_F4,
	KEY_F5 = SDL_SCANCODE_F5,
	KEY_F6 = SDL_SCANCODE_F6,
	KEY_F7 = SDL_SCANCODE_F7,
	KEY_F8 = SDL_SCANCODE_F8,
	KEY_F9 = SDL_SCANCODE_F9,
	KEY_F10 = SDL_SCANCODE_F10,
	KEY_F11 = SDL_SCANCODE_F11,
	KEY_F12 = SDL_SCANCODE_F12,

	KEY_WINDOWS = SDL_SCANCODE_APPLICATION,
	KEY_LGUI = SDL_SCANCODE_LGUI,
	KEY_RGUI = SDL_SCANCODE_RGUI,
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
	if (!rain->window_title) {
		rain->window_title = "Rain";
	}

	load_sdl_procs();

	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO);
	
	if (rain->multisample_window) {
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	}

	_sdl.window = SDL_CreateWindow(rain->window_title,
								   SDL_WINDOWPOS_CENTERED,
								   SDL_WINDOWPOS_CENTERED,
								   rain->window_width, rain->window_height,
								   SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	
	/*SDL_GLContext gl_context =*/ SDL_GL_CreateContext(_sdl.window);

	if (rain->multisample_window) {
		glEnable(GL_MULTISAMPLE);
	}

	rain->start_time = GetTime();
	rain->old_time = rain->start_time;
}

void rain_swap_buffers(Rain *rain) {
	SDL_GL_SwapWindow(_sdl.window);
}

void rain_poll_events(Rain *rain) {
	rain->mouse.position_delta.x = 0;
	rain->mouse.position_delta.y = 0;
	rain->mouse.wheel_delta = 0;
	
	rain->mouse.left.pressed = false;
	rain->mouse.left.released = false;
	rain->mouse.right.pressed = false;
	rain->mouse.right.released = false;
	rain->mouse.middle.pressed = false;
	rain->mouse.middle.released = false;
	
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				rain->quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) update_digital_button(&rain->mouse.left, true);
				if (event.button.button == SDL_BUTTON_RIGHT) update_digital_button(&rain->mouse.right, true);
				if (event.button.button == SDL_BUTTON_MIDDLE) update_digital_button(&rain->mouse.middle, true);
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) update_digital_button(&rain->mouse.left, false);
				if (event.button.button == SDL_BUTTON_RIGHT) update_digital_button(&rain->mouse.right, false);
				if (event.button.button == SDL_BUTTON_MIDDLE) update_digital_button(&rain->mouse.middle, false);
				break;
			case SDL_MOUSEMOTION:
				rain->mouse.position = {event.motion.x, event.motion.y};
				rain->mouse.position_delta = {event.motion.xrel, event.motion.yrel};
				break;
			case SDL_MOUSEWHEEL:
				rain->mouse.wheel_delta += event.wheel.y;
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						rain->window_width = event.window.data1;
						rain->window_height = event.window.data2;
						break;
				}
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
}

void rain_poll_time(Rain *rain) {
	int64 time = GetTime();
	rain->dt = ConvertToSeconds(time - rain->old_time);
	rain->dt60 = rain->dt*60.0;
	rain->time = time - rain->start_time;
	rain->time_s = ConvertToSeconds(time - rain->start_time);
	rain->old_time = time;
}

void rain_update(Rain *rain) {
	rain_swap_buffers(rain);
	rain_poll_events(rain);
	rain_poll_time(rain);
}

void InitSound(Rain *rain) {

}

void UpdateSound(Rain *rain) {
	
}
