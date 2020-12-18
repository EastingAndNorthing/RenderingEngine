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
#include "math/CoordinateSystem.h"

Settings g_settings;

int main() {

    Time &time = Time::Instance();
    Renderer &renderer = Renderer::Instance();
    PhysicsHandler &physicsHandler = PhysicsHandler::Instance();

    Material* colorMaterial = new Material("/shaders/Color");
    Material* phongMaterial = new Material("/shaders/Phong");
    
    auto lightDirection = new Uniform<glm::vec3>("u_lightDirection", glm::vec3(0.5f, 0.0f, 2.0f));
    phongMaterial->assignUniform(lightDirection);

    std::random_device rd; std::mt19937 gen(rd()); std::uniform_real_distribution<> randboy(0, 10);

    // Multiple Tetrahedra ////////////////////////////////////////////////////////////////////////
    // for (int i = 0; i < 300; i++) {
    //     float size = randboy(gen)/11 + 0.1f;
    //     Mesh* mesh = new SphereMesh(size);
    //     mesh->setPosition({ randboy(gen) - 5, 1.0f + randboy(gen), randboy(gen) - 5 });
    //     mesh->setMaterial(phongMaterial);
    //     renderer.Enqueue(mesh);

    //     RigidBody* rigidBod = new RigidBody(mesh);
    //     rigidBod->collider = new SphereCollider(size);
    //     rigidBod->mass = size;
    //     rigidBod->bounciness = 1.0f;
    //     physicsHandler.Enqueue(rigidBod);
    // }

    // for (int i = 0; i < 5; i++) {
    //     Mesh* myTetra = new TetrahedronMesh(1.0f);
    //     myTetra->setPosition({ randboy(gen) - 5, 10.0f + randboy(gen), randboy(gen) - 5 });
    //     myTetra->setMaterial(colorMaterial);
    //     renderer.Enqueue(myTetra);

    //     myTetra->setRotation({ randboy(gen) * 18, randboy(gen) * 18, randboy(gen) * 18 });
    //     RigidBody* tetraBod = new RigidBody(myTetra);
    //     tetraBod->collider = new MeshCollider(PrimitiveMesh(PrimitiveGenerator::Tetrahedron(1.0f)));
    //     tetraBod->mass = 1.0f;
    //     tetraBod->bounciness = 0.10f;
    //     // tetraBod->gravity = 0.0f;
    //     physicsHandler.Enqueue(tetraBod);
    // }

    // Single tetrahedron /////////////////////////////////////////////////////////////////////////
    
    Mesh* myTetra = new TetrahedronMesh(1.0f);
    myTetra->setPosition({ 2.0f, 1.5f, 0.0f });
    myTetra->setMaterial(colorMaterial);
    renderer.Enqueue(myTetra);

    // myTetra->setRotation({ -120.0f, 20.0f, 0.0f });
    RigidBody* tetraBod = new RigidBody(myTetra);
    tetraBod->collider = new MeshCollider(PrimitiveMesh(PrimitiveGenerator::Tetrahedron(1.0f)));
    tetraBod->bounciness = 0.10f;
    tetraBod->gravity = 0.0f;
    physicsHandler.Enqueue(tetraBod);

    // Floor //////////////////////////////////////////////////////////////////////////////////////
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
    floor->setMaterial(floorMaterial);
    renderer.Enqueue(floor);
    
    RigidBody* floorCollider = new RigidBody(floor, new PlaneCollider(glm::vec2(5.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    floorCollider->makeStatic();
    physicsHandler.Enqueue(floorCollider);

    Mesh debugMesh = Mesh(PrimitiveGenerator::Arrow());
    debugMesh.setMaterial(renderer.defaultShader);
    
    while (renderer.isActive()) {
        renderer.BeginLoop();
        physicsHandler.update();
        
        colorMaterial->setUniform("u_color", glm::vec4(0.0f, 1.0, 0.8f, 1.0f)); 
        // float oscillator = sin(time.time*1.5) / 2.0f + 0.5f;
        // colorMaterial->setUniform("u_color", glm::vec4(0.0f, oscillator, 0.8f, 1.0f)); 
        // lightDirection->set({ oscillator * 4.0f - 2.0f, 1.0f, 0.0f });

        // if(time.time < 3)
        //     for (auto& body: physicsHandler.bodies)
        //         body->applyForce(glm::vec3(15.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f));
        
        if(glfwGetKey(renderer.window, GLFW_KEY_I) == GLFW_PRESS) tetraBod->angularVelocity.x += 0.1f;
        if(glfwGetKey(renderer.window, GLFW_KEY_K) == GLFW_PRESS) tetraBod->angularVelocity.x -= 0.1f;
        if(glfwGetKey(renderer.window, GLFW_KEY_J) == GLFW_PRESS) tetraBod->angularVelocity.y += 0.1f;
        if(glfwGetKey(renderer.window, GLFW_KEY_L) == GLFW_PRESS) tetraBod->angularVelocity.y -= 0.1f;
        if(glfwGetKey(renderer.window, GLFW_KEY_U) == GLFW_PRESS) tetraBod->angularVelocity.z += 0.1f;
        if(glfwGetKey(renderer.window, GLFW_KEY_O) == GLFW_PRESS) tetraBod->angularVelocity.z -= 0.1f;

        glm::vec3 impulse = {0, -0.1f, 0};
        glm::vec3 impulsePos = {0, 0, -1.0f};
        
        if(glfwGetKey(renderer.window, GLFW_KEY_Q) == GLFW_PRESS) tetraBod->applyWorldImpulse(impulse, impulsePos);

        glm::vec3 tetraPointL = { 0, -0.333333333333333333, 0.9428090415820633658 };
        auto tetraPointW = CoordinateSystem::localToWorld(tetraPointL, tetraBod->rotation, tetraBod->position);
        auto worldPointVel = PhysicsSolver::getWorldPointVelocity(tetraPointW, tetraBod->position, tetraBod->velocity, tetraBod->getAngularVelocityW());

        // debugMesh.setPosition(tetraPointW);
        // debugMesh.setRotation(Quaternion::createFromTwoVectors(
        //     glm::vec3(0.0f, 1.0f, 0.0f),
        //     worldPointVel
        // ));
        debugMesh.setPosition(tetraPointW);
        debugMesh.setScale(glm::length(worldPointVel) * 0.5f);
        debugMesh.setRotation(Quaternion::createFromTwoVectors(
            glm::vec3(0.0f, 1.0f, 0.0f),
            tetraBod->getPointVelocityW(tetraPointW)
        ));

        debugMesh.Bind();
        glUniformMatrix4fv(debugMesh.material->shader->getUniformLocation("u_modelViewProjectionMatrix"), 1, GL_FALSE, 
            glm::value_ptr(renderer.camera->viewProjectionMatrix * debugMesh.getWorldPositionMatrix())
        );
        glDrawArrays(GL_LINES, 0, debugMesh.vertexBuffer.getCount());

        renderer.EndLoop();
    }

    return 0;
}
