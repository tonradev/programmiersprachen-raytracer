#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <color.hpp>

class Shape
{
public:
    Shape(std::string const& name, Color const& color) :
        name_ {name},
        color_ {color}
        {};
    virtual float area() const = 0;
    virtual float volume() const = 0;
private:
    std::string name_ = "NoName";
    Color color_ = {0.0f,0.0f,0.0f};

};

#endif