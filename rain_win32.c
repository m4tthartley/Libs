
/*
	Rain library? A media library called Rain?
	Using data based api ideas from Per Vognsen's platform library Mu
*/

#pragma warning(disable: 4244) // possible loss of data
#pragma warning(disable: 4800) // forcing value to bool

#include <windows.h>
#include <GL/gl.h>
#include <dsound.h>

void debug_print(char *str, ...) {
	va_list args;
	va_start(args, str);

	char buf[256];
	vsnprintf_s(buf, 256, _TRUNCATE, str, args);
	OutputDebugString(buf);

	va_end(args);
}

typedef enum {
	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',
	KEY_5 = '5',
	KEY_6 = '6',
	KEY_7 = '7',
	KEY_8 = '8',
	KEY_9 = '9',
	KEY_0 = '0',

	KEY_A = 'A',
	KEY_B = 'B',
	KEY_C = 'C',
	KEY_D = 'D',
	KEY_E = 'E',
	KEY_F = 'F',
	KEY_G = 'G',
	KEY_H = 'H',
	KEY_I = 'I',
	KEY_J = 'J',
	KEY_K = 'K',
	KEY_L = 'L',
	KEY_M = 'M',
	KEY_N = 'N',
	KEY_O = 'O',
	KEY_P = 'P',
	KEY_Q = 'Q',
	KEY_R = 'R',
	KEY_S = 'S',
	KEY_T = 'T',
	KEY_U = 'U',
	KEY_V = 'V',
	KEY_W = 'W',
	KEY_X = 'X',
	KEY_Y = 'Y',
	KEY_Z = 'Z',

	KEYBOARD_LEFT = VK_LEFT,
	KEYBOARD_RIGHT = VK_RIGHT,
	KEYBOARD_UP = VK_UP,
	KEYBOARD_DOWN = VK_DOWN,

	KEYBOARD_LCTRL = VK_LCONTROL,
	KEYBOARD_RCTRL = VK_RCONTROL,
	KEYBOARD_CTRL = VK_CONTROL,
	KEYBOARD_LSHIFT = VK_LSHIFT,
	KEYBOARD_RSHIFT = VK_RSHIFT,
	KEYBOARD_SHIFT = VK_SHIFT,

	KEYBOARD_ALT = VK_MENU,
	KEYBOARD_CAPS = VK_CAPITAL,
	KEYBOARD_TAB = VK_TAB,
	KEYBOARD_SPACE = VK_SPACE,
	KEYBOARD_RETURN = VK_RETURN,
	KEYBOARD_BACKSPACE = VK_BACK,
	KEYBOARD_ESCAPE = VK_ESCAPE,

	KEYBOARD_F1 = VK_F1,
	KEYBOARD_F2 = VK_F2,
	KEYBOARD_F3 = VK_F3,
	KEYBOARD_F4 = VK_F4,
	KEYBOARD_F5 = VK_F5,
	KEYBOARD_F6 = VK_F6,
	KEYBOARD_F7 = VK_F7,
	KEYBOARD_F8 = VK_F8,
	KEYBOARD_F9 = VK_F9,
	KEYBOARD_F10 = VK_F10,
	KEYBOARD_F11 = VK_F11,
	KEYBOARD_F12 = VK_F12,

	KEY_LEFT = VK_LEFT,
	KEY_RIGHT = VK_RIGHT,
	KEY_UP = VK_UP,
	KEY_DOWN = VK_DOWN,

	KEY_LCTRL = VK_LCONTROL,
	KEY_RCTRL = VK_RCONTROL,
	KEY_CTRL = VK_CONTROL,
	KEY_LSHIFT = VK_LSHIFT,
	KEY_RSHIFT = VK_RSHIFT,
	KEY_SHIFT = VK_SHIFT,

	KEY_ALT = VK_MENU,
	KEY_CAPS = VK_CAPITAL,
	KEY_TAB = VK_TAB,
	KEY_SPACE = VK_SPACE,
	KEY_RETURN = VK_RETURN,
	KEY_BACKSPACE = VK_BACK,
	KEY_ESCAPE = VK_ESCAPE,

	KEY_F1 = VK_F1,
	KEY_F2 = VK_F2,
	KEY_F3 = VK_F3,
	KEY_F4 = VK_F4,
	KEY_F5 = VK_F5,
	KEY_F6 = VK_F6,
	KEY_F7 = VK_F7,
	KEY_F8 = VK_F8,
	KEY_F9 = VK_F9,
	KEY_F10 = VK_F10,
	KEY_F11 = VK_F11,
	KEY_F12 = VK_F12,
} KeyID;

LARGE_INTEGER _globalPerformanceFrequency = {0};

typedef struct {
	HWND window;
	HDC hdc;
	BITMAPINFO bitmap_info;
} RainWin32;

RainWin32 _win32;

