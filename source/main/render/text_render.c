#include <stdio.h>
#include <stdlib.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "text_render.h"

static Character characters[128];

int init_text_rendering(const char* font_path, int font_size) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        fprintf(stderr, "ERROR::FREETYPE: Could not init FreeType Library\n");
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, font_path, 0, &face)) {
        fprintf(stderr, "ERROR::FREETYPE: Failed to load font: %s\n", font_path);
        FT_Done_FreeType(ft);
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, font_size);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "ERROR::FREETYTPE: Failed to load Glyph: %c\n", c);
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            .texture_id = texture,
            .size_x    = face->glyph->bitmap.width,
            .size_y    = face->glyph->bitmap.rows,
            .bearing_x = face->glyph->bitmap_left,
            .bearing_y = face->glyph->bitmap_top,
            .advance   = (unsigned int)face->glyph->advance.x
        };
        characters[c] = character;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return 0;
}

void render_text(const char* text, float x, float y, float scale, float r, float g, float b) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glColor3f(r, g, b);

    for (const char* p = text; *p; p++) {
        Character ch = characters[(unsigned char)*p];

        float xpos = x + ch.bearing_x * scale;
        float ypos = y - (ch.size_y - ch.bearing_y) * scale;

        float w = ch.size_x * scale;
        float h = ch.size_y * scale;

        glBindTexture(GL_TEXTURE_2D, ch.texture_id);

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos,     ypos + h);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos,     ypos);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos + w, ypos);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + w, ypos + h);
        glEnd();

        x += (ch.advance >> 6) * scale;
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void cleanup_text_rendering(void) {
    for (int i = 0; i < 128; i++) {
        if (characters[i].texture_id) {
            glDeleteTextures(1, &characters[i].texture_id);
        }
    }
}