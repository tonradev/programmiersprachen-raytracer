#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "shape.hpp"
#include "ray.hpp"
#include "hitpoint.hpp"
#include <glm/vec3.hpp>

class Sphere : public Shape 
{
public:
    Sphere(glm::vec3 const& center, float radius, std::string name, std::string mat_name);
    ~Sphere();
    float area() const override;
    float volume() const override;
    std::ostream& print(std::ostream& os) const override;
    HitPoint intersect(Ray const& r) const;
private:
    glm::vec3 center_;
    float radius_;
};

#endif