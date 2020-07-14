#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "shape.hpp"
#include <glm/vec3.hpp>

class Sphere : public Shape 
{
public:
    Sphere(glm::vec3 const& center, float radius, std::string name = "NoName", Color color = {0.5f,0.5f,0.5f});
    float area() const override;
    float volume() const override;
    std::ostream& print(std::ostream& os) const override;
private:
    glm::vec3 center_;
    float radius_;
};

#endif