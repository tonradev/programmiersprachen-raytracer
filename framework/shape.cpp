#include <shape.hpp>
#include <iostream>
#include "material.hpp"


Shape::Shape(std::string const& name, std::string const& mat_name) :
    name_ {name},
    mat_name_{mat_name}
{
    std::cout << "I'm the Shape class constructor." << std::endl;
};

Shape::~Shape() {
    std::cout << "I'm the Shape class destructor." << std::endl;
}

void Shape::setMaterial(Material mat) {
    mat_ = mat;
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

