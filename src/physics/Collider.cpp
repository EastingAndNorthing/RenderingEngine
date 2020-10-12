#include "Collider.h"

void Collider::setPolygons(const PrimitiveMesh& geometry) {

    if(geometry.indices.size() > 0) {
        
        for (int i = 0; i < geometry.indices.size(); i += 2) {
            if( (i+2) < geometry.indices.size()) {
                this->polygons.push_back( Polygon({ geometry.vertices[i], geometry.vertices[i+1], geometry.vertices[i+2] }) );
            }
        };

    } else if (geometry.vertices.size() > 0) {

        // for (int i = 0; i < geometry.vertices.size(); i += 3) {
        //     if( (i+2) < geometry.vertices.size()) {
        //         this->polygons.push_back( Polygon({ geometry.vertices[i], geometry.vertices[i+1], geometry.vertices[i+2] }) );
        //     }
        // }

    }
}

BoxCollider::BoxCollider(const glm::vec3 &size) {
    this->colliderType = ColliderType::Box;
    this->setPolygons(PrimitiveGenerator::Box::generate(size.x, size.y, size.z));
}

PlaneCollider::PlaneCollider(const glm::vec2 &size, const glm::vec3 &normal) {
    this->colliderType = ColliderType::Plane;
    this->size = size;
    this->normal = normal;
}

SphereCollider::SphereCollider(const float &diameter) {
    this->colliderType = ColliderType::Sphere;
    this->radius = diameter/2;
}
