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
    virtual HitPoint intersect(Ray r) = 0;
    virtual glm::vec3 calcNormal(glm::vec3 const& hitpoint) const = 0;
    void setMaterial(Material mat);
    void applyScaling(float x, float y, float z);
    void applyTranslation(float x, float y, float z);
    void updateInverse();
    std::string mat_name_;
    Material mat_;
    std::string name_ = "NoName";
    glm::mat4 world_transformation_ = glm::mat4{glm::vec4{1,0,0,0},glm::vec4{0,1,0,0},glm::vec4{0,0,1,0},glm::vec4{0,0,0,1}};
    glm::mat4 world_transformation_inv_ = glm::inverse(world_transformation_);
protected:
    Color color_ = {0,0,0};
};

std::ostream& operator<<(std::ostream& os, Shape const& s);

#endif