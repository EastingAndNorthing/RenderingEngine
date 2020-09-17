#include <random>
#include "common.h"
#include "core/Renderer.h"
#include "core/Camera.h"
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
#include "primitives/SphereMesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input/Mouse.h"
#include "input/WindowEventHandler.h"

Settings g_settings;

void dikkemoeder() {
    std::cout << "wowowowowowow " << std::endl;
}

int main() {

    Renderer &renderer = Renderer::Instance();
    WindowEventHandler &WindowEventHandler = WindowEventHandler::Instance();
    WindowEventHandler.bindWindow(renderer.window);

    WindowEventHandler.addMousePositionCallback(dikkemoeder);

    // Mouse &mouse = Mouse::Instance();

    Material basicMaterial("/shaders/Basic");
    Material vertexColorMaterial("/shaders/VertexColors");
    Material colorMaterial("/shaders/Color");
    Material phongShader("/shaders/Phong");

    Uniform4f triangle_color("u_color", { 1.0f, 0.5f, 0.9f, 1.0f });
    colorMaterial.setUniform(triangle_color);

    Uniform3f lightDirection("u_lightDirection", Vec3(0.0f, 0.0f, 2.0f));
    phongShader.setUniform(lightDirection);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> randboy(-0.7, 0.7);

    for (int i = 0; i < 15; i++) {
        // Mesh* box = new BoxMesh(0.1f);
        Mesh* sphere = new SphereMesh(0.25f, 24);
        sphere->setPosition(glm::vec3(randboy(gen), randboy(gen), randboy(gen)));
        sphere->assignMaterial(&phongShader);
        renderer.Enqueue(sphere);
    }

    Mesh* myTetra = new TetrahedronMesh(0.5f);
    myTetra->assignMaterial(&colorMaterial);
    renderer.Enqueue(myTetra);

    Camera* cam = renderer.camera;
    cam->autoRotation = true;

    while (renderer.isActive()) {
        renderer.BeginLoop();


        float time = glfwGetTime();
        float oscillator = sin(time*2) / 2.0f + 0.5f;
        triangle_color.set({ 0.0f, oscillator, 0.8f, 1.0f });
        
        renderer.EndLoop();
    }

    return 0;
}
