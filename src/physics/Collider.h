#pragma once

#include "common.h"
#include "primitives/Polygon.h"
#include "primitives/PrimitiveGenerator.h"

enum ColliderType {
    Box, Plane, Sphere, CustomGeometry
};

struct Collider {
public:

    ColliderType colliderType = CustomGeometry;

    std::vector<Polygon> polygons;

    glm::vec3 relativePos = glm::vec3(0.0f);

    Collider() = default;
    ~Collider() = default;
    
    void setPolygons(const PrimitiveMesh& geometry);

};

struct BoxCollider : public Collider {
    glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
    BoxCollider(const glm::vec3 &size);
};

struct PlaneCollider : public Collider {
    glm::vec2 size = glm::vec2(1.0f, 1.0f);
    glm::vec3 normal = glm::vec3(0.0f);
    PlaneCollider(const glm::vec2 &size, const glm::vec3 &normal = glm::vec3(0.0f));
};

struct SphereCollider : public Collider {
    float radius = 1.0f;
    SphereCollider(const float &radius);
};
