// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"
#include "box.hpp"
#include "sphere.hpp"
#include "shape.hpp"
#include "sdfparser.hpp"
#include <vector>
#include <memory>
#include <numeric>
#include <glm/gtx/string_cast.hpp>

Renderer::Renderer(unsigned w, unsigned h, std::string const &file, SdfParser const& parser, RenderProps const& render_props, std::vector<Shape*> objects, std::map<std::string, Material> const& materials,std::vector<Light> const& lights, Camera const& cam)
    : width_(w),
    height_(h),
    color_buffer_(w * h, Color{0.0, 0.0, 0.0}),
    filename_(file),
    parser_(parser),
    render_props_(render_props),
    objects_(objects),
    materials_(materials),
    lights_(lights),
    cam_(cam),
    ppm_(width_, height_)
{
  
}



void Renderer::render()
{

  std::size_t const checker_pattern_size = 20;

  for (unsigned y = 0; y < height_; ++y)
  {
    for (unsigned x = 0; x < width_; ++x)
    {
      Pixel p(x, y);
      p.color = trace(compute_ray(cam_, p));
      /*
      if ( ((x/checker_pattern_size)%2) != ((y/checker_pattern_size)%2)) {
        p.color = Color{0.0f, 1.0f, float(x)/height_};
      } else {
        p.color = Color{1.0f, 0.0f, float(y)/width_};
      }
      */
      write(p);
    }
  }
  ppm_.save(filename_);
}

void Renderer::write(Pixel const &p)
{
  // flip pixels, because of opengl glDrawPixels
  size_t buf_pos = (width_ * p.y + p.x);
  if (buf_pos >= color_buffer_.size() || (int)buf_pos < 0)
  {
    std::cerr << "Fatal Error Renderer::write(Pixel p) : "
              << "pixel out of ppm_ : "
              << (int)p.x << "," << (int)p.y
              << std::endl;
  }
  else
  {
    color_buffer_[buf_pos] = p.color;
  }

  ppm_.write(p);
}

Ray Renderer::compute_ray(Camera const &cam, Pixel const &p)
{
  // float pix_width = 1.0f/cam.plane_width_;
  float plane_width = tan(cam.fov_x_ * M_PI / 180);
  float pix_width = plane_width / render_props_.x_res;
  float plane_height = render_props_.y_res * pix_width;

  

  glm::vec3 pixel_on_plane = {((-plane_width / 2.0f) + (pix_width * p.x)), ((-plane_height / 2.0f) + (pix_width * p.y)), -1.0f};

  
  // glm::vec3 pixel_on_plane = {(cam.plane_pos_.x-((cam.plane_width_/2.0f)*pix_width)+(p.x*pix_width)),(cam.plane_pos_.y-((cam.plane_height_/2.0f)*pix_width)+(p.y*pix_width)),-1.0f};
  Ray eye_ray = {cam.cam_pos_, {pixel_on_plane.x, pixel_on_plane.y, pixel_on_plane.z}};

  /*
  std::cout << "Eye ray before transofrmaiozn: " << std::endl;
  std::cout << "X" << eye_ray.origin_.x << "Y" << eye_ray.origin_.y << "Z" << eye_ray.origin_.z << std::endl;

  std::cout << "X" << eye_ray.direction_.x << "Y" << eye_ray.direction_.y << "Z" << eye_ray.direction_.z << std::endl;
  */
  glm::vec4 eye_ray_origin_transformed = cam.world_transformation_*glm::vec4{eye_ray.origin_,1.0f};
  glm::vec4 eye_ray_direction_transformed = cam.world_transformation_*glm::vec4{eye_ray.direction_,0};
  /*
  std::cout << glm::to_string(cam.world_transformation_) << std::endl;

std::cout << glm::to_string(cam.fov_x_) << std::endl;
  std::cout << glm::to_string(cam.cam_direction_) << std::endl;
            std::cout << glm::to_string(cam.up_) << std::endl;

  std::cout << "Eye ray after transofrmaiozn: " << std::endl;
  std::cout << "X" << eye_ray_origin_transformed.x << "Y" << eye_ray_origin_transformed.y << "Z" << eye_ray_origin_transformed.z << std::endl;

  std::cout << "X" << eye_ray_direction_transformed.x << "Y" << eye_ray_direction_transformed.y << "Z" << eye_ray_direction_transformed.z << std::endl;
  */
  Ray res = Ray{glm::vec3{eye_ray_origin_transformed},glm::vec3{eye_ray_direction_transformed}};
  // std::cout << glm::to_string(res.origin_) << std::endl;
  return res;
  // return eye_ray;
}

