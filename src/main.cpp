#include <vector>
#include <random>
#include "common.h"
#include "core/Renderer.h"
#include "core/Vec3.h"
#include "core/Vec4.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"
#include "core/IndexBuffer.h"
#include "core/Shader.h"
#include "core/Uniforms.h"
#include "primitives/Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Settings g_settings;

int main(int argc, char **argv) {

    Renderer &renderer = Renderer::Instance();

    // Draw some random quads on the screen
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.7, 0.7);

    float quadSize = 0.05f;

    Shader basicShader("shaders/VertexColors");
    Material basicMaterial(&basicShader);

    for (int i = 0; i < 15; i++) {
        
        double randX = dis(gen);
        double randY = dis(gen);

        Vertex v1 = {
            Vec3(-quadSize + randX, -quadSize + randY, 0.5f),
            Vec4(1.0f, 0.0f, 0.0f, 0.0f),
        };

        Vertex v2 = {
            Vec3(quadSize + randX, -quadSize + randY, -0.2f),
            Vec4(0.0f, 1.0f, 0.0f, 0.0f),
        };

        Vertex v3 = {
            Vec3(quadSize + randX,  quadSize + randY, 0.2f),
            Vec4(0.0f, 0.0f, 1.0f, 0.0f),
        };

        Vertex v4 = {
            Vec3(-quadSize + randX,  quadSize + randY, -0.6f),
            Vec4(1.0f, 1.0f, 0.0f, 0.0f),
        };

        std::vector<Vertex> someQuad {
            v1, v2, v3, v4
        };

        std::vector<unsigned int> someQuadIndices = {
            0, 1, 2,
            0, 2, 3
        };

        Mesh* myQuad = new Mesh(someQuad, someQuadIndices);  // auto myQuad = std::make_unique<Mesh>(someQuad, someQuadIndices);

        myQuad->assignMaterial(&basicMaterial);

        renderer.Enqueue(myQuad);
    }

    std::vector<Vertex> someTriangle { Vertex(-0.5,-0.5,0), Vertex(0.5,-0.5,1), Vertex(0,0.5,0) };
    Mesh* myTriangle = new Mesh(someTriangle);  // auto myTriangle = std::make_unique<Mesh>(someTriangle);

    Material material(new Shader("shaders/Color"));
    Uniform4f triangle_color("u_color", { 1.0f, 0.5f, 0.9f, 1.0f });
    material.setUniform(triangle_color);

    myTriangle->assignMaterial(&material);
    
    renderer.Enqueue(myTriangle);

    renderer.Clear();

    while (!glfwWindowShouldClose(renderer.window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();
        float greenValue = sin(time*2) / 2.0f + 0.5f;

        triangle_color.set({ 0.0f, greenValue, 0.8f, 1.0f });
        renderer.Draw();

        glfwSwapBuffers(renderer.window);
    }

    // delete material; // Clean up heap, do this via Renderer?
    renderer.Clear();

    glfwTerminate();

    return 0;
}
