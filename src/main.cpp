#include "common.h"
#include "renderer/renderer.h"

Settings g_settings;

int main(void)
{

    Renderer &renderer = Renderer::instance();

    while (!glfwWindowShouldClose(renderer.window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap the screen buffers
        glfwSwapBuffers(renderer.window);
    }

    glfwTerminate();
    return 0;
}