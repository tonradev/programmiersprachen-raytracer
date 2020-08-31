#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec3.hpp>

class Camera
{
    public:
        Camera(){};
        Camera(std::string name, float angle_x) :
        name_{name},
        fov_x_{angle_x}
        {};
        glm::vec3 cam_pos_ = {0,0,0};
        glm::vec3 cam_direction_ = {0,0,-1.0f};
        std::string name_ = "NoName";
        float fov_x_ = 45.0f;
};

#endif // CAMERA_HPP