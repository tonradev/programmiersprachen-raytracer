#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <iostream> // debug prints
#include <fstream>  // open / read / write /close files
#include <sstream>  // stringstream -> easy parsing mechanics
#include <string>   // buffer lines read from file
#include <map>
#include <glm/vec3.hpp> // for shape attributes
#include <camera.hpp>
#include <shape.hpp>
#include <box.hpp>
#include <sphere.hpp>
#include <material.hpp>
#include <renderprops.hpp>
#include <light.hpp>


class SdfParser{
    public:
        SdfParser(std::string sdf_file);
        std::vector<Shape*> getShapes() const;
        Camera getCamera() const;
        std::map<std::string,Material> getMaterials() const;
        RenderProps getRenderProperties() const;
        std::vector<Light> getLights() const;
    private:
        std::string const in_file_path_;
        std::vector<Shape*> shapes_;
        Camera camera_;
        std::map<std::string,Material> materials_;
        RenderProps render_properties_;
        std::vector<Light> lights_;
};

#endif // #ifndef PARSER_HPP