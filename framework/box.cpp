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

HitPoint Box::intersect(Ray r)
{
    r = r.transformRay(world_transformation_inv_);
    glm::vec3::value_type dist = 0.0f;
    glm::vec3::value_type min_dist = std::numeric_limits<float>::infinity();
    glm::vec3 hitpt = glm::vec3(1.0f, 1.0f, 1.0f);

    // glm::vec3 min_transformed = glm::vec3{world_transformation_inv_*glm::vec4{min_.x,min_.y,min_.z,1.0f}};
    // glm::vec3 max_transformed = glm::vec3{world_transformation_inv_*glm::vec4{max_.x,max_.y,max_.z,0}};

    // glm::vec3 ray_direction_normalized = glm::normalize(r.direction_);

    // std::cout << "dist after intersect front: " << dist << std::endl;  
    //std::cout << "min: " << min_.x << " " << min_.y << " " << min_.z << std::endl; 
    //std::cout << "Ray origin: " << r.origin_.x << " " << r.origin_.y << " " << r.origin_.z << std::endl;                           
    //std::cout << "Ray direction: " << r.direction_.x << " " << r.direction_.y << " " << r.direction_.z << std::endl; 
    
    glm::vec3 leftborder_bottom = glm::vec3{min_.x,min_.y,max_.z}-min_;
    glm::vec3 frontborder_bottom = glm::vec3{max_.x,min_.y,min_.z}-min_;
    glm::vec3 leftborder_top = glm::vec3{min_.x,max_.y,max_.z}-glm::vec3{min_.x,max_.y,min_.z};
    glm::vec3 frontborder_top = glm::vec3{max_.x,max_.y,min_.z}-glm::vec3{min_.x,max_.y,min_.z};
    // glm::vec3 normal_bottom = glm::cross(leftborder_bottom,frontborder_bottom);
    // glm::vec3 normal_top = glm::cross(leftborder_top,frontborder_top);

    // std::cout << "BOTTOM NORMAL CALCULATED WITH CROSS PRODUCT: " << std::endl;
    // std::cout << normal_bottom.x << " " << normal_bottom.y << " " << normal_bottom.z << std::endl;

    // std::cout << "TOP NORMAL CALCULATED WITH CROSS PRODUCT: " << std::endl;
    // std::cout << normal_top.x << " " << normal_top.y << " " << normal_top.z << std::endl;

    // glm::vec3 normal_front = glm::vec3{world_transformation_inv_*glm::vec4{0,0,1.0f,0}};
    
    bool intersect_front = glm::intersectRayPlane(r.origin_,r.direction_,
                                                  glm::vec3{min_.x,min_.y,min_.z},glm::vec3{0,0,1.0f},dist);

    if (intersect_front && dist < min_dist)
    {
        glm::vec3 tmp_hitpt = {(r.origin_+dist*r.direction_)};
        tmp_hitpt = glm::vec3{world_transformation_inv_*glm::vec4{tmp_hitpt.x,tmp_hitpt.y,tmp_hitpt.z,1.0f}};
        
        if( min_.x <= tmp_hitpt.x 
            && tmp_hitpt.x <= max_.x
            && min_.y <= tmp_hitpt.y
            && tmp_hitpt.y <= max_.y)
        {
            // std::cout << "Front dist smaller: " << dist << std::endl;
            min_dist = dist;
            hitpt = tmp_hitpt;
            hitpoint_normal = glm::vec3{0,0,1.0f};
        }
    }

    glm::vec3 normal_back = glm::vec3{world_transformation_inv_*glm::vec4{0,0,-1.0f,0}};

    bool intersect_back = glm::intersectRayPlane(r.origin_,r.direction_,
                                                glm::vec3{min_.x,min_.y,max_.z},normal_back,dist);
    if (intersect_back && dist < min_dist)
    {
        glm::vec3 tmp_hitpt = {(r.origin_+dist*r.direction_)};
        tmp_hitpt = glm::vec3{world_transformation_inv_*glm::vec4{tmp_hitpt.x,tmp_hitpt.y,tmp_hitpt.z,1.0f}};

        if( min_.x <= tmp_hitpt.x 
            && tmp_hitpt.x <= max_.x
            && min_.y <= tmp_hitpt.y
            && tmp_hitpt.y <= max_.y)
        {
            // std::cout << "Back dist smaller: " << dist << std::endl;
            min_dist = dist;
            hitpt = tmp_hitpt;
            hitpoint_normal = glm::vec3{0,0,-1.0f};
        }
    }

    glm::vec3 normal_top = glm::vec3{world_transformation_inv_*glm::vec4{0,1.0f,0,0}};

    bool intersect_top = glm::intersectRayPlane(r.origin_,r.direction_,
                                                glm::vec3{min_.x,max_.y,min_.z},normal_top,dist);
    if (intersect_top && dist < min_dist)
    {
        glm::vec3 tmp_hitpt = {(r.origin_+dist*r.direction_)};
        tmp_hitpt = glm::vec3{world_transformation_inv_*glm::vec4{tmp_hitpt.x,tmp_hitpt.y,tmp_hitpt.z,1.0f}};

        if( min_.z >= tmp_hitpt.z 
            && tmp_hitpt.z >= max_.z
            && min_.x <= tmp_hitpt.x
            && tmp_hitpt.x <= max_.x)
        {
            // std::cout << "INTERSECT TOP" << std::endl;
            // std::cout << "Top dist smaller: " << dist << std::endl;
            min_dist = dist;
            hitpt = tmp_hitpt;
            hitpoint_normal = glm::vec3{0,1.0f,0};
        }
    }
    glm::vec3 normal_bottom = glm::vec3{world_transformation_inv_*glm::vec4{0,-1.0f,0,1.0f}};

    bool intersect_bottom = glm::intersectRayPlane(r.origin_,r.direction_,
                                                  min_,normal_bottom,dist);
    if (intersect_bottom && dist < min_dist)
    {
        glm::vec3 tmp_hitpt = {(r.origin_+dist*r.direction_)};
        tmp_hitpt = glm::vec3{world_transformation_inv_*glm::vec4{tmp_hitpt.x,tmp_hitpt.y,tmp_hitpt.z,1.0f}};

        if( min_.z >= tmp_hitpt.z 
            && tmp_hitpt.z >= max_.z
            && min_.x <= tmp_hitpt.x
            && tmp_hitpt.x <= max_.x)
        {
            // std::cout << "Bottom dist smaller: " << dist << std::endl;
            // std::cout << "INTERSECT BOTTOM" << std::endl;
            min_dist = dist;
            hitpt = tmp_hitpt;
            hitpoint_normal = glm::vec3{0,-1.0f,0};
        }
    }

    glm::vec3 normal_left = glm::vec3{world_transformation_inv_*glm::vec4{-1.0f,0,0,0}};

    bool intersect_left = glm::intersectRayPlane(r.origin_,r.direction_,
                                                  min_,normal_left,dist);
    if (intersect_left && dist < min_dist)
    {
        glm::vec3 tmp_hitpt = {(r.origin_+dist*r.direction_)};
        tmp_hitpt = glm::vec3{world_transformation_inv_*glm::vec4{tmp_hitpt.x,tmp_hitpt.y,tmp_hitpt.z,1.0f}};

        if( min_.z >= tmp_hitpt.z 
            && tmp_hitpt.z >= max_.z
            && min_.y <= tmp_hitpt.y
            && tmp_hitpt.y <= max_.y)
        {
            // std::cout << "Left dist smaller: " << dist << std::endl;
            min_dist = dist;
            hitpt = tmp_hitpt;
            hitpoint_normal = glm::vec3{-1.0f,0,0};
        }
    }

    glm::vec3 normal_right = glm::vec3{world_transformation_inv_*glm::vec4{1.0f,0,0,0}};

    bool intersect_right = glm::intersectRayPlane(r.origin_,r.direction_,
                                                  glm::vec3{max_.x,min_.y,min_.z},glm::vec3{1.0f,0.0f,0.0f},dist);
    if (intersect_right && dist < min_dist)
    {
        glm::vec3 tmp_hitpt = {(r.origin_+dist*r.direction_)};
        tmp_hitpt = glm::vec3{world_transformation_inv_*glm::vec4{tmp_hitpt.x,tmp_hitpt.y,tmp_hitpt.z,1.0f}};

        if( min_.z >= tmp_hitpt.z
            && tmp_hitpt.z >= max_.z
            && min_.y <= tmp_hitpt.y
            && tmp_hitpt.y <= max_.y)
        {
            // std::cout << "Right dist smaller: " << dist << std::endl;
            min_dist = dist;
            hitpt = tmp_hitpt;
            hitpoint_normal = glm::vec3{1.0f,0,0};
        }
    }

    if(min_dist < std::numeric_limits<float>::infinity()) {
        hitpt = glm::vec3{world_transformation_inv_*glm::vec4{hitpt.x,hitpt.y,hitpt.z,1.0f}};
        return HitPoint{true,min_dist,name_,color_,hitpt,r.direction_};
    }
    
    return HitPoint{false};
}

glm::vec3 Box::calcNormal(glm::vec3 const& hitpoint) const {
    // glm::vec3 center = {min_.x+((max_.x-min_.x)/2.0f),min_.y+((max_.y-min_.y)/2.0f),min_.z+((min_.z-max_.z)/2.0f)};
    // glm::vec3 hitpoint_normal = hitpoint-center;
    glm::vec4 hp_normal_transformed = world_transformation_inv_*glm::vec4{hitpoint_normal.x, hitpoint_normal.y, hitpoint_normal.z, 0};
    return glm::vec3{hp_normal_transformed};
}