//typedef struct {
//	HWND _window;
//	HDC hdc;
//	BITMAPINFO bitmapInfo;
//	void *videoMemory;
//	int windowWidth;
//	int windowHeight;
//	int backBufferWidth;
//	int backBufferHeight;
//	bool windowOpen;
//	struct {
//		bool keys[256];
//		bool keysLast[256];
//		struct {
//			int x;
//			int y;
//		} mouse;
//	} input;
//	bool soundDisabled;
//} OSState;

//typedef enum {
//	VIDEO_MODE_SOFTWARE,
//	VIDEO_MODE_HARDWARE,
//} VideoMode;

#define PrintOut(...) debug_print(__VA_ARGS__); //fprintf(stdout, __VA_ARGS__);
#define PrintErr(...) debug_print(__VA_ARGS__); //fprintf(stderr, __VA_ARGS__);

double GetSeconds () {
	if (!_globalPerformanceFrequency.QuadPart) {
		QueryPerformanceFrequency(&_globalPerformanceFrequency);
	}
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	double seconds = (double)time.QuadPart / (double)_globalPerformanceFrequency.QuadPart;
	return seconds;
}

int64 GetTime () {
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	return time.QuadPart;
}

float ConvertToSeconds (int64 time) {
	if (!_globalPerformanceFrequency.QuadPart) {
		QueryPerformanceFrequency(&_globalPerformanceFrequency);
	}
	float seconds = (double)time / (double)_globalPerformanceFrequency.QuadPart;
	return seconds;
}

LRESULT CALLBACK WindowCallback (HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	Rain *rain = (Rain*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

	LRESULT result = 0;
	switch (message) {
		case WM_DESTROY: {
			rain->quit = true;
			OutputDebugString("WM_DESTROY");
		} break;
		case WM_INPUT: {
			RAWINPUT raw = {0};
			UINT cbsize = sizeof(raw);
			int x = GetRawInputData((HRAWINPUT)lparam, RID_INPUT, &raw, &cbsize, sizeof(RAWINPUTHEADER));
			int y = 0;
			if (raw.header.dwType == RIM_TYPEMOUSE && raw.data.mouse.usFlags == MOUSE_MOVE_RELATIVE) {
				rain->mouse.position_delta.x += raw.data.mouse.lLastX;
				rain->mouse.position_delta.y += raw.data.mouse.lLastY;

				USHORT flags = raw.data.mouse.usButtonFlags;
				if (flags & RI_MOUSE_LEFT_BUTTON_DOWN) update_digital_button(&rain->mouse.left, true);
				if (flags & RI_MOUSE_LEFT_BUTTON_UP) update_digital_button(&rain->mouse.left, false);
				if (flags & RI_MOUSE_RIGHT_BUTTON_DOWN) update_digital_button(&rain->mouse.right, true);
				if (flags & RI_MOUSE_RIGHT_BUTTON_UP) update_digital_button(&rain->mouse.right, false);
				if (flags & RI_MOUSE_MIDDLE_BUTTON_DOWN) update_digital_button(&rain->mouse.middle, true);
				if (flags & RI_MOUSE_MIDDLE_BUTTON_UP) update_digital_button(&rain->mouse.middle, false);

				if (flags & RI_MOUSE_WHEEL) {
					SHORT w = raw.data.mouse.usButtonData;
					rain->mouse.wheel_delta += (w/ WHEEL_DELTA);
				}
			}

			result = DefWindowProc(hwnd, message, wparam, lparam);
			break;
		}
		case WM_SIZE: {
			if (rain) {
				RECT rect;
				GetWindowRect(hwnd, &rect);
				//debug_print("rect %i %i %i %i\n", rect.left, rect.top, rect.right, rect.bottom);

				RECT windowRect;
				windowRect.left = 0;
				windowRect.right = rect.right - rect.left;
				windowRect.top = 0;
				windowRect.bottom = rect.bottom - rect.top;
				AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE, 0);

				int2 diff = {
					(windowRect.right - windowRect.left) - (rect.right - rect.left),
					(windowRect.bottom - windowRect.top) - (rect.bottom - rect.top),
				};

				rain->window_width = (rect.right - rect.left) - diff.x;
				rain->window_height = (rect.bottom - rect.top) - diff.y;
			}
			break;
		}
		default: {
			result = DefWindowProc(hwnd, message, wparam, lparam);
		} break;
	}
	return result;
}

