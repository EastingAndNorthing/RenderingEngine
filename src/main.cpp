#include <random>
#include "./common.h"
#include "core/Renderer.h"
#include "core/Shader.h"
#include "core/Uniforms.h"
#include "primitives/Mesh.h"
#include "physics/PhysicsHandler.h"
#include "physics/RigidBody.h"
#include "math/Quaternion.h"
#include "math/CoordinateSystem.h"

Settings g_settings;

int main() {

    Time &time = Time::Instance();
    Renderer &renderer = Renderer::Instance();
    PhysicsHandler &physicsHandler = PhysicsHandler::Instance();

    auto lightDirection = new Uniform<glm::vec3>("u_lightDirection", glm::vec3(0.5f, 0.0f, 2.0f));

    Material* colorMaterial = new Material("/shaders/Color");
    Material* phongMaterial = new Material("/shaders/Phong");
    phongMaterial->assignUniform(lightDirection);
    // Material* floorMaterial = new Material("/shaders/Phong", {
    //     new Uniform<glm::vec3>("ambient", glm::vec3(0.2f, 0.3f, 0.3f)),
    //     new Uniform<glm::vec3>("diffuseAlbedo", glm::vec3(0.2f, 0.3f, 0.3f)),
    //     new Uniform<glm::vec3>("specularAlbedo", glm::vec3(0.2f, 0.2f, 0.2f)),
    //     new Uniform<glm::vec3>("rimColor", glm::vec3(0.2f, 0.3f, 0.3f)),
    //     new Uniform<int>("rimLightOn", 1),
    //     new Uniform<float>("shininess", 20.0f),
    //     new Uniform<float>("rimPower", 200.0f),
    //     lightDirection
    // });

    std::random_device rd; std::mt19937 gen(rd()); std::uniform_real_distribution<> randboy(0, 10);

    // for (int i = 0; i < 300; i++) {
    //     float size = randboy(gen)/11 + 0.1f;
    //     Mesh* mesh = new SphereMesh(size);
    //     mesh->setPosition({ randboy(gen) - 5, 1.0f + randboy(gen), randboy(gen) - 5 });
    //     mesh->setMaterial(phongMaterial);
    //     renderer.Enqueue(mesh);

    //     RigidBody* rigidBod = new RigidBody(mesh);
    //     rigidBod->collider = new SphereCollider(size);
    //     rigidBod->bounciness = 1.0f;
    //     physicsHandler.Enqueue(rigidBod);
    // }

    // Multiple Tetrahedra ////////////////////////////////////////////////////////////////////////
    // for (int i = 0; i < 10; i++) {
    //     Mesh* myMesh = new TetrahedronMesh(1.0f);
    //     myMesh->setPosition({ randboy(gen) - 5, 10.0f + randboy(gen), randboy(gen) - 5 });
    //     myMesh->setMaterial(colorMaterial);
    //     renderer.Enqueue(myMesh);

    //     myMesh->setRotation({ randboy(gen) * 18, randboy(gen) * 18, randboy(gen) * 18 });
    //     RigidBody* myBody = new RigidBody(myMesh);
    //     myBody->collider = new MeshCollider(PrimitiveMesh(PrimitiveGenerator::Tetrahedron(1.0f)));
    //     physicsHandler.Enqueue(myBody);
    // }

    // Single tetrahedron /////////////////////////////////////////////////////////////////////////

    Mesh* myMesh = new TetrahedronMesh(1.0f);
    myMesh->setPosition({ 3.0f, 7.0f, 0.0f });
    // myMesh->setRotation({ -120.0f, 20.0f, 0.0f });
    myMesh->setMaterial(colorMaterial);
    renderer.Enqueue(myMesh);

    RigidBody* myBody = new RigidBody(myMesh);
    myBody->collider = new MeshCollider(PrimitiveMesh(PrimitiveGenerator::Tetrahedron(1.0f)));
    // myBody->bounciness = 1.0f;
    // myBody->gravity = 0.0f;
    myBody->invMass = 1.0f;
    myBody->invInertia = glm::vec3(1.0f);
    physicsHandler.Enqueue(myBody);

    // Floor //////////////////////////////////////////////////////////////////////////////////////
    Mesh* floor = new PlaneMesh(10.0f);
    floor->setRotation({ -90.0f, 0.0f, 0.0f });
    floor->setMaterial(colorMaterial);
    renderer.Enqueue(floor);
    
    RigidBody* floorCollider = new RigidBody(floor, new PlaneCollider(glm::vec2(10.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    floorCollider->makeStatic();
    // floorCollider->gravity = 0.0f;
    // floorCollider->invMass = 0.001f;
    // floorCollider->invInertia = glm::vec3(0.001f);
    physicsHandler.Enqueue(floorCollider);

    Mesh debugMesh = Mesh(PrimitiveGenerator::Arrow());
    debugMesh.setMaterial(renderer.defaultShader);
    
    while (renderer.isActive()) {

        renderer.BeginLoop();
        
        if(glfwGetKey(renderer.window, GLFW_KEY_UP) == GLFW_PRESS)   { time.faster(); };
        if(glfwGetKey(renderer.window, GLFW_KEY_DOWN) == GLFW_PRESS) { time.slower(); };
        if(glfwGetKey(renderer.window, GLFW_KEY_M) == GLFW_PRESS) { time.setStepMode(true); };
        if(glfwGetKey(renderer.window, GLFW_KEY_N) == GLFW_PRESS) { time.setStepMode(false); };
    
        if(!time.isStepMode || glfwGetKey(renderer.window, GLFW_KEY_P) == GLFW_PRESS) { physicsHandler.update(); };

        if(glfwGetKey(renderer.window, GLFW_KEY_SPACE) == GLFW_PRESS) { myBody->gravity = 0.0f; myBody->vel = glm::vec3(0.0f); myBody->omega = glm::vec3(0.0f); };
        if(glfwGetKey(renderer.window, GLFW_KEY_KP_4) == GLFW_PRESS) myBody->omega.x += 0.3f;
        if(glfwGetKey(renderer.window, GLFW_KEY_KP_6) == GLFW_PRESS) myBody->omega.x -= 0.3f;
        if(glfwGetKey(renderer.window, GLFW_KEY_KP_8) == GLFW_PRESS) myBody->omega.y += 0.3f;
        if(glfwGetKey(renderer.window, GLFW_KEY_KP_5) == GLFW_PRESS) myBody->omega.y -= 0.3f;
        if(glfwGetKey(renderer.window, GLFW_KEY_KP_7) == GLFW_PRESS) myBody->omega.z += 0.3f;
        if(glfwGetKey(renderer.window, GLFW_KEY_KP_9) == GLFW_PRESS) myBody->omega.z -= 0.3f;
        if(glfwGetKey(renderer.window, GLFW_KEY_L) == GLFW_PRESS) myBody->vel.x += 0.3f;
        if(glfwGetKey(renderer.window, GLFW_KEY_J) == GLFW_PRESS) myBody->vel.x -= 0.3f;
        if(glfwGetKey(renderer.window, GLFW_KEY_I) == GLFW_PRESS) myBody->vel.y += 0.3f;
        if(glfwGetKey(renderer.window, GLFW_KEY_K) == GLFW_PRESS) myBody->vel.y -= 0.3f;
        if(glfwGetKey(renderer.window, GLFW_KEY_U) == GLFW_PRESS) myBody->vel.z += 0.3f;
        if(glfwGetKey(renderer.window, GLFW_KEY_O) == GLFW_PRESS) myBody->vel.z -= 0.3f;

        glm::vec3 tetraPointL = { 0, -0.333333333333333333, 0.9428090415820633658 };
        glm::vec3 tetraPointW = CoordinateSystem::localToWorld(tetraPointL, myBody->pose.q, myBody->pose.p);

        glm::vec3 impulse = {0, 0.1f, 0};
        glm::vec3 impulsePos = tetraPointW;
        glm::vec3 v = myBody->getVelocityAt(tetraPointW);

        glm::vec3 force = glm::vec3(0, 20.0f, 0);
        glm::vec3 forcePos = tetraPointW;

        if(glfwGetKey(renderer.window, GLFW_KEY_Q) == GLFW_PRESS) {
            // myBody->applyCorrection(impulse, impulsePos, true);
            myBody->applyForce(force, forcePos);
        }

        debugMesh.setPosition(forcePos);
        debugMesh.setScale(glm::length(force) * 1.0f);
        debugMesh.setRotation(Quaternion::createFromTwoVectors(
            glm::vec3(0.0f, 1.0f, 0.0f),
            force
        ));

        debugMesh.Bind();
        glUniformMatrix4fv(debugMesh.material->shader->getUniformLocation("u_modelViewProjectionMatrix"), 1, GL_FALSE, 
            glm::value_ptr(renderer.camera->viewProjectionMatrix * debugMesh.getWorldPositionMatrix())
        );
        glDrawArrays(GL_LINES, 0, debugMesh.vertexBuffer.getCount());
        
        colorMaterial->setUniform("u_color", glm::vec4(0.0f, 1.0, 0.8f, 1.0f)); 
        // float oscillator = sin(time.time*1.5) / 2.0f + 0.5f;
        // colorMaterial->setUniform("u_color", glm::vec4(0.0f, oscillator, 0.8f, 1.0f)); 
        // lightDirection->set({ oscillator * 4.0f - 2.0f, 1.0f, 0.0f });

        renderer.EndLoop();
    }

    return 0;
}
