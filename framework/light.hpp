#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/vec3.hpp>
#include <string>
#include <color.hpp>

struct Light {
    std::string name;
    glm::vec3 pos;
    Color color;
    float brightness;
};

#endif // LIGHT_HPP