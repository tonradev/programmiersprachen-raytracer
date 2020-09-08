#ifndef BOX_HPP
#define BOX_HPP

#include "shape.hpp"
#include "hitpoint.hpp"
#include <glm/vec3.hpp>
#include "material.hpp"

class Box : public Shape 
{
public:
    Box(glm::vec3 const& min, glm::vec3 const& max, std::string name, std::string mat_name);
    ~Box();
    float area() const override;
    float volume() const override;
    std::ostream& print(std::ostream& os) const override;
    HitPoint intersect(Ray const& r);
    glm::vec3 calcNormal(glm::vec3 const& hitpoint) const override;
private:
    glm::vec3 hitpoint_normal = {0,0,0};
    glm::vec3 min_;
    glm::vec3 max_;
};

#endif