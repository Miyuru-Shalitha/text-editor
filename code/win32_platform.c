#include "utils.c"

#include <Windows.h>

typedef struct {
    bool is_running;
} WindowData;

static WindowData window_data = {
    .is_running = true
};

typedef struct {
    BITMAPINFO bitmap_info;
    uint32_t* pixels;
    int width, height;
} RenderBuffer;

static RenderBuffer render_buffer = {0};

#include "rendering.c"

LRESULT CALLBACK window_proc(HWND window, UINT message, WPARAM w_param, LPARAM l_param) {
    LRESULT result = {0};

    switch (message) {
        case WM_CLOSE:
        case WM_DESTROY: {
            window_data.is_running = false;
        } break;

        case WM_SIZE: {
            RECT rect;
            if (!GetClientRect(window, &rect)) {
                // TODO(Miyuru): Handle error
                assert(false);
            }

            render_buffer.width = rect.right - rect.left;
            render_buffer.height = rect.bottom - rect.top;

            if (render_buffer.pixels) {
                if (!VirtualFree(render_buffer.pixels, 0, MEM_RELEASE)) {
                    // TODO(Miyuru): Handle error
                    assert(false);
                }
            }

            render_buffer.pixels = VirtualAlloc(
                0, 
                sizeof(uint32_t) * render_buffer.width * render_buffer.height, 
                MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE
            );
            if (!render_buffer.pixels) {
                // TODO(Miyuru): Handle error
                assert(false);
            }

            render_buffer.bitmap_info.bmiHeader.biSize = sizeof(render_buffer.bitmap_info.bmiHeader);
            render_buffer.bitmap_info.bmiHeader.biWidth = render_buffer.width;
            render_buffer.bitmap_info.bmiHeader.biHeight = render_buffer.height;
            render_buffer.bitmap_info.bmiHeader.biPlanes = 1;
            render_buffer.bitmap_info.bmiHeader.biBitCount = 32;
            render_buffer.bitmap_info.bmiHeader.biCompression = BI_RGB;
        } break;

        default: {
            result = DefWindowProcA(window, message, w_param, l_param);
        }
    }

    return result;
}

int WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_show_cmd) {
    WNDCLASS window_class = {0};
    window_class.lpfnWndProc = window_proc;
    window_class.hInstance = h_instance;
    window_class.lpszClassName = "Text editor window class name";

    ATOM class_atom = RegisterClassA(&window_class);
    if (!class_atom) {
        // TODO(Miyuru): Handle error
        assert(false);
    }

    HWND window = CreateWindowExA(
        0,
        window_class.lpszClassName,
        "Text editor",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1280, 720,
        NULL,
        NULL,
        h_instance,
        NULL
    );
    if (!window) {
        // TODO(Miyuru): Handle error
        assert(false);
    }

    HDC device_context = GetDC(window);

    while (window_data.is_running) {
        MSG message;

        while (PeekMessageA(&message, window, 0, 0, PM_REMOVE)) {
            switch (message.message) {
                default: {
                    TranslateMessage(&message);
                    DispatchMessageA(&message);
                }
            }
        }

        clear_screen(0x555555);

        if (!StretchDIBits(
            device_context,
            0, 0,
            render_buffer.width, render_buffer.height,
            0, 0,
            render_buffer.width, render_buffer.height,
            render_buffer.pixels,
            &render_buffer.bitmap_info,
            DIB_RGB_COLORS,
            SRCCOPY
        )) {
            // TODO(Miyuru): Handle error
            assert(false);
        }
    }

    return 0;
}
