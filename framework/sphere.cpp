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

bool Sphere::intersect(Ray const& r) const
{
    glm::vec3::value_type dist = 0.0;
    glm::vec3 ray_direction_normalized = glm::normalize(r.direction);
    bool intersect = glm::intersectRaySphere(r.origin, ray_direction_normalized, center_, radius_*radius_, dist);
    if (intersect) {
        return true;
        /*
        HitPoint result = {intersect, dist, name_, color_, (r.origin+dist*r.direction), r.direction};
        return result;
        */
    }
    else {
        return false;
        // return HitPoint{intersect};
    }
}