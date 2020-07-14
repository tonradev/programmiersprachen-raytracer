#include <box.hpp>
#include <iostream>

Box::Box(glm::vec3 const& min, glm::vec3 const& max, std::string name, Color color) :
    Shape(name, color),
    min_{min},
    max_{max}
{}

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
    os << "I'm a box printed by the box print method!";
    return os;
}
