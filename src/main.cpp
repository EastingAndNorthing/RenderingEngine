#include <vector>
#include <random>
#include "common.h"
#include "core/Renderer.h"
#include "core/Vec3.h"
#include "core/Vec4.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"
#include "core/IndexBuffer.h"
#include "core/BatchBuffer.h"
#include "core/Shader.h"
#include "core/Uniforms.h"
#include "primitives/Mesh.h"

#include "primitives/TetrahedronMesh.h"
#include "primitives/BoxMesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Settings g_settings;

int main() {

    Renderer &renderer = Renderer::Instance();

    Material basicMaterial("shaders/Basic");

    Material material("shaders/Color");
    Uniform4f triangle_color("u_color", { 1.0f, 0.5f, 0.9f, 1.0f });
    material.setUniform(triangle_color);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> randboy(-0.7, 0.7);

    BatchBuffer batchBuffer;
    batchBuffer.setMaterial(&basicMaterial);
    
    for (int i = 0; i < 15; i++) {
        Mesh* box = new BoxMesh(0.1f);
        box->position = glm::vec3(randboy(gen), randboy(gen), randboy(gen));
        box->assignMaterial(&basicMaterial);
        renderer.Enqueue(box);
        batchBuffer.addGeometry(box);
    }

    Mesh* myTetra = new TetrahedronMesh(0.2f);
    // // myTetra->rotation = glm::rotate(myTetra->rotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    myTetra->assignMaterial(&basicMaterial);
    renderer.Enqueue(myTetra);

    while (renderer.isActive()) {
        renderer.BeginLoop();

        // basicMaterial.Bind();
        // // myTetra->Bind();
        batchBuffer.Bind();

        // float time = glfwGetTime();
        // float oscillator = sin(time*2) / 2.0f + 0.5f;
        // triangle_color.set({ 0.0f, oscillator, 0.8f, 1.0f });
        
        renderer.EndLoop();
    }

    return 0;
}
