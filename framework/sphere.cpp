#include <sphere.hpp>
#include <cmath>
#include <glm/gtx/intersect.hpp>

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

HitPoint Sphere::intersect(Ray const& r)
{
    glm::vec3::value_type dist = 0.0;
    glm::vec3 ray_direction_normalized = glm::normalize(r.direction_);

    bool intersect = glm::intersectRaySphere(r.origin_, ray_direction_normalized, center_, radius_*radius_, dist);
    if (intersect) {
        // return true;
        glm::vec3 intersection_point = glm::vec3{(r.origin_+dist*r.direction_)};

        /*

        std::cout << "===========================================" << std::endl;

        std::cout << "RAY DIRECTION" << std::endl;
        std::cout << r.direction_.x << " " << r.direction_.y << " " << r.direction_.z << std::endl;

        std::cout << "CENTER" << std::endl;
        std::cout << center_.x << " " << center_.y << " " << center_.z << std::endl;
            
        std::cout << "INTERSECTION POINT" << std::endl;
        std::cout << intersection_point.x << " " << intersection_point.y << " " << intersection_point.z << std::endl;

        std::cout << "NORMAL: " << std::endl;
        std::cout << hitpoint_normal_.x << " " << hitpoint_normal_.y << " " << hitpoint_normal_.z << std::endl;
        
        std::cout << "===========================================" << std::endl;

        */
        
        
        HitPoint result = {intersect, dist, name_, color_, (r.origin_+dist*r.direction_), r.direction_};
        return result;
        
    }
    else {
        // return false;
        return HitPoint{intersect};
    }
}

glm::vec3 Sphere::getCenter() const {
    return center_;
}

glm::vec3 Sphere::calcNormal(glm::vec3 const& hitpoint) const {
    glm::vec3 hitpoint_normal = hitpoint-center_;
    return hitpoint_normal;
}
