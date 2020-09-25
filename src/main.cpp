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
#include "primitives/SphereMesh.h"
#include "primitives/PlaneMesh.h"
#include "primitives/BoxMesh.h"

Settings g_settings;

int main() {

    Renderer &renderer = Renderer::Instance();

    // Material basicMaterial("/shaders/Basic");
    // Material vertexColorMaterial("/shaders/VertexColors");

    Material colorMaterial("/shaders/Color");
    Material phongMaterial("/shaders/Phong");
    
    auto triangle_color = new Uniform<Vec4>("u_color", { 1.0f, 0.5f, 0.9f, 1.0f });
    auto lightDirection = new Uniform<Vec3>("u_lightDirection", Vec3(0.5f, 0.0f, 2.0f));

    colorMaterial.setUniform(triangle_color);
    phongMaterial.setUniform(lightDirection);

    std::random_device rd; std::mt19937 gen(rd()); std::uniform_real_distribution<> randboy(-1, 1);

    for (int i = 0; i < 15; i++) {
        Mesh* sphere = new SphereMesh(0.25f, 24);
        sphere->setPosition(randboy(gen), 0.8f + randboy(gen) * 0.5, randboy(gen));
        sphere->assignMaterial(&phongMaterial);
        renderer.Enqueue(sphere);
    }

    Mesh* myTetra = new TetrahedronMesh(0.5f);
    myTetra->assignMaterial(&colorMaterial);
    renderer.Enqueue(myTetra);

    Mesh* floor = new PlaneMesh(255.0f);
    floor->setRotation(-90.0f, 0.0f, 0.0f);
    floor->setPosition(0.0f, -0.166f, 0.0f);

    Material floorMaterial("/shaders/Phong", {
        new Uniform<Vec3>("ambient", Vec3(0.2f, 0.3f, 0.3f)),
        new Uniform<Vec3>("diffuseAlbedo", Vec3(0.2f, 0.3f, 0.3f)),
        new Uniform<Vec3>("specularAlbedo", Vec3(0.2f, 0.2f, 0.2f)),
        new Uniform<Vec3>("rimColor", Vec3(0.2f, 0.3f, 0.3f)),
        new Uniform<int>("rimLightOn", 1),
        new Uniform<float>("shininess", 20.0f),
        new Uniform<float>("rimPower", 200.0f),
        lightDirection
    });
    floor->assignMaterial(&floorMaterial);

    renderer.Enqueue(floor);

    // colorMaterial.set("u_color", { 0.0f, 0.0f, 1.0f }); 

    while (renderer.isActive()) {
        renderer.BeginLoop();

        float time = glfwGetTime();
        float oscillator = sin(time*1.5) / 2.0f + 0.5f;
        
        triangle_color->set({ 0.0f, oscillator, 0.8f, 1.0f });
        lightDirection->set({ oscillator * 4.0f - 2.0f, 1.0f, 0.0f });

        renderer.EndLoop();
    }

    return 0;
}
