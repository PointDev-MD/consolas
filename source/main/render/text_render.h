#ifndef TEXT_RENDER_H
#define TEXT_RENDER_H

#include <GLFW/glfw3.h>

typedef struct {
    unsigned int texture_id;
    int size_x;
    int size_y;
    int bearing_x;
    int bearing_y;          
    unsigned int advance;
} Character;

int init_text_rendering(const char* font_path, int font_size);
void render_text(const char* text, float x, float y, float scale, float r, float g, float b);
void cleanup_text_rendering(void);

#endif