#include "Collider.h"

void Collider::setGeometry(const PrimitiveMesh& geometry) {

    this->vertices = geometry.vertices;

    if(geometry.indices.size() > 0) {
        this->indices = geometry.indices;

        for(int i = 0; i < this->indices.size(); i++) {
            if (std::find(this->uniqueIndices.begin(), this->uniqueIndices.end(), this->indices[i]) == this->uniqueIndices.end()) {
                this->uniqueIndices.push_back(this->indices[i]);
            }
        }

    } else if (geometry.vertices.size() > 0) {
        
        for (int i = 0; i < geometry.vertices.size(); i++) {
            std::cout << "Generating collider indices #INEFFICIENT" << std::endl;
            this->indices.push_back(i);
            this->uniqueIndices.push_back(i);
        }

    }
}
void Collider::updateRotation(const glm::quat& rotation) { }

BoxCollider::BoxCollider(const glm::vec3 &size) {
    this->colliderType = ColliderType::Box;
    // this->setGeometry(GeometryGenerator::Box(size.x, size.y, size.z));
}

PlaneCollider::PlaneCollider(const glm::vec2 &size, const glm::vec3 &normal) {
    this->colliderType = ColliderType::Plane;
    this->size = size;
    this->normal = glm::normalize(normal);
    this->normalRef = glm::normalize(normal);
}
void PlaneCollider::updateRotation(const glm::quat& rotation) {
    // this->normal = rotation * glm::vec3(0.0f, 0.0f, 1.0f);
    this->normal = rotation * this->normalRef;
}

SphereCollider::SphereCollider(const float &diameter) {
    this->colliderType = ColliderType::Sphere;
    this->radius = diameter/2;
}

MeshCollider::MeshCollider(PrimitiveMesh convexMesh) {
    this->colliderType = ColliderType::ConvexMesh;
    this->setGeometry(convexMesh);
}

MeshCollider::MeshCollider(Mesh* mesh) {
    this->colliderType = ColliderType::ConvexMesh;
    this->setGeometry(PrimitiveMesh(mesh->vertexBuffer.vertices, mesh->indexBuffer.indices));
}
