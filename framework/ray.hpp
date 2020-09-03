#ifndef RAY_HPP
#define RAY_HPP

#include <glm/vec3.hpp>
#include <glm/gtx/intersect.hpp>

class Ray
{
    public:
        Ray(glm::vec3 const& origin, glm::vec3 const& direction) :
            origin_{origin},
            direction_{direction}
            {
                direction_ = glm::normalize(direction_);
            }
        glm::vec3 origin_ = {0.0f, 0.0f, 0.0f};
        glm::vec3 direction_ = {0.0f, 0.0f, -1.0f};
};

#endif //#define RAY_HPP