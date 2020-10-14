#include "Collider.h"
#include <algorithm>

void Collider::setGeometry(const PrimitiveMesh& geometry) {

    this->vertices = geometry.vertices;

    if(geometry.indices.size() > 0) {
        this->indices = geometry.indices;

        for(int i = 0; i < this->indices.size(); i++) {

            if (std::find(this->uniqueIndices.begin(), this->uniqueIndices.end(), this->indices[i]) == this->uniqueIndices.end()) {
                this->uniqueIndices.push_back(this->indices[i]);
            }
            // Vertex v = vertices[indices[i]];
            // std::cout << indices[i] << std::endl;
            // std::cout << v.position.x << " " << v.position.y << " " << v.position.z << " " << std::endl;
        }

        // for (int i = 0; i < this->indices.size(); i += 3) {
        //     // if( (i+2) < this->indices.size()) {
        //     // std::cout << "Indices: " << this->indices[i] << " " << this->indices[i+1] << " " << this->indices[i+2] << std::endl;
        //     //     // this->polygons.push_back( Polygon({ geometry.vertices[i], geometry.vertices[i+1], geometry.vertices[i+2] }) );
        //     // }
        // };

    } else if (geometry.vertices.size() > 0) {
        
        for (int i = 0; i < geometry.vertices.size(); i++) {
            std::cout << "Generating collider indices" << std::endl;
            this->indices.push_back(i);
        }

    }
}

BoxCollider::BoxCollider(const glm::vec3 &size) {
    this->colliderType = ColliderType::Box;
    // this->setGeometry(PrimitiveGenerator::Box::generate(size.x, size.y, size.z));
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

MeshCollider::MeshCollider(PrimitiveMesh convexMesh) {
    this->colliderType = ColliderType::ConvexMesh;
    this->setGeometry(convexMesh);
}
