#include <stdio.h>
#include <GLFW/glfw3.h>
#include "gapbuffer/gap_buffer.h"
#include "rendering/text_render.h"

static GapBuffer* active_buffer = NULL;

void character_callback(GLFWwindow* window, unsigned int codepoint) {
    if (active_buffer && codepoint < 128) {
        gap_buffer_insert_char(active_buffer, (char)codepoint);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_BACKSPACE) {
            gap_buffer_delete_char(active_buffer);
        }
    }
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Consolas Editor", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    active_buffer = gap_buffer_create(64);

    glfwSetCharCallback(window, character_callback);
    glfwSetKeyCallback(window, key_callback);

    if (init_text_rendering("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 18) != 0) {
        printf("Failed to initialize fonts.\n");
    }

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, height, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClearColor(0.10f, 0.10f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        render_text("Consolas Editor - index.html", 10, 25, 1.0f, 0.7f, 0.7f, 0.7f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanup_text_rendering();
    gap_buffer_free(active_buffer);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}