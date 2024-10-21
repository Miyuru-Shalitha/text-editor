inline void clear_screen(uint32_t color) {
    for (int j = 0; j < render_buffer.height; j++) {
        for (int i = 0; i < render_buffer.width; i++) {
            render_buffer.pixels[j * render_buffer.width + i] = color;
        }
    }
}
