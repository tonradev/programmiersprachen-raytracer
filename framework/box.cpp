#include <box.hpp>
#include <glm/gtx/intersect.hpp>
#include <iostream>

Box::Box(glm::vec3 const& min, glm::vec3 const& max, std::string name, Color color) :
    Shape(name, color),
    min_{min},
    max_{max}
{
    std::cout << "I'm the Box class constructor." << std::endl;
}

Box::~Box(){
    std::cout << "I'm the Box class destructor." << std::endl;
}

float Box::area() const {
    float b = max_[0]-min_[0];
    float l = min_[2]-max_[2];
    float h = max_[1]-min_[1];
    return 2*b*h+2*l*h+2*b*l;
}

float Box::volume() const {
    float b = max_[0]-min_[0];
    float l = min_[2]-max_[2];
    float h = max_[1]-min_[1];
    return b*h*l;
}


std::ostream& Box::print(std::ostream& os) const
{
    Shape::print(os);
    os << "I'm a box printed by the box print method!" << std::endl;
    return os;
}

bool Box::intersect(Ray const& r) const
{
    glm::vec3::value_type dist_front = 0.0;
    glm::vec3::value_type dist_back = 0.0;
    glm::vec3::value_type dist_top = 0.0;
    glm::vec3::value_type dist_bottom = 0.0;
    glm::vec3::value_type dist_left = 0.0;
    glm::vec3::value_type dist_right = 0.0;
    glm::vec3::value_type dist = 0.0f;
    glm::vec3::value_type min_dist = std::numeric_limits<float>::infinity();
    glm::vec3 ray_direction_normalized = glm::normalize(r.direction);
    bool intersect_front = glm::intersectRayPlane(r.origin,ray_direction_normalized,
                                                  min_,{0.0f,0.0f,1.0f},dist);
    if (dist < min_dist)
    {
        min_dist = dist;
    }
    bool intersect_back = glm::intersectRayPlane(r.origin,ray_direction_normalized,
                                                {min_.x,min_.y,max_.z},{0.0f,0.0f,1.0f},dist);
    if (dist < min_dist)
    {
        min_dist = dist;
    }
    bool intersect_top = glm::intersectRayPlane(r.origin,ray_direction_normalized,
                                                {min_.x,max_.y,min_.z},{0.0f,1.0f,0.0f},dist);
    if (dist < min_dist)
    {
        min_dist = dist;
    }
    bool intersect_bottom = glm::intersectRayPlane(r.origin,ray_direction_normalized,
                                                  min_,{0.0f,1.0f,0.0f},dist);
    if (dist < min_dist)
    {
        min_dist = dist;
    }
    bool intersect_left = glm::intersectRayPlane(r.origin,ray_direction_normalized,
                                                  min_,{1.0f,0.0f,0.0f},dist);
    if (dist < min_dist)
    {
        min_dist = dist;
    }
    bool intersect_right = glm::intersectRayPlane(r.origin,ray_direction_normalized,
                                                  {max_.x,min_.y,min_.z},{1.0f,0.0f,0.0f},dist);
    if (dist < min_dist)
    {
        min_dist = dist;
    }

    if (intersect_front || intersect_back || intersect_top || intersect_bottom || intersect_left || intersect_right) {
        glm::vec3 hitpt_front = {(r.origin+min_dist*r.direction)};
        // std::cout << "HITPOINT COORDINATES: X " << hitpt_front.x << " Y " << hitpt_front.y << " Z " << hitpt_front.z << std::endl;
        if (min_.y <= hitpt_front.y 
            && hitpt_front.y <= max_.y 
            && min_.x <= hitpt_front.x 
            && hitpt_front.x <= max_.x
            && min_.z >= hitpt_front.z
            && hitpt_front.z >= max_.z
            ) {
            // std::cout << "TRUE" << std::endl;
            return true;
        }
        // std::cout << "HOWDY" << std::endl;
        return false;
    }
    else {
        return false;
    }
}
