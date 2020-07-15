#include <shape.hpp>
#include <iostream>


Shape::Shape(std::string const& name, Color const& color) :
    name_ {name},
    color_ {color}
{
    std::cout << "I'm the Shape class constructor." << std::endl;
};

Shape::~Shape() {
    std::cout << "I'm the Shape class destructor." << std::endl;
}

std::ostream& Shape::print(std::ostream& os) const
{
    os << "Name: " << name_ << "\n" << "Color (R, G, B): " << color_.r << ", " << color_.g << ", " << color_.b << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, Shape const& s)
{
    return s.print(os);
}