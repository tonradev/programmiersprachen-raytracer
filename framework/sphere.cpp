#include <sphere.hpp>
#include <cmath>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/string_cast.hpp>

Sphere::Sphere(glm::vec3 const& center, float const radius, std::string name, std::string mat_name) :
    Shape(name, mat_name),
    center_{center},
    radius_{radius}
{
    std::cout << "I'm the Sphere class constructor." << std::endl;
}

Sphere::~Sphere() {
    std::cout << "I'm the Sphere class destructor." << std::endl;
}

float Sphere::area() const {
    return 4.0f*M_PI*std::pow(radius_,2);
}

float Sphere::volume() const {
    return (4.0f/3.0f)*M_PI*std::pow(radius_,3);
}

std::ostream& Sphere::print(std::ostream& os) const
{
    Shape::print(os);
    os << "I'm a sphere printed by the sphere print method!" << std::endl;
    return os;
}

HitPoint Sphere::intersect(Ray r)
{
    r = r.transformRay(world_transformation_inv_);
    glm::vec3::value_type dist = 0.0;
    glm::vec3 ray_direction_normalized = glm::normalize(r.direction_);

    bool intersect = glm::intersectRaySphere(r.origin_, ray_direction_normalized, center_, radius_*radius_, dist);
    /*
    if (name_ == "bsphere" && dist < 0) {
        std::cout << "DIST BSPHERE" << dist << std::endl;
    }
    */
    
    if (intersect) {
        glm::vec3 intersection_point = glm::vec3{(r.origin_+dist*r.direction_)};
        std::cout << glm::to_string(intersection_point) << std::endl;
        intersection_point = glm::vec3{world_transformation_*glm::vec4{intersection_point,1.0f}};

        std::cout << glm::to_string(intersection_point) << std::endl;
        
        HitPoint result = {intersect, dist, name_, color_, intersection_point, r.direction_};
        std::cout << glm::to_string(result.intersection_point) << std::endl;
        return result;
        
    }
    else {
        // return false;
        return HitPoint{intersect};
    }
}

glm::vec3 Sphere::getCenter() const {
    glm::vec4 res = world_transformation_inv_*glm::vec4{center_.x, center_.y, center_.z, 1.0f};
    return glm::vec3{res};
}

glm::vec3 Sphere::calcNormal(glm::vec3 const& hitpoint) const {
    glm::vec3 hitpoint_normal = hitpoint-getCenter();
    return hitpoint_normal;
}
