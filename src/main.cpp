#include <random>
#include "common.h"
#include "core/Renderer.h"
#include "core/Camera.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"
#include "core/IndexBuffer.h"
#include "core/BatchBuffer.h"
#include "core/Shader.h"
#include "core/Uniforms.h"
#include "primitives/Mesh.h"
#include "physics/PhysicsHandler.h"
#include "physics/RigidBody.h"

Settings g_settings;

int main() {

    Time &time = Time::Instance();
    Renderer &renderer = Renderer::Instance();
    PhysicsHandler &physicsHandler = PhysicsHandler::Instance();

    // Material basicMaterial("/shaders/Basic");
    // Material vertexColorMaterial("/shaders/VertexColors");

    Material colorMaterial("/shaders/Color");
    Material phongMaterial("/shaders/Phong");
    
    auto lightDirection = new Uniform<glm::vec3>("u_lightDirection", glm::vec3(0.5f, 0.0f, 2.0f));
    phongMaterial.assignUniform(lightDirection);

    std::random_device rd; std::mt19937 gen(rd()); std::uniform_real_distribution<> randboy(0, 10);

    for (int i = 0; i < 120; i++) {
        Mesh* mesh = new SphereMesh(0.5f, 22);
        mesh->setPosition(randboy(gen) - 5, 1.0f + randboy(gen), randboy(gen) - 5);
        // mesh->setPosition(1.0f, 2.0f + i, randboy(gen)/10);
        mesh->assignMaterial(phongMaterial);
        renderer.Enqueue(mesh);

        RigidBody* rigidBod = new RigidBody(mesh);
        rigidBod->collider = new SphereCollider(0.5f);
        rigidBod->bounciness = 0.88f;
        physicsHandler.Enqueue(rigidBod);
    }

    Mesh* myTetra = new TetrahedronMesh(1.0f);
    myTetra->setPosition(0.0f, 1.0f, 0.0f);
    myTetra->assignMaterial(colorMaterial);
    renderer.Enqueue(myTetra);

    // myTetra->setRotation(-90.0f, 0.0f, 0.0f);
    // RigidBody* tetraBod = new RigidBody(myTetra);
    // tetraBod->collider = new MeshCollider(PrimitiveMesh(PrimitiveGenerator::Tetrahedron::generate(1.0f)));
    // tetraBod->mass = 100.0f;
    // physicsHandler.Enqueue(tetraBod);

    Mesh* floor = new PlaneMesh(255.0f);
    floor->setRotation(-90.0f, 0.0f, 0.0f);

    Material floorMaterial("/shaders/Phong", {
        new Uniform<glm::vec3>("ambient", glm::vec3(0.2f, 0.3f, 0.3f)),
        new Uniform<glm::vec3>("diffuseAlbedo", glm::vec3(0.2f, 0.3f, 0.3f)),
        new Uniform<glm::vec3>("specularAlbedo", glm::vec3(0.2f, 0.2f, 0.2f)),
        new Uniform<glm::vec3>("rimColor", glm::vec3(0.2f, 0.3f, 0.3f)),
        new Uniform<int>("rimLightOn", 1),
        new Uniform<float>("shininess", 20.0f),
        new Uniform<float>("rimPower", 200.0f),
        lightDirection
    });
    floor->assignMaterial(floorMaterial);
    renderer.Enqueue(floor);
    
    RigidBody* floorCollider = new RigidBody(floor);
    floorCollider->collider = new PlaneCollider(glm::vec2(255.0f, 255.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    floorCollider->mass = 100.0f;
    floorCollider->isDynamic = false;
    physicsHandler.Enqueue(floorCollider);

    while (renderer.isActive()) {
        renderer.BeginLoop();
        
        physicsHandler.update();

        // if(time.time < 10) {
        //     for (auto& body: physicsHandler.bodies) {
        //         body->applyForce(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(2.0f, 0.0f, 2.0f));
        //     }
        // }

        float oscillator = sin(time.time*1.5) / 2.0f + 0.5f;
        colorMaterial.setUniform("u_color", glm::vec4(0.0f, oscillator, 0.8f, 1.0f)); 
        lightDirection->set({ oscillator * 4.0f - 2.0f, 1.0f, 0.0f });

        renderer.EndLoop();
    }

    return 0;
}
