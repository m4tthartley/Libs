
#ifndef _GIANTJELLY_SDL_
#define _GIANTJELLY_SDL_

#include "sdl_def.c"

#define SDL_PROCS\
	SDL_PROC(void, SDL_SetMainReady, void)\
	SDL_PROC(int, SDL_Init, Uint32 flags)\
	SDL_PROC(SDL_Window*, SDL_CreateWindow, const char* title, int x, int y, int w, int h, Uint32 flags)\
	SDL_PROC(SDL_GLContext, SDL_GL_CreateContext, SDL_Window* window)\
	SDL_PROC(void, SDL_GL_SwapWindow, SDL_Window* window)\
	SDL_PROC(int, SDL_GL_SetSwapInterval, int interval)\
	SDL_PROC(int, SDL_PollEvent, SDL_Event* event)\
	SDL_PROC(const Uint8*, SDL_GetKeyboardState, int* numkeys)\
	SDL_PROC(Uint64, SDL_GetPerformanceCounter, void)\
	SDL_PROC(Uint64, SDL_GetPerformanceFrequency, void)\
	SDL_PROC(int, SDL_GL_SetAttribute, SDL_GLattr attr, int value)\
	SDL_PROC(Uint32, SDL_RegisterEvents, int numevents)\
	SDL_PROC(int, SDL_PushEvent, SDL_Event* event)\
	SDL_PROC(SDL_Scancode, SDL_GetScancodeFromKey, SDL_Keycode key)\
	SDL_PROC(int, SDL_WaitEvent, SDL_Event* event)\
	SDL_PROC(const char*, SDL_GetKeyName, SDL_Keycode key)\

//SDL_PROC()\

#define SDL_PROC(ret, name, ...) typedef ret (name##_proc)(__VA_ARGS__); name##_proc *name;
SDL_PROCS
#undef SDL_PROC

void load_sdl_procs() {
	DyLib sdl_lib = load_dynamic_library("libSDL2");
#define SDL_PROC(ret, name, ...) name = (name##_proc*)load_library_proc(sdl_lib, #name);
	SDL_PROCS
#undef SDL_PROC
}

#endif