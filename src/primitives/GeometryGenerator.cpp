#include "GeometryGenerator.h"

PrimitiveMesh::PrimitiveMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
    : vertices(vertices), indices(indices)
{}

PrimitiveMesh GeometryGenerator::Plane(const float &length, const float &width) {
    
    float x_dist = length/2;
    float y_dist = width/2;
    
    std::vector<Vertex> geometry {
        Vertex(glm::vec3( -x_dist, -y_dist, 0), glm::vec3(0.0f, 0.0f, 1.0f)),
        Vertex(glm::vec3(  x_dist, -y_dist, 0), glm::vec3(0.0f, 0.0f, 1.0f)),
        Vertex(glm::vec3( -x_dist,  y_dist, 0), glm::vec3(0.0f, 0.0f, 1.0f)),
        Vertex(glm::vec3(  x_dist,  y_dist, 0), glm::vec3(0.0f, 0.0f, 1.0f)),
    }; 

    std::vector<unsigned int> indices = {
        0, 1, 2,
        1, 3, 2
    };

    return PrimitiveMesh(geometry, indices);
}

PrimitiveMesh GeometryGenerator::Tetrahedron(const float &size) {
    
    float sqrt8over9 = 0.9428090415820633658 * size;
    float sqrt2over9 = 0.4714045207910316829 * size;
    float sqrt2over3 = 0.8164965809277260327 * size;
    float oneThird = 0.333333333333333333 * size;

    std::vector<Vertex> geometry {
        Vertex(glm::vec3(0, -oneThird, sqrt8over9)),
        Vertex(glm::vec3(sqrt2over3, -oneThird, -sqrt2over9)), 
        Vertex(glm::vec3(-sqrt2over3, -oneThird, -sqrt2over9)),
        Vertex(glm::vec3(0, size, 0)),
    };

    std::vector<unsigned int> indices = {
        2, 1, 0,
        1, 2, 3,
        0, 3, 2,
        1, 3, 0
    };

    return PrimitiveMesh(geometry, indices);
}


PrimitiveMesh GeometryGenerator::Box(const float &width, const float &height, const float &depth) {
    
    // @TODO add normals

    float x_dist = width  / 2;
    float y_dist = height / 2;
    float z_dist = depth  / 2;
    
    std::vector<Vertex> box {
        Vertex(glm::vec3(-x_dist, -y_dist, -z_dist)), // Front
        Vertex(glm::vec3( x_dist, -y_dist, -z_dist)), // Front
        Vertex(glm::vec3( x_dist,  y_dist, -z_dist)), // Front
        Vertex(glm::vec3(-x_dist,  y_dist, -z_dist)), // Front
        Vertex(glm::vec3( x_dist, -y_dist,  z_dist)), // Back
        Vertex(glm::vec3( x_dist,  y_dist,  z_dist)), // Back
        Vertex(glm::vec3(-x_dist,  y_dist,  z_dist)), // Back
        Vertex(glm::vec3(-x_dist, -y_dist,  z_dist)), // Back

        // Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        // Vertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        // Vertex(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        // Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        // Vertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        // Vertex(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        // Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        // Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        // Vertex(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        // Vertex(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        // Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        // Vertex(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        // Vertex(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        // Vertex(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        // Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        // Vertex(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        // Vertex(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
        // Vertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
        // Vertex(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
        // Vertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
        // Vertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        // Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        // Vertex(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        // Vertex(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
    }; 

    std::vector<unsigned int> indices = {
        2, 1, 0, // Front
        3, 2, 0, // Front
        1, 5, 4, // Right
        1, 2, 5, // Right
        4, 5, 6, // Back
        4, 6, 7, // Back
        7, 6, 3, // Left
        7, 3, 0, // Left
        0, 1, 7, // Bottom
        1, 4, 7, // Bottom
        2, 3, 6, // Top
        2, 6, 5, // Top
    };

    return PrimitiveMesh(box, indices);
}

PrimitiveMesh GeometryGenerator::Sphere(const float &size, const unsigned int segments) {

    // http://www.songho.ca/opengl/gl_sphere.html
    float radius = size/2;
    int sectorCount = segments;
    int stackCount = segments;
    
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
    float s, t;                                     // texCoord

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    std::vector<Vertex> geometry;
    std::vector<unsigned int> indices;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;      // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            // // vertex tex coord between [0, 1]
            // s = (float)j / sectorCount;
            // t = (float)i / stackCount;

            geometry.push_back(Vertex(
                glm::vec3(x, y, z),
                glm::vec3(nx, ny, nz)
            ));
            
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if(i != 0)
            {
                // addIndices(k1, k2, k1+1);   // k1---k2---k1+1
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1+1);
            }

            if(i != (stackCount-1))
            {
                // addIndices(k1+1, k2, k2+1); // k1+1---k2---k2+1
                indices.push_back(k1+1);
                indices.push_back(k2);
                indices.push_back(k2+1);
            }
        }
    }

    return PrimitiveMesh(geometry, indices);
}

PrimitiveMesh GeometryGenerator::Arrow(const float& size) {
    
    std::vector<Vertex> geometry = { 
        Vertex(glm::vec3(0, 0, 0)), Vertex(glm::vec3(0, size, 0)),
        Vertex(glm::vec3(0, size, 0)), Vertex(glm::vec3(-0.2 * size, 0.8 * size, 0)),
        Vertex(glm::vec3(0, size, 0)), Vertex(glm::vec3( 0.2 * size, 0.8 * size, 0))
    };

    return PrimitiveMesh(geometry);
}