void PollEvents (Rain *rain) {
	// memset(&os->input.
	//os->input.keysLast[Message.wParam] = os->input.keys[Message.wParam];
	//memcpy(rain->input.keys_last, rain->input.keys, sizeof(rain->input.keys));

	BYTE keyboard[256] = {0};
	GetKeyboardState(keyboard);
	for (int i = 0; i < 256; ++i) {
		update_digital_button(&rain->keys[i], keyboard[i]>>7);
	}

	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(_win32.window, &mouse);
	rain->mouse.position.x = mouse.x;
	rain->mouse.position.y = mouse.y;

	rain->mouse.position_delta.x = 0;
	rain->mouse.position_delta.y = 0;
	rain->mouse.wheel_delta = 0;

	rain->mouse.left.pressed = false;
	rain->mouse.left.released = false;
	rain->mouse.right.pressed = false;
	rain->mouse.right.released = false;
	rain->mouse.middle.pressed = false;
	rain->mouse.middle.released = false;

	MSG Message;
	while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE)) {
		switch (Message.message) {
#if 0
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN: {
				switch (Message.wParam) {
					case VK_LEFT: {
						// state.input.leftDown = true;
					} break;
					case VK_RIGHT: {
						// state.input.rightDown = true;
					} break;
				}

				rain->input.keys[Message.wParam] = true;
			} break;

			case WM_SYSKEYUP:
			case WM_KEYUP: {
				rain->input.keys[Message.wParam] = false;
			} break;
#endif
			default: {
				TranslateMessage(&Message);
				DispatchMessageA(&Message);
			}
			break;
		}
	}
}

//bool KeyDown (OSState *os, KeyID key) {
//	return os->input.keys[key];
//}
//
//bool KeyPressed (OSState *os, KeyID key) {
//	return os->input.keys[key] && !os->input.keysLast[key];
//}

bool rain_create_window(Rain *rain) {
	WNDCLASS windowClass = {0};
	windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowCallback;
	// @note: Apparently getting the hInstance this way can cause issues if used in a dll
	HMODULE hInstance = GetModuleHandle(NULL);
	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = "RainWindowClass";
	windowClass.hCursor = LoadCursor(0, IDC_ARROW);

	RECT windowRect;
	windowRect.left = 0;
	windowRect.right = rain->window_width;
	windowRect.top = 0;
	windowRect.bottom = rain->window_height;
	AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE, 0);

	if (RegisterClassA(&windowClass)) {
		if (!rain->window_title) rain->window_title = "Rain";
		_win32.window = CreateWindowExA(0, windowClass.lpszClassName, rain->window_title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			0, 0, hInstance, 0);

		if (_win32.window) {
			rain->quit = false;
			UpdateWindow(_win32.window);

			_win32.hdc = GetDC(_win32.window);
		} else {
			PrintErr("Error while creating window\n");
			//goto error;
			return false;
		}
	} else {
		PrintErr("Error while registering window class\n");
		//goto error;
		return false;
	}

	// register raw input mouse
	RAWINPUTDEVICE mouse_raw_input;
	mouse_raw_input.usUsagePage = 1;
	mouse_raw_input.usUsage = 2;
	mouse_raw_input.dwFlags = 0;
	mouse_raw_input.hwndTarget = _win32.window;
	if (!RegisterRawInputDevices(&mouse_raw_input, 1, sizeof(mouse_raw_input))) {
		debug_print("failed to register raw input mouse\n");
		__debugbreak();
	}

	return true;
}

void InitSoftwareVideo (Rain *rain) {
	//WNDCLASS windowClass = {0};
	//windowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
	//windowClass.lpfnWndProc = WindowCallback;
	//// @note: Apparently getting the hInstance this way can cause issues if used in a dll
	//HMODULE hInstance = GetModuleHandle(NULL);
	//windowClass.hInstance = hInstance;
	//windowClass.lpszClassName = "Win32 window class";
	//windowClass.hCursor = LoadCursor(0, IDC_ARROW);

	//os->windowWidth = windowWidth;
	//os->windowHeight = windowHeight;
	//RECT windowRect;
	//windowRect.left = 0;
	//windowRect.right = windowWidth;
	//windowRect.top = 0;
	//windowRect.bottom = windowHeight;
	//AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW|WS_VISIBLE, FALSE, 0);

	//if (RegisterClassA(&windowClass)) {
	//	os->_window = CreateWindowExA(0, windowClass.lpszClassName, "Software renderer", WS_OVERLAPPEDWINDOW|WS_VISIBLE,
	//		CW_USEDEFAULT, CW_USEDEFAULT,
	//		windowRect.right-windowRect.left,
	//		windowRect.bottom-windowRect.top,
	//		0, 0, hInstance, 0);

		//if (os->_window) {
			
			//UpdateWindow(os->_window);

			//os->hdc = GetDC(os->_window);

	if (!rain_create_window(rain)) goto error;

	if (!rain->software_video_width || !rain->software_video_height) {
		rain->software_video_width = rain->window_width;
		rain->software_video_height = rain->window_height;
	}

	ZeroStruct(_win32.bitmap_info);
	_win32.bitmap_info.bmiHeader.biSize = sizeof(_win32.bitmap_info.bmiHeader);
	/*_win32.backBufferWidth = backBufferWidth;
	_win32.backBufferHeight = backBufferHeight;*/
	_win32.bitmap_info.bmiHeader.biWidth = rain->software_video_width;
	_win32.bitmap_info.bmiHeader.biHeight = rain->software_video_height;
	_win32.bitmap_info.bmiHeader.biPlanes = 1;
	_win32.bitmap_info.bmiHeader.biBitCount = 32;
	_win32.bitmap_info.bmiHeader.biCompression = BI_RGB;

	HBITMAP hBitmap = CreateDIBSection(_win32.hdc, &_win32.bitmap_info, DIB_RGB_COLORS, (void**)&rain->video_memory, 0, 0);

	/*	} else {
			PrintErr("Error while creating window\n");
			goto error;
		}
	} else {
		PrintErr("Error while registering window class\n");
		goto error;
	}*/

	return;
error:
	MessageBox(_win32.window, "There was an error initializing software video", NULL, MB_OK);
	exit(1);
}

