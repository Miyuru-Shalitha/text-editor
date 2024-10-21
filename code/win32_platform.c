#include "utils.c"

#include <Windows.h>

typedef struct {
    bool is_running;
} WindowData;

static WindowData window_data = {
    .is_running = true
};

LRESULT CALLBACK window_proc(HWND h_window, UINT message, WPARAM w_param, LPARAM l_param) {
    LRESULT result;

    switch (message) {
        case WM_CLOSE:
        case WM_DESTROY: {
            window_data.is_running = false;
        } break;

        default: {
            result = DefWindowProcA(h_window, message, w_param, l_param);
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
    }

    return 0;
}
