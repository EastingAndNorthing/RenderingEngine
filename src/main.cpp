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
#include "physics/PhysicsHandler.h"
#include "physics/RigidBody.h"

#include "primitives/TetrahedronMesh.h"
#include "primitives/SphereMesh.h"
#include "primitives/PlaneMesh.h"
#include "primitives/BoxMesh.h"

Settings g_settings;

int main() {

    Time &time = Time::Instance();
    Renderer &renderer = Renderer::Instance();
    PhysicsHandler &physicsHandler = PhysicsHandler::Instance();

    // Material basicMaterial("/shaders/Basic");
    // Material vertexColorMaterial("/shaders/VertexColors");

    Material colorMaterial("/shaders/Color");
    Material phongMaterial("/shaders/Phong");
    
    auto lightDirection = new Uniform<Vec3>("u_lightDirection", Vec3(0.5f, 0.0f, 2.0f));
    phongMaterial.assignUniform(lightDirection);

    std::random_device rd; std::mt19937 gen(rd()); std::uniform_real_distribution<> randboy(0, 10);

    for (int i = 0; i < 25; i++) {
        Mesh* sphere = new SphereMesh(0.5f, 24);
        sphere->setPosition(randboy(gen) - 5, 0.5f + randboy(gen) * 2.0, randboy(gen) - 5);
        sphere->assignMaterial(phongMaterial);
        renderer.Enqueue(sphere);

        RigidBody* sphereBody = new RigidBody(sphere);
        sphereBody->boundingBox = glm::vec3(0.5f);
        sphereBody->mass = randboy(gen) / 2;
        physicsHandler.Enqueue(sphereBody);
    }

    Mesh* myTetra = new TetrahedronMesh(1.0f);
    myTetra->setPosition(0.0f, 0.4f, 0.0f);
    myTetra->assignMaterial(colorMaterial);
    renderer.Enqueue(myTetra);

    Mesh* floor = new PlaneMesh(255.0f);
    floor->setRotation(-90.0f, 0.0f, 0.0f);

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
    floor->assignMaterial(floorMaterial);
    renderer.Enqueue(floor);

    while (renderer.isActive()) {
        renderer.BeginLoop();

        physicsHandler.update();

        float oscillator = sin(time.time*1.5) / 2.0f + 0.5f;
        
        colorMaterial.setUniform("u_color", Vec4(0.0f, oscillator, 0.8f, 1.0f)); 
        lightDirection->set({ oscillator * 4.0f - 2.0f, 1.0f, 0.0f });

        renderer.EndLoop();
    }

    return 0;
}