#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_SAMPLE_BUFFERS_ARB            0x2041
#define WGL_SAMPLES_ARB                   0x2042

#define WGL_CONTEXT_DEBUG_BIT_ARB         0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_FLAGS_ARB             0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

typedef HGLRC WINAPI wglCreateContextAttribsARB_proc(HDC hDC, HGLRC hShareContext, const int *attribList);
wglCreateContextAttribsARB_proc *wglCreateContextAttribsARB;
typedef BOOL WINAPI wglChoosePixelFormatARB_proc(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
wglChoosePixelFormatARB_proc *wglChoosePixelFormatARB;

void InitOpenglVideo (Rain *rain) {
	PIXELFORMATDESCRIPTOR pixelFormat = {0};

	// note: this sets theses files as the std output location
	/*freopen("stdout.txt", "a", stdout);
	freopen("stderr.txt", "a", stderr);*/

	if (!rain_create_window(rain)) goto error;

	pixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelFormat.nVersion = 1;
	pixelFormat.iPixelType = PFD_TYPE_RGBA;
	pixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pixelFormat.cColorBits = 32;
	pixelFormat.cAlphaBits = 8;
	pixelFormat.iLayerType = PFD_MAIN_PLANE;

	int suggestedIndex = ChoosePixelFormat(_win32.hdc, &pixelFormat);
	if (!suggestedIndex) {
		PrintErr("ChoosePixelFormat failed\n");
		goto error;
	}
	PIXELFORMATDESCRIPTOR suggested;
	DescribePixelFormat(_win32.hdc, suggestedIndex, sizeof(PIXELFORMATDESCRIPTOR), &suggested);
	if (!SetPixelFormat(_win32.hdc, suggestedIndex, &suggested)) {
		PrintErr("SetPixelFormat failed\n");
		goto error;
	}

	HGLRC glContext = wglCreateContext(_win32.hdc);
	if (!glContext) {
		PrintErr("wglCreateContext failed\n");
		goto error;
	}
	if (!wglMakeCurrent(_win32.hdc, glContext)) {
		PrintErr("wglMakeCurrent failed\n");
		goto error;
	}

#if 1
	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_proc*)wglGetProcAddress("wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (wglChoosePixelFormatARB_proc*)wglGetProcAddress("wglChoosePixelFormatARB");
	wglDeleteContext(glContext);

	int format_attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,

		/*WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 8,*/
		0,
	};
	int format;
	uint num_formats;
	wglChoosePixelFormatARB(_win32.hdc, format_attribs, NULL, 1, &format, &num_formats);

	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		0
	};
	HGLRC context = wglCreateContextAttribsARB(_win32.hdc, 0, attribs);
	wglMakeCurrent(_win32.hdc, context);
#endif

	return;
error:
	MessageBox(_win32.window, "There was an error initializing OpenGL video", NULL, MB_OK);
	exit(1);
}


/*
	Only supporting 32 bit floats or 8 bit ints(signed or unsigned)
	Should you be able to have different rgba order?
*/
typedef enum {
	PIXEL_FORMAT_FLOAT,
	PIXEL_FORMAT_UBYTE,
	/*
		PIXEL_FORMAT_FLOAT
		PIXEL_FORMAT_FLOAT_RGBA
		PIXEL_FORMAT_INT_R8G8B8A8
		PIXEL_FORMAT_INT_R16G16B16A16
		PIXEL_FORMAT_INT_R32G32B32A32
	*/
} SoftwarePixelFormat;

#define rgba_to_argb(r, g, b, a) (((a&0xFF)<<24) | ((r&0xFF)<<16) | ((g&0xFF)<<8) | b&0xFF)

