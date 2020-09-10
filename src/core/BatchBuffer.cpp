#include "core/BatchBuffer.h"
#include "primitives/Mesh.h"


BatchBuffer::BatchBuffer() {
    
    // int size = 1024;

    // float x_dist = 1.0/2;
    // float y_dist = 1.0/2;
    // float z_dist = 1.0/2;
    
    // std::vector<Vertex> box {
    //     Vertex({-x_dist, -y_dist, -z_dist}), // Front
    //     Vertex({ x_dist, -y_dist, -z_dist}), // Front
    //     Vertex({ x_dist,  y_dist, -z_dist}), // Front
    //     Vertex({-x_dist,  y_dist, -z_dist}), // Front
    //     Vertex({ x_dist, -y_dist,  z_dist}), // Back
    //     Vertex({ x_dist,  y_dist,  z_dist}), // Back
    //     Vertex({-x_dist,  y_dist,  z_dist}), // Back
    //     Vertex({-x_dist, -y_dist,  z_dist}), // Back
    // }; 

    // std::vector<unsigned int> indices = {
    //     2, 1, 0, // Front
    //     3, 2, 0, // Front
    //     1, 5, 4, // Right
    //     1, 2, 5, // Right
    //     4, 5, 6, // Back
    //     4, 6, 7, // Back
    //     7, 6, 3, // Left
    //     7, 3, 0, // Left
    //     0, 1, 7, // Bottom
    //     1, 4, 7, // Bottom
    //     2, 3, 6, // Top
    //     2, 6, 5, // Top
    // };

    // glGenVertexArrays(1, &this->vao);
    // glBindVertexArray(this->vao);

    // glGenBuffers(1, &this->vbuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, this->vbuffer);
    // glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_DYNAMIC_DRAW);
    
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, position));
    // glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    // glEnableVertexAttribArray(1);

    // glGenBuffers(1, &this->ibo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &this->ibo);
    glGenBuffers(1, &this->dirtycontainer);

}

void BatchBuffer::addGeometry(Mesh* mesh) {
    
    std::cout << "Mesh vbuffer ID: " << mesh->vertexBuffer.vbuffer << std::endl;
    std::cout << "Dirty container ID: " << this->dirtycontainer << std::endl;

    this->indicesCount = mesh->vertexBuffer.getSize();

    GLint size = 0;
    glBindBuffer           (GL_ARRAY_BUFFER, mesh->vertexBuffer.vbuffer);
    glGetBufferParameteriv (GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
    
    std::cout << "GL_BUFFER_SIZE: " << size << std::endl;

    glBindBuffer           (GL_COPY_WRITE_BUFFER, this->dirtycontainer);
    glBufferData           (GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);

    glCopyBufferSubData    (GL_COPY_WRITE_BUFFER, GL_ARRAY_BUFFER, 0, 0, size);

    glBindBuffer           (GL_ARRAY_BUFFER, this->dirtycontainer);

    // glDeleteBuffers(1, &this->dirtycontainer);
    // glGenBuffers(1, &this->dirtycontainer);
    // std::cout << "Dirty: " << this->dirtycontainer << std::endl;

	// other.bind(GL_COPY_READ_BUFFER);
	// bind(GL_COPY_WRITE_BUFFER);
	// GLint size = 0;
	// glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
	// if (size != 0) {
	// 	glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);
	// 	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
	// }
    
    // glGenBuffers(1, &this->vbuffer);
    // glDeleteBuffers(1, &this->vbuffer);
    
    // std::vector<Vertex> vbufferData;
    // std::vector<Vertex> ibufferData;
    // glBindBuffer(GL_ARRAY_BUFFER, ibo);
    // glGetBufferSubData(GL_ARRAY_BUFFER, 0, vbufferData);
    
    // glBindBuffer(GL_ARRAY_BUFFER, ibo);
    // glGetBufferSubData(GL_ARRAY_BUFFER, 0, ibufferData);

}

void BatchBuffer::addGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    this->vertices.insert(this->vertices.end(), vertices.begin(), this->vertices.end());
    this->indices.insert(this->indices.end(), indices.begin(), this->indices.end());
}

void BatchBuffer::setMaterial(Material* material) {
    
}

void BatchBuffer::Bind() {
    glBindVertexArray(this->vao); 
    glBindBuffer(GL_ARRAY_BUFFER, this->vao);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);

    // Testing
    glDrawElements(GL_TRIANGLES, this->indicesCount, GL_UNSIGNED_INT, nullptr);
    // GLuint size = 0;
    // glDrawArrays(GL_TRIANGLES, 0, 3);

}

void BatchBuffer::UnBind() {
    
}

BatchBuffer::~BatchBuffer() {
    glDeleteBuffers(1, &this->vbuffer);
    glDeleteBuffers(1, &this->vao);
    glDeleteBuffers(1, &this->ibo);
}