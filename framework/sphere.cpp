#include <sphere.hpp>
#include <cmath>

Sphere::Sphere(glm::vec3 const& center, float const radius, std::string name, Color color) :
    Shape(name, color),
    center_{center},
    radius_{radius}
{}

float Sphere::area() const {
    return 4.0f*M_PI*std::pow(radius_,2);
}

float Sphere::volume() const {
    return (4.0f/3.0f)*M_PI*std::pow(radius_,3);
}

std::ostream& Sphere::print(std::ostream& os) const
{
    Shape::print(os);
    os << "I'm a sphere printed by the sphere print method!";
    return os;
}