void DisplaySoftwareGraphics (Rain *rain, void *data, SoftwarePixelFormat format, int numComponents) {
	unsigned int *pixels = (unsigned int*)rain->video_memory;

	if (format == PIXEL_FORMAT_FLOAT) {
		float *video = (float*)data;
		for (int i = 0; i < rain->software_video_width*rain->software_video_height; ++i) {
			float *v = video + (i*numComponents);
			int components = numComponents >= 4 ? numComponents : 4;
			uint8 c[4] = {0};
			c[3] = 255;
			for (int cc = 0; cc < components; ++cc) {
				c[cc] = v[cc]*255.0f;
			}
			pixels[i] = (c[3] << 24) | (c[0] << 16) | (c[1] << 8) | (c[2]);
			// aarrggbb
		}
	} else if (format == PIXEL_FORMAT_UBYTE) {
		uint8 *video = (uint8*)data;
		for (int i = 0; i < rain->software_video_width*rain->software_video_height; ++i) {
			uint8 *v = video + (i*numComponents);
			int components = numComponents >= 4 ? numComponents : 4;
			uint8 c[4] = {0};
			c[3] = 255;
			for (int cc = 0; cc < components; ++cc) {
				c[cc] = v[cc];
			}
			pixels[i] = (c[0] << 24) | (c[3] << 16) | (c[2] << 8) | (c[1]);
			// aarrggbb
		}
	}
	StretchDIBits(_win32.hdc, 0, 0, rain->window_width, rain->window_height, 0, 0, rain->software_video_width, rain->software_video_height, rain->video_memory, &_win32.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
}

//void FinishVideo (OSState *os) {
//	SwapBuffers(os->hdc);
//}

int _window_width;
int _window_height;
char *_window_title;

void rain_init(Rain *rain) {
	if (!rain->window_width || !rain->window_height) {
		rain->window_width = 1280;
		rain->window_height = 720;
	}

	if (rain->software_video) {
		InitSoftwareVideo(rain);
	} else {
		InitOpenglVideo(rain);
	}

	SetWindowLongPtrA(_win32.window, GWLP_USERDATA, (LONG_PTR)rain);

	rain->start_time = GetTime();
	rain->old_time = rain->start_time;

	_window_width = rain->window_width;
	_window_height = rain->window_height;
	_window_title = rain->window_title;
}

void rain_update(Rain *rain) {
#if 0
	if (rain->window_width != _window_width || rain->window_height != _window_height) {
		RECT curpos;
		GetWindowRect(_win32.window, &curpos);

		RECT windowRect;
		windowRect.left = 0;
		windowRect.right = rain->window_width;
		windowRect.top = 0;
		windowRect.bottom = rain->window_height;
		AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE, 0);

		SetWindowPos(_win32.window, NULL, curpos.left, curpos.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, 0);

		_window_width = rain->window_width;
		_window_height = rain->window_height;
	}
#endif

	if (rain->software_video) {
		//InitSoftwareVideo(rain);
		StretchDIBits(_win32.hdc, 0, 0,
					  rain->window_width, rain->window_height,
					  0, 0,
					  rain->software_video_width, rain->software_video_height,
					  rain->video_memory, &_win32.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	} else {
		SwapBuffers(_win32.hdc); // todo: if this is meant for the top of the loop
									  // then don't swap the first time
	}

	PollEvents(rain);

	int64 time = GetTime();
	//debug_print("time %f\n", ConvertToSeconds(time));
	rain->dt = ConvertToSeconds(time - rain->old_time);
	rain->dt60 = rain->dt*60.0;
	rain->time = time - rain->start_time;
	rain->time_s = ConvertToSeconds(time - rain->start_time);
	rain->old_time = time;
}

#define main_entry int CALLBACK WinMain (HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)

//int CALLBACK WinMain (HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow) {
//	main(__argc, __argv);
//}

// Audio

typedef HRESULT WINAPI DirectSoundCreateProc (LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter);

#define SOUND_SAMPLES_PER_SEC (48000/1)
#define SOUND_MIX_FORWARD 0.05f

/* @todo:
		Need to lock - access to playingSounds
					   access to pause
		Remove critical section from around mixing loop
*/

LPDIRECTSOUND dsound;
LPDIRECTSOUNDBUFFER primaryBuffer;
LPDIRECTSOUNDBUFFER secondaryBuffer;
CRITICAL_SECTION playingSoundsLock;
void InitSound (Rain *rain) {
	WAVEFORMATEX wave = {0};
	DSBUFFERDESC desc = {0};
	DSBUFFERDESC desc2 = {0};

	InitializeCriticalSectionAndSpinCount(&playingSoundsLock, 1024);

	HMODULE dsoundLib = LoadLibraryA("dsound.dll");
	if (!dsoundLib) {
		PrintErr("Error loading dsound.dll\n");
		goto error;
	}

	// HRESULT WINAPI (*)(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter)
	// HRESULT WINAPI (*DirectSoundCreate)(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter)
	// 	= GetProcAddress(dsoundLib, "DirectSoundCreate");
	// DirectSoundCreateProc DirectSoundCreate = GetProcAddress(dsoundLib, "DirectSoundCreate");
	DirectSoundCreateProc *DirectSoundCreate = (DirectSoundCreateProc*)GetProcAddress(dsoundLib, "DirectSoundCreate");
	if (!DirectSoundCreate) {
		PrintErr("Error loading DirectSoundCreate proc\n");
		goto error;
	}

	if (!SUCCEEDED(DirectSoundCreate(0, &dsound, 0))) {
		PrintErr("DirectSoundCreate error\n");
		goto error;
	}

	wave.wFormatTag = WAVE_FORMAT_PCM;
	wave.nChannels = 2;
	wave.nSamplesPerSec = SOUND_SAMPLES_PER_SEC;
	wave.wBitsPerSample = 16;
	wave.nBlockAlign = 4;
	wave.nAvgBytesPerSec = SOUND_SAMPLES_PER_SEC * 4;

	if (!SUCCEEDED(IDirectSound_SetCooperativeLevel(dsound, _win32.window, DSSCL_PRIORITY))) {
		PrintErr("IDirectSound_SetCooperativeLevel error\n");
		goto error;
	}

	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	if (!SUCCEEDED(IDirectSound_CreateSoundBuffer(dsound, &desc, &primaryBuffer, 0))) {
		PrintErr("IDirectSound_CreateSoundBuffer error\n");
		goto error;
	}

	if (!SUCCEEDED(IDirectSoundBuffer_SetFormat(primaryBuffer, &wave))) {
		PrintErr("IDirectSoundBuffer_SetFormat error\n");
		goto error;
	}

	desc2.dwSize = sizeof(DSBUFFERDESC);
	desc2.dwFlags = DSBCAPS_GLOBALFOCUS|DSBCAPS_GETCURRENTPOSITION2;
	desc2.dwBufferBytes = SOUND_SAMPLES_PER_SEC * 4;
	desc2.lpwfxFormat = &wave;
	if (!SUCCEEDED(IDirectSound_CreateSoundBuffer(dsound, &desc2, &secondaryBuffer, 0))) {
		PrintErr("IDirectSound_CreateSoundBuffer error\n");
		goto error;
	}

	IDirectSoundBuffer_SetCurrentPosition(secondaryBuffer, 0);
	IDirectSoundBuffer_Play(secondaryBuffer, 0, 0, DSBPLAY_LOOPING);

	return;

error:
	PrintErr("Failed to initialize DirectSound\n");
	MessageBox(_win32.window, "There was an error initializing audio, continuing without sound", NULL, MB_OK);
	rain->sound_disabled = true;
}

int lastChunkWritten = 0;
typedef union {
	struct {
		int16 left;
		int16 right;
	};
	int16 channels[2];
} SoundSample;

typedef struct {
	Sound *sound;
	int numSamples;
	int cursor;
	float cursorFract;
	float fadeSpeed;
	float fade;
} PlayingSound;

enum PlayingSoundFlags {
	SOUND_FADE_IN = (1<<0),
};

PlayingSound playingSounds[64];
int playingSoundCount = 0;

void SoundPlay (Sound *sound, int flags) {
	EnterCriticalSection(&playingSoundsLock);
	if (sound->channels == 2 && sound->bitsPerSample == 16) {
		playingSounds[playingSoundCount].sound = sound; // todo: setting this as a pointer is bad
		playingSounds[playingSoundCount].numSamples = sound->size/4;
		playingSounds[playingSoundCount].cursor = 0;
		playingSounds[playingSoundCount].fadeSpeed = 0.0f;
		playingSounds[playingSoundCount].fade = 1.0f;
		if (flags & SOUND_FADE_IN) {
			playingSounds[playingSoundCount].fadeSpeed = 0.005f;
			playingSounds[playingSoundCount].fade = 0.0f;
		}
		++playingSoundCount;
	} else {
		PrintErr("Currently only 2 channel 16bit audio is supported!\n");
	}
	LeaveCriticalSection(&playingSoundsLock);
}

void FadeOutSounds () {
	EnterCriticalSection(&playingSoundsLock);
	for (int i = 0; i < playingSoundCount; ++i) {
		playingSounds[i].fadeSpeed = -0.005f;
	}
	LeaveCriticalSection(&playingSoundsLock);
}

int GetSoundDeviceWriteCursor () {
	int playCursor;
	int writeCursor;
	HRESULT result;
	if (result = IDirectSoundBuffer_GetCurrentPosition(secondaryBuffer, (LPDWORD)&playCursor, (LPDWORD)&writeCursor) != DS_OK) {
		PrintErr("IDirectSoundBuffer_GetCurrentPosition error\n");
		return 0;
	}

	return writeCursor / 4; // convert to samples
}

void WriteToSoundDevice (void *buffer, int pos, int size) {
	void *region1;
	uint region1Size;
	void *region2;
	uint region2Size;
	HRESULT r;
	if (!size) {
		PrintErr("paint size is 0\n");
		return;
	}
	if (r = IDirectSoundBuffer_Lock(secondaryBuffer, (pos%SOUND_SAMPLES_PER_SEC)*4, size*4,
								&region1, (LPDWORD)&region1Size, &region2, (LPDWORD)&region2Size, 0) != DS_OK) {
		PrintErr("IDirectSoundBuffer_Lock error\n");
		return;
	}

	memcpy(region1, buffer, region1Size);
	if (region2) {
		memcpy(region2, (char*)buffer + (region1Size), region2Size);
	}

	IDirectSoundBuffer_Unlock(secondaryBuffer, region1, region1Size, region2, region2Size);
}

_inline long AtomicRead (volatile long *value) {
	return InterlockedExchangeAdd(value, 0);
}

_inline long AtomicCompareExchange (volatile long *value, long compare, long exchange) {
	return InterlockedCompareExchange(value, exchange, compare);
}

_inline long AtomicExchange (volatile long *value, long exchange) {
	return InterlockedExchange(value, exchange);
}

bool soundPaused = false;
void PauseSound () {
	// soundPaused = true;
	AtomicExchange((long*)&soundPaused, true);
}

void UnpauseSound () {
	// soundPaused = false;
	AtomicExchange((long*)&soundPaused, false);
}

void TogglePauseSound () {
	// if (!soundPaused) soundPaused = true;
	// else soundPaused = false;
	if (AtomicCompareExchange((long*)&soundPaused, false, true) != false) {
		AtomicExchange((long*)&soundPaused, false);
	}
}

// int16 debugSoundBuffer[SOUND_SAMPLES_PER_SEC];
int buffers = 0;
int oldWritePos = 0;
int paintedEnd = 0;
float volume = 0.25f;
int64 oldTime = 0;
SoundSample visualSamples[SOUND_SAMPLES_PER_SEC];
int visualCursor = 0;
void UpdateSound (Rain *rain) {
	if (rain->sound_disabled) return;

	int writePos = GetSoundDeviceWriteCursor();

	if (writePos < oldWritePos) {
		++buffers;
		// @todo: do the integer wrapping stuff
	}
	int pos = (buffers * SOUND_SAMPLES_PER_SEC) + writePos;
	oldWritePos = writePos;
	int paint = paintedEnd;
	if (paint < pos) paint = pos;

	float mixForward = SOUND_MIX_FORWARD;

	if (!oldTime) oldTime = GetTime();
	int64 time = GetTime();
	float timePassed = ConvertToSeconds(time-oldTime);
	float secondsPerFrame = 1.0f / 60.0f;
	while (timePassed > secondsPerFrame*2.0f) {
		mixForward *= 2.0f;
		secondsPerFrame *= 2.0f;
	}
	if (mixForward > 0.8f) mixForward = 0.8f;
	oldTime = time;

	int end = pos + (SOUND_SAMPLES_PER_SEC * mixForward);
	int paintSize = end - paint;
	if (paintSize < 0) paintSize = 0;

	EnterCriticalSection(&playingSoundsLock);
	for (int i = playingSoundCount-1; i >= 0; --i) {
		playingSounds[i].fade += playingSounds[i].fadeSpeed;
		if (playingSounds[i].fade > 1.0f) {
			playingSounds[i].fade = 1.0f;
			playingSounds[i].fadeSpeed = 0.0f;
		}
		int samplesToPlay = playingSounds[i].numSamples - playingSounds[i].cursor;
		if (samplesToPlay <= 0 || playingSounds[i].fade <= 0.0f) {
			playingSounds[i] = playingSounds[playingSoundCount-1];
			--playingSoundCount;
		}
	}
	
#define BUFFER_SIZE 512
	SoundSample buffer[BUFFER_SIZE];

	// @todo: since we are lerping forward samples you could read 1 sample too many off the end
	while (paintSize > 0) {
		memset(buffer, 0, sizeof(SoundSample)*BUFFER_SIZE);
		int count = paintSize <= BUFFER_SIZE ? paintSize : BUFFER_SIZE;
		bool paused = AtomicRead((long*)&soundPaused);
		if (!paused) {
			int soundCount = AtomicRead((long*)&playingSoundCount);
			for (int i = 0; i < soundCount; ++i) {
				float sampleRateRatio = (double)playingSounds[i].sound->samplesPerSec / (double)SOUND_SAMPLES_PER_SEC;
				int samplesToPlay = (playingSounds[i].numSamples - playingSounds[i].cursor) /** sampleRateRatio*/;
				int size = count;
				if ((samplesToPlay*sampleRateRatio) < size) {
					size = (samplesToPlay*sampleRateRatio);
				}
				SoundSample *input = (SoundSample*)playingSounds[i].sound->data;
				for (int j = 0; j < size; ++j) {
					SoundSample sample;
					float amount0 = 1.0f - playingSounds[i].cursorFract;
					float amount1 = playingSounds[i].cursorFract;
					sample.left = (input[playingSounds[i].cursor].left*volume*amount0) + (input[playingSounds[i].cursor+1].left*volume*amount1);
					sample.right = (input[playingSounds[i].cursor].right*volume*amount0) + (input[playingSounds[i].cursor+1].right*volume*amount1);
					sample.left *= playingSounds[i].fade;
					sample.right *= playingSounds[i].fade;
					
					{
						playingSounds[i].cursorFract += sampleRateRatio;
						int cursorInt = playingSounds[i].cursorFract;
						playingSounds[i].cursorFract = playingSounds[i].cursorFract - cursorInt;
						playingSounds[i].cursor += cursorInt;
					}

					buffer[j].left += sample.left;
					buffer[j].right += sample.right;
				}
			}
		}
		WriteToSoundDevice(buffer, paint, count);
		paintSize -= count;
		paint += count;

		if (!paused) {
			for (int i = 0; i < count; ++i) {
				visualSamples[visualCursor].left = buffer[i].left;
				visualSamples[visualCursor].right = buffer[i].right;
				++visualCursor;
				visualCursor %= SOUND_SAMPLES_PER_SEC;
			}
		}
	}
	LeaveCriticalSection(&playingSoundsLock);

	paintedEnd = end;
}

// Threading test stuff
typedef struct {
	void(*proc) (void *udata);
} WorkerThreadJob;
typedef struct {
	HANDLE semaphore;
	WorkerThreadJob jobs[1024];
	int jobCount;
} WorkerThreadPool;

struct {
	int id;
	int value;
} results[100];
int resultCount = 0;
int num = 0;
DWORD WorkerThreadProc(LPVOID udata) {
	WorkerThreadPool *workerThreadPool = (WorkerThreadPool*)udata;
	DWORD threadId = GetThreadId(GetCurrentThread());
	for (;;) {
		WaitForSingleObject(workerThreadPool->semaphore, INFINITE);
		if (workerThreadPool->jobCount > 0) {
			void(*proc) (void *udata) = workerThreadPool->jobs[workerThreadPool->jobCount - 1].proc;
			workerThreadPool->jobCount--;
			proc(NULL);
			Sleep(10);
		}
		else {
			OutputDebugString("thread woke up when no jobs are available\n");
		}
	}
	return 0;
}

void CreateWorkerThreadPool(WorkerThreadPool *workerThreadPool) {
	ZeroStruct(*workerThreadPool);
	workerThreadPool->semaphore = CreateSemaphore(0, 0, 1024, NULL);
	for (int i = 0; i < 4; ++i) {
		DWORD id;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)WorkerThreadProc, workerThreadPool, 0, &id);
	}
}

