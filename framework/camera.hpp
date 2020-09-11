#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec3.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/string_cast.hpp>

class Camera
{
    public:
        Camera(){};
        Camera(std::string name, float angle_x) :
        name_{name},
        fov_x_{angle_x}
        {};
        Camera(std::string name, float angle_x, glm::vec3 eye_pos, glm::vec3 dir, glm::vec3 up) :
        name_{name},
        fov_x_{angle_x},
        cam_pos_{eye_pos},
        cam_direction_{dir},
        up_{up}
        {
            // u = cam_direction_ x up, v = u x cam_direction_
            glm::vec3 u = glm::cross(glm::normalize(cam_direction_),glm::normalize(up_));
            glm::vec3 v = glm::cross(glm::normalize(u),glm::normalize(cam_direction_));
            u = glm::normalize(u);
            v = glm::normalize(v);
            world_transformation_ = glm::mat4{
                            glm::vec4{u.x,u.y,u.z,0},
                            glm::vec4{v.x,v.y,v.z,0},
                            glm::vec4{-cam_direction_.x,-cam_direction_.y,-cam_direction_.z,0},
                            glm::vec4{cam_pos_.x,cam_pos_.y,cam_pos_.z,1.0f}
                            };
        };
        
        std::string name_ = "NoName";
        float fov_x_ = 45.0f;
        glm::vec3 cam_pos_ = {0,0,0};
        glm::vec3 cam_direction_ = {0,0,-1.0f};
        glm::vec3 up_ = {0,1.0f,0};
        glm::mat4 world_transformation_;
};

#endif // CAMERA_HPP