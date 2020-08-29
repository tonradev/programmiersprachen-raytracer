#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec3.hpp>

class Camera
{
    public:
        Camera(glm::vec3 const& cam_pos, glm::vec3 const& plane_pos, int plane_width, int plane_height) :
        cam_pos_{cam_pos},
        plane_pos_{plane_pos},
        plane_width_{plane_width},
        plane_height_{plane_height}
        {};
        glm::vec3 cam_pos_;
        glm::vec3 cam_direction_;
        glm::vec3 plane_pos_;
        int plane_width_;
        int plane_height_;
};

#endif // CAMERA_HPP