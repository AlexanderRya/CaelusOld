#ifndef CAELUS_MESH_HPP
#define CAELUS_MESH_HPP

#include "engine/core/Types.hpp"

#include <vector>

namespace caelus::core::components {
    struct Mesh {
        std::vector<types::Vertex> vertices;
        std::vector<glm::vec3> instances;


        u64 pipeline_id;
        //!TODO: Textures.
    };
} // namespace caelus::core::components

#endif //CAELUS_MESH_HPP