void AddWorkerThreadJob(WorkerThreadPool *workerThreadPool, void(*proc) (void *udata), void *udata) {
	workerThreadPool->jobs[workerThreadPool->jobCount].proc = proc;
	++workerThreadPool->jobCount;
	ReleaseSemaphore(workerThreadPool->semaphore, 1, NULL);
}

void AddResult(void *udata) {
	DWORD threadId = GetThreadId(GetCurrentThread());
	results[resultCount].id = threadId;
	results[resultCount].value = num;
	++resultCount;
	++num;
}

void WorkerThreadTest() {
	WorkerThreadPool workerThreads;
	CreateWorkerThreadPool(&workerThreads);
	/*semaphoreHandle = CreateSemaphore(0, 0, 1024, NULL);
	for (int i = 0; i < 4; ++i) {
	DWORD id;
	CreateThread(0, 0, WorkerThreadProc, NULL, 0, &id);
	}

	ReleaseSemaphore(semaphoreHandle, 1, NULL);
	ReleaseSemaphore(semaphoreHandle, 1, NULL);*/

	AddWorkerThreadJob(&workerThreads, AddResult, NULL);
	AddWorkerThreadJob(&workerThreads, AddResult, NULL);
	AddWorkerThreadJob(&workerThreads, AddResult, NULL);
	AddWorkerThreadJob(&workerThreads, AddResult, NULL);
	AddWorkerThreadJob(&workerThreads, AddResult, NULL);
	AddWorkerThreadJob(&workerThreads, AddResult, NULL);
	AddWorkerThreadJob(&workerThreads, AddResult, NULL);
	AddWorkerThreadJob(&workerThreads, AddResult, NULL);
	Sleep(1000);

	for (int i = 0; i < resultCount; ++i) {
		char str[64];
		sprintf(str, "value %i, thread %i\n", results[i].value, results[i].id);
		OutputDebugString(str);
	}
}