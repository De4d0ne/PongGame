#include "utils.cpp"

#include <Windows.h>

global_variable bool runing = true;

struct Render_State
{
	int width, height;
	void* memory;

	BITMAPINFO bitmap_info;
};


global_variable Render_State render_state;

#include "renderer.cpp"

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (uMsg)
	{
		case WM_CLOSE:
		case WM_DESTROY:
			{
				runing = false;
			} break;

		case WM_SIZE:
			{
				RECT rect;
				GetClientRect(hwnd, &rect);
				render_state.width = rect.right - rect.left;
				render_state.height = rect.bottom - rect.top;

				int size = render_state.width * render_state.height * sizeof(u32);

				if(render_state.memory)
				{
					VirtualFree(render_state.memory, 0, MEM_RELEASE);
				}
				render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);


				render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
				render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
				render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
				render_state.bitmap_info.bmiHeader.biPlanes = 1;
				render_state.bitmap_info.bmiHeader.biBitCount = 32;
				render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
				typedef struct tagBITMAP {
					LONG   bmType;
					LONG   bmWidth;
					LONG   bmHeight;
					LONG   bmWidthBytes;
					WORD   bmPlanes;
					WORD   bmBitsPixel;
					LPVOID bmBits;
				} BITMAP, * PBITMAP, * NPBITMAP, * LPBITMAP;
			}

		default:
			{
				result = DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
	}
	return result;
};


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = LPCWSTR("Game Windows Class");
	window_class.lpfnWndProc = window_callback;

	RegisterClass(&window_class);

	HWND window = CreateWindow(window_class.lpszClassName, LPCWSTR("Pong"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	while (runing)
	{
		//input
		MSG message;
		while(PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		
		//simulate
		clear_screen(0xff5500);
		draw_rect(0, 0, 0.2, 0.2, 0xff0000);
		//render
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

	}
}