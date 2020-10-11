#include "Collider.h"

Collider::Collider() {

    PrimitiveMesh geometry = PrimitiveGenerator::Box::generate(0.5f, 0.5f, 0.5f);

    if(geometry.indices.size() > 0) {
        
        for (int i = 0; i < geometry.indices.size(); i += 2) {
            if( (i+2) < geometry.indices.size()) {
                this->polygons.push_back( Polygon({ geometry.vertices[i], geometry.vertices[i+1], geometry.vertices[i+2] }) );
            }
        };

    } else {

        // for (int i = 0; i < geometry.vertices.size(); i += 3) {
        //     if( (i+2) < geometry.vertices.size()) {
        //         this->polygons.push_back( Polygon({ geometry.vertices[i], geometry.vertices[i+1], geometry.vertices[i+2] }) );
        //     }
        // }

    }
}
