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

#include "primitives/TetrahedronMesh.h"
#include "primitives/BoxMesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Settings g_settings;

int main(int argc, char **argv) {

    Renderer &renderer = Renderer::Instance();

    Shader basicShader("shaders/VertexColors");
    Material basicMaterial(&basicShader);

    // Draw some random quads on the screen
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> randboy(-0.7, 0.7);
    
    for (int i = 0; i < 15; i++) {

        Mesh* box = new BoxMesh(0.1f);

        box->position = glm::vec3(randboy(gen), randboy(gen), randboy(gen));
        
        box->assignMaterial(&basicMaterial);

        renderer.Enqueue(box);

    }

    Mesh* myTetra = new TetrahedronMesh(0.4f);
    myTetra->rotation = glm::rotate(myTetra->rotation, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    Material material(new Shader("shaders/Color"));
    Uniform4f triangle_color("u_color", { 1.0f, 0.5f, 0.9f, 1.0f });
    material.setUniform(triangle_color);

    myTetra->assignMaterial(&material);
    
    renderer.Enqueue(myTetra);

    while (!glfwWindowShouldClose(renderer.window)) {
        glfwPollEvents();

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
