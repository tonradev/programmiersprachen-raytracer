#include "sdfparser.hpp"


SdfParser::SdfParser(std::string sdf_file)
  : in_file_path_{sdf_file}
  {

  // open file in read-only && ASCII mode 
  std::ifstream in_file(in_file_path_, std::ios::in);
  
  std::string line_buffer;

  int32_t line_count = 0;


  std::string identifier;    
  std::string class_name;

  std::map<std::string,glm::vec3> scaleprops;
  std::map<std::string,glm::vec3> translateprops;
  std::map<std::string,glm::vec4> rotateprops;

  while( std::getline(in_file, line_buffer)  ) {
    //std::cout << ++line_count << ": " << line_buffer << std::endl; 
    
    //construct stringstream using line_buffer string
    std::istringstream in_sstream(line_buffer);
    
    in_sstream >> identifier;

    std::cout << "Identifier content: " << identifier << std::endl;

    // Ignore comments
    if("#" == identifier) {
      std::cout << "Kommentarzeile" << std::endl;
      continue;
    }
    else if ("" == identifier){
      continue;
    }
    
    // check for shapes / materials / lights
    if("define" == identifier) {
      in_sstream >> class_name; 
  
      // check for specific shape
      if("shape" == class_name) {
         //check for shape type, then: parse attributes (including material lookup)
         std::string shape_type;
         std::string name;
         
         in_sstream >> shape_type;
         in_sstream >> name;

         std::cout << "Shape type: " << shape_type << " Name of shape: " << name << std::endl;
         
         if("sphere" == shape_type) {
           float center_x, center_y, center_z;
           glm::vec3 center;
           float radius;
           std::string mat_name;

           in_sstream >> center_x >> center_y >> center_z;
           center = {center_x, center_y, center_z};
           in_sstream >> radius;
           in_sstream >> mat_name;

           shapes_.push_back(new Sphere{center,radius,name,mat_name});
         }
         else if("box" == shape_type) {
           float min_point_x, min_point_y, min_point_z;
           float max_point_x, max_point_y, max_point_z;
           glm::vec3 min_point;
           glm::vec3 max_point;
           std::string mat_name;

           in_sstream >> min_point_x;
           in_sstream >> min_point_y;
           in_sstream >> min_point_z;
           in_sstream >> max_point_x >> max_point_y >> max_point_z;
           in_sstream >> mat_name;
           /*std::cout << "name: " << name << std::endl;
           std::cout << "mat name: " << mat_name << std::endl;
           std::cout << "min point: " << min_point_x << " " << min_point_y << " " << min_point_z << std::endl;
           std::cout << "max point: " << max_point_x << " " << max_point_y << " " << max_point_z << std::endl;
           throw std::exception();*/
           

           min_point = {min_point_x,min_point_y,min_point_z};
           max_point = {max_point_x,max_point_y,max_point_z};

           

           shapes_.push_back(new Box{min_point,max_point,name,mat_name});

         }
      }
      else if ("material" == class_name) {
        //parse material attributes
        std::string material_name;
        float ka_red, ka_green, ka_blue;
        float kd_red, kd_green, kd_blue;
        float ks_red, ks_green, ks_blue;
        int m;
 
        in_sstream >> material_name;
        in_sstream >> ka_red >> ka_green >> ka_blue; // 
        in_sstream >> kd_red >> kd_green >> kd_blue; // 
        in_sstream >> ks_red >> ks_green >> ks_blue; // 

        in_sstream >> m;
 
        std::cout << ka_red  << " " << ka_green << " " << ka_blue << std::endl;

        std::cout << "Material" << std::endl;

        materials_.emplace(material_name, Material{material_name,{ka_red,ka_green,ka_blue},{kd_red,kd_green,kd_blue},{ks_red,ks_green,ks_blue},m});

      }
      else if ("light" == class_name) {
        std::string name;
        float pos_x, pos_y, pos_z;
        float color_r, color_g, color_b;
        float brightness;

        in_sstream >> name >> pos_x >> pos_y >> pos_z >> color_r >> color_g >> color_b >> brightness;

        std::cout << "Light" << std::endl;

        lights_.push_back({name,{pos_x,pos_y,pos_z},{color_r,color_g,color_b},brightness});

      }
      else if ("camera" == class_name) {
        std::string name;
        float fov_x;
        float eye_pos_x;
        float eye_pos_y;
        float eye_pos_z;
        float dir_x;
        float dir_y;
        float dir_z;
        float up_x;
        float up_y;
        float up_z;

        in_sstream >> name >> fov_x >> eye_pos_x >> eye_pos_y >> eye_pos_z >> dir_x >> dir_y >> dir_z >> up_x >> up_y >> up_z;

        glm::vec3 eye_pos{eye_pos_x,eye_pos_y,eye_pos_z};
        glm::vec3 dir{dir_x,dir_y,dir_z};
        glm::vec3 up{up_x,up_y,up_z};


        camera_ = Camera{name, fov_x, eye_pos, dir, up};
      }
      else {
        std::cout << "Invalid SDF syntax in line, cannot parse!" << std::endl;
      }
    }
    else if("ambient" == identifier) {
      float ambient_1, ambient_2, ambient_3;

      in_sstream >> ambient_1 >> ambient_2 >> ambient_3;
    }
    else if("transform" == identifier) {
      std::string transformation_type;
      in_sstream >> transformation_type;
      std::string object_name;
      in_sstream >> object_name;

      if("scale" == transformation_type){

        float s1, s2, s3;

        in_sstream >> s1 >> s2 >> s3;

        // in_sstream >> scaleprops[object_name].x >> scaleprops[object_name].y >> scaleprops[object_name].z;

        // Shape* result = [object_name](Shape* s) { return s->name_ == object_name; }, shapes_;

        for (auto & i : shapes_) {
          if (i->name_ == object_name) {
            i->applyScaling(s1, s2, s3);
            continue;
          }
        }

        std::cout << "Object not found, scaling failed." << std::endl;

      }
      
      else if("translate" == transformation_type){

        float t1, t2, t3;

        in_sstream >> t1 >> t2 >> t3;

        for (auto & i : shapes_) {
          if (i->name_ == object_name) {
            i->applyTranslation(t1, t2, t3);
            continue;
          }
        }

        std::cout << "Object not found, scaling failed." << std::endl;

      }
      /*
      else if("rotate" == transformation_type){
        std::string object_name;
        in_sstream >> object_name;

        in_sstream >> rotateprops[object_name].p >> rotateprops[object_name].q >> rotateprops[object_name].r >> render_properties_.rotateprops[object_name].s;

      }
      */
    }
    else if("render" == identifier) {
      std::string cam_name;
      std::string filename;
      int x_res;
      int y_res;

      in_sstream >> cam_name >> filename.append(".ppm") >> x_res >> y_res;

      render_properties_ = RenderProps{cam_name,filename,x_res,y_res};
    }
    else {
      std::cout << "Invalid SDF syntax in line, cannot parse!" << std::endl;
    }

  }
  // Set material for each Shape
  for(auto& x: shapes_) {
    x->setMaterial(materials_[x->mat_name_]);
  }
  // close file
  in_file.close();
};

Camera SdfParser::getCamera() const{
  return camera_;
}

RenderProps SdfParser::getRenderProperties() const{
  return render_properties_;
}

std::vector<Shape*> SdfParser::getShapes() const{
  return shapes_;
}

std::map<std::string,Material> SdfParser::getMaterials() const{
  return materials_;
}

std::vector<Light> SdfParser::getLights() const{
  return lights_;
}