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

void Shape::applyScaling(float x, float y, float z){
    world_transformation_ = {{x,0,0,0},{0,y,0,0},{0,0,z,0},{0,0,0,1.0f}};
    updateInverse();
}

void Shape::applyTranslation(float x, float y, float z){
    world_transformation_ = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{x,y,z,1.0f}};
    updateInverse();
}

void Shape::updateInverse(){
    world_transformation_inv_ = glm::inverse(world_transformation_);
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

