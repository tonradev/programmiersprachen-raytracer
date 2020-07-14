#ifndef BOX_HPP
#define BOX_HPP

#include "shape.hpp"
#include <glm/vec3.hpp>

class Box : public Shape 
{
public:
    Box(glm::vec3 const& min, glm::vec3 const& max, std::string name = "NoName", Color color = {0.5f,0.5f,0.5f});
    float area() const override;
    float volume() const override;
    std::ostream& print(std::ostream& os) const override;
private:
    glm::vec3 min_;
    glm::vec3 max_;
};

#endif