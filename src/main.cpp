#include <random>
#include "./common.h"
#include "core/Renderer.h"
#include "core/Shader.h"
#include "core/Uniforms.h"
#include "primitives/Mesh.h"
#include "physics/PhysicsHandler.h"
#include "physics/RigidBody.h"
#include "physics/PhysicsSolver.h"
#include "math/Quaternion.h"

Settings g_settings;

int main() {

    Time &time = Time::Instance();
    Renderer &renderer = Renderer::Instance();
    PhysicsHandler &physicsHandler = PhysicsHandler::Instance();

    // Material vertexColorMaterial("/shaders/VertexColors");

    Material* colorMaterial = new Material("/shaders/Color");
    Material* phongMaterial = new Material("/shaders/Phong");
    
    auto lightDirection = new Uniform<glm::vec3>("u_lightDirection", glm::vec3(0.5f, 0.0f, 2.0f));
    phongMaterial->assignUniform(lightDirection);

    std::random_device rd; std::mt19937 gen(rd()); std::uniform_real_distribution<> randboy(0, 10);

    // for (int i = 0; i < 50; i++) {
    //     float size = randboy(gen)/11 + 0.1f;
    //     Mesh* mesh = new SphereMesh(size, 22);
    //     mesh->setPosition(randboy(gen) - 5, 1.0f + randboy(gen), randboy(gen) - 5);
    //     // mesh->setPosition(1.0f, 2.0f + i, randboy(gen)/10);
    //     mesh->assignMaterial(phongMaterial);
    //     renderer.Enqueue(mesh);

    //     RigidBody* rigidBod = new RigidBody(mesh);
    //     rigidBod->collider = new SphereCollider(size);
    //     rigidBod->mass = size;
    //     rigidBod->bounciness = 0.90f;
    //     physicsHandler.Enqueue(rigidBod);
    // }

    // for (int i = 0; i < 1; i++) {
        Mesh* myTetra = new TetrahedronMesh(1.0f);
        myTetra->setPosition({ 0.0f, 1.5f, 0.0f });
        // myTetra->setPosition(randboy(gen) - 5, 1.0f + randboy(gen), randboy(gen) - 5);
        // myTetra->setScale({0.5f, 1.0f, 0.5f});
        myTetra->assignMaterial(colorMaterial);
        renderer.Enqueue(myTetra);

        // myTetra->setRotation({ -120.0f, 20.0f, 0.0f });
        // myTetra->setRotation(randboy(gen) * 11, randboy(gen) * 11, randboy(gen) * 11);
        RigidBody* tetraBod = new RigidBody(myTetra);
        tetraBod->collider = new MeshCollider(PrimitiveMesh(PrimitiveGenerator::Tetrahedron::generate(1.0f)));
        tetraBod->mass = 1.0f;
        // tetraBod->gravity = 0.0f;
        // tetraBod->angularVelocity = glm::vec3(3.0f, 0.0f, 0.0f);
        physicsHandler.Enqueue(tetraBod);
    // }

    Mesh* floor = new PlaneMesh(255.0f);
    floor->setRotation({ -90.0f, 0.0f, 0.0f });

    Material* floorMaterial = new Material("/shaders/Phong", {
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
    
    RigidBody* floorCollider = new RigidBody(floor, new PlaneCollider(glm::vec2(5.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    floorCollider->makeStatic();
    // floorCollider->gravity = 0.0f;
    // floorCollider->mass = 500.0f;
    // floorCollider->inertiaTensor = glm::mat3(5000.0f);
    physicsHandler.Enqueue(floorCollider);

    Mesh debugMesh = Mesh({ Vertex(glm::vec3(0, 0, 0)), Vertex(glm::vec3(0, 1, 0)) });
    debugMesh.assignMaterial(renderer.defaultShader);
    
    while (renderer.isActive()) {
        renderer.BeginLoop();
        physicsHandler.update();
        
        float oscillator = sin(time.time*1.5) / 2.0f + 0.5f;
        colorMaterial->setUniform("u_color", glm::vec4(0.0f, oscillator, 0.8f, 1.0f)); 
        lightDirection->set({ oscillator * 4.0f - 2.0f, 1.0f, 0.0f });

        // if(time.time < 2) {
        //     for (auto& body: physicsHandler.bodies) {
        //         body->applyForce(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f));
        //     }
        // }
        
        if(glfwGetKey(renderer.window, GLFW_KEY_I) == GLFW_PRESS) {
            tetraBod->angularVelocity.x += 0.1f;
        }
        if(glfwGetKey(renderer.window, GLFW_KEY_K) == GLFW_PRESS) {
            tetraBod->angularVelocity.x -= 0.1f;
        }
        if(glfwGetKey(renderer.window, GLFW_KEY_J) == GLFW_PRESS) {
            tetraBod->angularVelocity.y += 0.1f;
        }
        if(glfwGetKey(renderer.window, GLFW_KEY_L) == GLFW_PRESS) {
            tetraBod->angularVelocity.y -= 0.1f;
        }
        if(glfwGetKey(renderer.window, GLFW_KEY_U) == GLFW_PRESS) {
            tetraBod->angularVelocity.z += 0.1f;
        }
        if(glfwGetKey(renderer.window, GLFW_KEY_O) == GLFW_PRESS) {
            tetraBod->angularVelocity.z -= 0.1f;
        }

        glm::vec3 localTetraPoint = glm::vec3(0, -0.333333333333333333, 0.9428090415820633658);
        glm::vec3 worldTetraPoint = (tetraBod->rotation * localTetraPoint) + tetraBod->position;

        auto localPointVel = PhysicsSolver::getPointVelocity(localTetraPoint, glm::vec3(0.0f), tetraBod->angularVelocity);
        auto worldPointVel = PhysicsSolver::getPointVelocity(worldTetraPoint, glm::vec3(0.0f), tetraBod->angularVelocity);

        debugMesh.setPosition(tetraBod->position);
        debugMesh.setRotation(Quaternion::createFromUnitVectors(
            glm::vec3(0.0f, 1.0f, 0.0f),
            (tetraBod->rotation * tetraBod->angularVelocity) // To world
            // tetraBod->inverseRotation * (tetraBod->rotation * tetraBod->angularVelocity) // To world and back to local
        ));
        debugMesh.setScale(glm::vec3(1.0f, 0.05f + glm::length(tetraBod->angularVelocity) * 0.5f, 1.0f));

        debugMesh.Bind();
        glUniformMatrix4fv(debugMesh.material->shader->getUniformLocation("u_modelViewProjectionMatrix"), 1, GL_FALSE, 
            glm::value_ptr(renderer.camera->viewProjectionMatrix * debugMesh.getWorldPositionMatrix())
        );
        glDrawArrays(GL_LINES, 0, debugMesh.vertexBuffer.getCount());

        renderer.EndLoop();
    }

    return 0;
}
