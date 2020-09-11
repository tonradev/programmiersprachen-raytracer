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
        Ray transformRay(glm::mat4 const& mat) {
            // mat == inv. of transformation matrix
            glm::vec4 ray_origin_transf = mat*glm::vec4{origin_.x,origin_.y,origin_.z,1.0f};
            glm::vec4 ray_direction_transf = mat*glm::vec4{direction_.x,direction_.y,direction_.z,0};
            // Ray can only take vec3, drop 4th dimension
            return Ray{glm::vec3{ray_origin_transf},glm::vec3{ray_direction_transf}};
        }
};

#endif //#define RAY_HPP