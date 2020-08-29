#include <iostream>
#include <vector>
#include <string>
#include <GLFW/glfw3.h>
// #include "log.h"

using namespace std;

int main(void)
{
    // cin.get();

    GLFWwindow* window;

    vector<string> msg {"Hello", "World"};

    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}