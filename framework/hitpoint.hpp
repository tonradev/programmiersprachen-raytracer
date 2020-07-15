#ifndef HITPOINT_HPP
#define HITPOINT_HPP

#include "color.hpp"
#include "ray.hpp"
#include <glm/vec3.hpp>


struct HitPoint
{
    bool intersect = false;
    float distance = 0.0f;
    std::string object_name = "";
    Color object_color = {0.0f, 0.0f, 0.0f};
    glm::vec3 intersection_point = {0.0f, 0.0f, 0.0f};
    glm::vec3 direction = {0.0f, 0.0f, 0.0f};
};

#endif //#define HITPOINT_HPP