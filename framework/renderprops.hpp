#ifndef RENDERPROPS_HPP
#define RENDERPROPS_HPP

#include <string>

struct RenderProps {
    std::string cam_name;
    std::string filename;
    int x_res;
    int y_res;
    /*
    std::map<std::string,glm::vec3> scaleprops;
    std::map<std::string,glm::vec3> translateprops;
    std::map<std::string,glm::vec4> rotateprops;
    */
};

#endif // RENDERPROPS_HPP