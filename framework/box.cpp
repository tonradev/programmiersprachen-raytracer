#include <box.hpp>
#include <glm/gtx/intersect.hpp>
#include <iostream>
#include <cmath>

Box::Box(glm::vec3 const& min, glm::vec3 const& max, std::string name, std::string mat_name) :
    Shape(name, mat_name),
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

HitPoint Box::intersect(Ray const& r) const
{
    glm::vec3::value_type dist = 0.0f;
    glm::vec3::value_type min_dist = std::numeric_limits<float>::infinity();
    glm::vec3 hitpt = glm::vec3(1.0f, 1.0f, 1.0f);
    // glm::vec3 ray_direction_normalized = glm::normalize(r.direction_);
    bool intersect_front = glm::intersectRayPlane(r.origin_,r.direction_,
                                                  glm::vec3{min_.x,min_.y,min_.z},glm::normalize(glm::vec3{0.0f,0.0f,1.0f}),dist);
    // std::cout << "dist after intersect front: " << dist << std::endl;  
    //std::cout << "min: " << min_.x << " " << min_.y << " " << min_.z << std::endl; 
    //std::cout << "Ray origin: " << r.origin_.x << " " << r.origin_.y << " " << r.origin_.z << std::endl;                           
    //std::cout << "Ray direction: " << r.direction_.x << " " << r.direction_.y << " " << r.direction_.z << std::endl; 
    if (intersect_front && abs(dist) < min_dist)
    {
        glm::vec3 tmp_hitpt = {(r.origin_+dist*r.direction_)};
        if( min_.x <= tmp_hitpt.x 
            && tmp_hitpt.x <= max_.x
            && min_.y <= tmp_hitpt.y
            && tmp_hitpt.y <= max_.y)
        {
            // std::cout << "Front dist smaller: " << dist << std::endl;
            min_dist = dist;
            hitpt = tmp_hitpt;
        }
    }
    bool intersect_back = glm::intersectRayPlane(r.origin_,r.direction_,
                                                glm::vec3{min_.x,min_.y,max_.z},glm::normalize(glm::vec3{0.0f,0.0f,-1.0f}),dist);
    if (intersect_back && abs(dist) < min_dist)
    {
        glm::vec3 tmp_hitpt = {(r.origin_+dist*r.direction_)};
        if( min_.x <= tmp_hitpt.x 
            && tmp_hitpt.x <= max_.x
            && min_.y <= tmp_hitpt.y
            && tmp_hitpt.y <= max_.y)
        {
            // std::cout << "Back dist smaller: " << dist << std::endl;
            min_dist = dist;
            hitpt = tmp_hitpt;
        }
    }
    bool intersect_top = glm::intersectRayPlane(r.origin_,r.direction_,
                                                glm::vec3{min_.x,max_.y,min_.z},glm::normalize(glm::vec3{0.0f,1.0f,0.0f}),dist);
    if (intersect_top && abs(dist) < min_dist)
    {
        glm::vec3 tmp_hitpt = {(r.origin_+dist*r.direction_)};
        if( min_.z >= tmp_hitpt.z 
            && tmp_hitpt.z >= max_.z
            && min_.x <= tmp_hitpt.x
            && tmp_hitpt.x <= max_.x)
        {
            // std::cout << "Top dist smaller: " << dist << std::endl;
            min_dist = dist;
            hitpt = tmp_hitpt;
        }
    }
    bool intersect_bottom = glm::intersectRayPlane(r.origin_,r.direction_,
                                                  min_,glm::normalize(glm::vec3{0.0f,-1.0f,0.0f}),dist);
    if (intersect_bottom && abs(dist) < min_dist)
    {
        glm::vec3 tmp_hitpt = {(r.origin_+dist*r.direction_)};
        if( min_.z >= tmp_hitpt.z 
            && tmp_hitpt.z >= max_.z
            && min_.x <= tmp_hitpt.x
            && tmp_hitpt.x <= max_.x)
        {
            // std::cout << "Bottom dist smaller: " << dist << std::endl;
            min_dist = dist;
            hitpt = tmp_hitpt;
        }
    }
    bool intersect_left = glm::intersectRayPlane(r.origin_,r.direction_,
                                                  min_,glm::normalize(glm::vec3{-1.0f,0.0f,0.0f}),dist);
    if (intersect_left && abs(dist) < min_dist)
    {
        glm::vec3 tmp_hitpt = {(r.origin_+dist*r.direction_)};
        if( min_.z >= tmp_hitpt.z 
            && tmp_hitpt.z >= max_.z
            && min_.y <= tmp_hitpt.y
            && tmp_hitpt.y <= max_.y)
        {
            // std::cout << "Left dist smaller: " << dist << std::endl;
            min_dist = dist;
            hitpt = tmp_hitpt;
        }
    }
    bool intersect_right = glm::intersectRayPlane(r.origin_,r.direction_,
                                                  glm::vec3{max_.x,min_.y,min_.z},glm::normalize(glm::vec3{1.0f,0.0f,0.0f}),dist);
    if (intersect_right && abs(dist) < min_dist)
    {
        glm::vec3 tmp_hitpt = {(r.origin_+dist*r.direction_)};
        if( min_.z >= tmp_hitpt.z
            && tmp_hitpt.z >= max_.z
            && min_.y <= tmp_hitpt.y
            && tmp_hitpt.y <= max_.y)
        {
            // std::cout << "Right dist smaller: " << dist << std::endl;
            min_dist = dist;
            hitpt = tmp_hitpt;
        }
    }

    if(min_dist < std::numeric_limits<float>::infinity())
        return HitPoint{true,min_dist,name_,color_,hitpt,r.direction_};
    
    return HitPoint{false};
}