Color Renderer::trace(Ray r)
{
  // Tracing of a single ray

  // Check if the ray crosses any object
  for (auto const &i : objects_)
  {
    // std::cout << i->name_ << std::endl;

    std::string object_name = i->name_;

    /*
    if (.x != 0) {
    std::cout << "OUTPUT: " << render_props_.scaleprops[object_name].x << std::endl;
    }

    if(render_props_.scaleprops[object_name].x != 0) {
      std::cout << "TRANSFORMATION DETECTED" << std::endl;
      glm::mat4 scale_mat = glm::mat4{
                              glm::vec4{render_props_.scaleprops[object_name].x,0,0,0},
                              glm::vec4{0,render_props_.scaleprops[object_name].y,0,0},
                              glm::vec4{0,0,render_props_.scaleprops[object_name].z,0},
                              glm::vec4{0,0,0,0}};
      }
      */

    // std::cout << glm::to_string(r.direction_) << std::endl;
    if (i->intersect(r).intersect)
    {
      // Ray crossed this object, retrieve HitPoint
      // std::cout << "before" << std::endl;
      HitPoint hp = i->intersect(r);
      // std::cout << "r origin: " << r.origin_.x << " " << r.origin_.y << " " << r.origin_.z << std::endl;
      // std::cout << "r direction: " << r.direction_.x << " " << r.direction_.y << " " << r.direction_.z << std::endl;
      
      // std::cout << "CAMERA RAY CROSSED OBJECT AT: " << std::endl;
      // std::cout << hp.intersection_point.x << " " << hp.intersection_point.y << " " << hp.intersection_point.z << std::endl;
      

      // Ambient properties
      float l_a = 0.9f;
      glm::vec3 ka = i->mat_.ka;

      // Define RGB values and calculate ambient part
      float clr_r = l_a*ka.x;
      float clr_g = l_a*ka.y;
      float clr_b = l_a*ka.z;

      // Check the lights
      for (auto const &j : lights_)
      {
        // PRINT LIGHT POSITION
        // std::cout << "LIGHT POSITION:" << std::endl;
        // std::cout << j.pos.x << " " << j.pos.y << " " << j.pos.z << std::endl;
        //std::cout << "intersection: " << hp.intersection_point.x << " " << hp.intersection_point.y << " " << hp.intersection_point.z << std::endl;
        glm::vec3 hp_to_light_direction = j.pos - hp.intersection_point;

        // std::cout << "LIGHT TO HITPOINT DIRECTION:" << std::endl;
        // std::cout << hp_to_light_direction.x << " " << hp_to_light_direction.y << " " << hp_to_light_direction.z << std::endl;


        glm::vec3 hp_to_light_direction_normalized = glm::normalize(hp_to_light_direction);
        Ray hp_to_light = {hp.intersection_point, hp_to_light_direction_normalized};
        //Ray hp_to_light_offset = {hp.intersection_point + (hp_to_light_direction_normalized / 10.0f), hp_to_light_direction};
        // std::cout << hp.intersection_point.x << " " << hp.intersection_point.y << " " << hp.intersection_point.z << std::endl;
        //std::cout << hp.intersection_point.x << " " << hp.intersection_point.y << " " << hp.intersection_point.z << std::endl;

        // Calculate distance between hitpoint and light
        //float dist_light_hp = {sqrt(pow(j.pos.x - hp.intersection_point.x, 2) + pow(j.pos.y - hp.intersection_point.y, 2) + pow(j.pos.z - hp.intersection_point.z, 2))};

        //Ray hp_to_light_inverted = {j.pos, (hp.intersection_point - j.pos)};

        Material mat = i->mat_;

        // Diffuse part
        float l_p = j.brightness;
        
        glm::vec3 kd = mat.kd;
        glm::vec3 ks = mat.ks;
        int reflection_coeff = mat.m;
        glm::vec3 normal = i->calcNormal(hp.intersection_point);
        // std::cout << "normal: " << normal.x << " " << normal.y << " " << normal.z << std::endl;
        glm::vec3 normal_normalized = glm::normalize(i->calcNormal(hp.intersection_point));
        glm::vec3 vec_to_light = j.pos-hp.intersection_point;
        glm::vec3 vec_to_light_normalized = glm::normalize(j.pos-hp.intersection_point);
       
        float angle_diff = std::max(glm::dot(normal_normalized,vec_to_light_normalized),0.0f);

        glm::vec3 i_diff = glm::vec3{kd.x*angle_diff, kd.y*angle_diff, kd.z*angle_diff};

        Color diff_clr = Color{i_diff.x, i_diff.y, i_diff.z};


        float specular = 0.0;
        Color spec_clr = Color{0,0,0};

        // Specular part
        if (angle_diff > 0.0) {
        glm::vec3 vec_to_camera_normalized = glm::normalize(cam_.cam_pos_ - hp.intersection_point);
        glm::vec3 vec_reflection_normalized = glm::normalize(-glm::reflect(hp_to_light_direction_normalized,normal_normalized));

        glm::vec3 R_test = glm::normalize(hp_to_light_direction_normalized-2*(glm::dot(-hp_to_light_direction_normalized,normal_normalized))*normal_normalized);
      
        
        float angle_spec = std::max((glm::dot(vec_reflection_normalized,vec_to_camera_normalized)),0.0f);

        specular = std::pow(angle_spec, reflection_coeff);

        spec_clr = Color{(ks.x*specular),(ks.y*specular),(ks.z*specular)};
        
        }
        bool in_shadow = false;
        for (auto const &k : objects_)
        {
          if (k->name_ != i->name_ && k->intersect(hp_to_light).intersect)
          {
            // std::cout << k->name_ << std::endl;
            HitPoint hp2 = k->intersect(hp_to_light);
            if(i->name_ == "bsphere"){
              /*
              std::cout << "RAY ORIGIN: " << hp_to_light.origin_.x << " " << hp_to_light.origin_.y << " " << hp_to_light.origin_.z << std::endl;

              std::cout << "RAY DIRECTION: " << hp_to_light.direction_.x << " " << hp_to_light.direction_.y << " " << hp_to_light.direction_.z << std::endl;
                std::cout << "INTERSECTION POINT LIGHT -> OBJECT:" << std::endl;
            std::cout << hp2.intersection_point.x << " " << hp2.intersection_point.y << " " << hp2.intersection_point.z << std::endl;
            */
            float dist = {sqrt(pow(hp2.intersection_point.x - hp.intersection_point.x, 2) + pow(hp2.intersection_point.y - hp.intersection_point.y, 2) + pow(hp2.intersection_point.z - hp.intersection_point.z, 2))};
            /*
            std::cout << "Ditance of hitpoints: " << std::endl;
            std::cout << dist << std::endl;
            */
            }
            
            
            // Case: Shadow area
            
            /*
            if (dist > 0.01f){
              // return Color{mat.ka.x,mat.ka.y,mat.ka.z};
            }
            */
           in_shadow = true;
           break;
          }
          
        }
        if(!in_shadow){
          clr_r += l_p*(kd.x*angle_diff+ks.x*specular);
          clr_g += l_p*(kd.y*angle_diff+ks.y*specular);
          clr_b += l_p*(kd.z*angle_diff+ks.z*specular);
        }

        // return Color{(diff_clr.r+amb_clr.r+spec_clr.r),(diff_clr.g+amb_clr.g+spec_clr.g),(diff_clr.b+amb_clr.b+spec_clr.b)};

      }

      // Tone mapping
      clr_r = clr_r/(clr_r+1);
      clr_g = clr_g/(clr_g+1);
      clr_b = clr_b/(clr_b+1);
      
      return Color{clr_r,clr_g,clr_b};

    }
  }
  return Color{.1f,.1f,.1f};
}