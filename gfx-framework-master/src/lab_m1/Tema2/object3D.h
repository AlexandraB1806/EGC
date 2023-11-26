#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace object3D
{
    Mesh* CreateCar(const std::string& name, glm::vec3 center, float length, float height, float width, glm::vec3 color);
    Mesh* CreateTree(const std::string& name);
    std::vector<Mesh*> CreateGrass(glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 corner4);
    Mesh* CreateTrack(const std::string& name, std::vector<glm::vec3>& outer, std::vector<glm::vec3>& inner,
                        std::vector<VertexFormat>& moreVertices,
                        std::vector<glm::vec3>& innerTree, std::vector<glm::vec3>& outerTree);
}
