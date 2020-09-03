#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <color.hpp>
#include "ray.hpp"
#include "material.hpp"
#include "hitpoint.hpp"

class Shape
{
public:
    Shape(std::string const& name, std::string const& mat_name);
    virtual ~Shape();
    virtual float area() const = 0;
    virtual float volume() const = 0;
    virtual std::ostream& print(std::ostream& os) const;
    virtual HitPoint intersect(Ray const& r) const = 0;
    void setMaterial(Material mat);
    std::string mat_name_;
    Material mat_;
    std::string name_ = "NoName";
protected:
    
    Color color_ = {0.0f,0.0f,0.0f};

};

std::ostream& operator<<(std::ostream& os, Shape const& s);

#endif