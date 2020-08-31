#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/vec3.hpp>
#include <string>

struct Material {
    std::string name;
    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    int m;
};

#endif // MATERIAL_HPP