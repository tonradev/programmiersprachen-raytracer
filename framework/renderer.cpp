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

Renderer::Renderer(unsigned w, unsigned h, std::string const &file)
    : width_(w), height_(h), color_buffer_(w * h, Color{0.0, 0.0, 0.0}), filename_(file), ppm_(width_, height_)
{
}

/*
Material m1 = {"red", {1.0,0,0},{1.0,0,0},{1.0,0,0},1};
Camera c1 = Camera{{0.0f,0.0f,0.0f},45.0};
Sphere sp2 = {{0.0f,0.0f,-40.0f},5.5f,"sp2","red"};
// Box b9 = Box{{0.0f,0.0f,0.0f},{3.0f,4.2f,-4.0f},"Box9"};
Box b9 = Box{{0.0f,0.0f,-40.0f},{50.0f,10.0f,-50.0f},"Box9","red"};
*/

SdfParser parser = {"scene1.sdf"};

RenderProps render_props = parser.getRenderProperties();

std::vector<Shape *> objects = parser.getShapes();

std::map<std::string, Material> materials = parser.getMaterials();

std::vector<Light> lights = parser.getLights();

Camera cam = parser.getCamera();

void Renderer::render()
{

  std::size_t const checker_pattern_size = 20;

  for (unsigned y = 0; y < height_; ++y)
  {
    for (unsigned x = 0; x < width_; ++x)
    {
      Pixel p(x, y);
      p.color = trace(compute_ray(cam, p));
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
  float pix_width = plane_width / render_props.x_res;
  float plane_height = render_props.y_res * pix_width;

  glm::vec3 pixel_on_plane = {((-plane_width / 2.0f) + (pix_width * p.x)), ((-plane_height / 2.0f) + (pix_width * p.y)), -1.0f};

  // glm::vec3 pixel_on_plane = {(cam.plane_pos_.x-((cam.plane_width_/2.0f)*pix_width)+(p.x*pix_width)),(cam.plane_pos_.y-((cam.plane_height_/2.0f)*pix_width)+(p.y*pix_width)),-1.0f};
  Ray eye_ray = {cam.cam_pos_, {pixel_on_plane.x, pixel_on_plane.y, pixel_on_plane.z}};
  if (p.x == 0 && p.y == 0)
  {
    std::cout << "X" << eye_ray.direction_.x << "Y" << eye_ray.direction_.y << "Z" << eye_ray.direction_.z << std::endl;
  }
  return eye_ray;
}

Color Renderer::trace(Ray r)
{
  // Tracing of a single ray

  // Check if the ray crosses any object
  for (auto const &i : objects)
  {
    
    if (i->intersect(r).intersect)
    {
      // Ray crossed this object, retrieve HitPoint
      HitPoint hp = i->intersect(r);

      /*
      std::cout << "CAMERA RAY CROSSED OBJECT AT: " << std::endl;
      std::cout << hp.intersection_point.x << " " << hp.intersection_point.y << " " << hp.intersection_point.z << std::endl;
      */

      // Check the lights
      for (auto const &j : lights)
      {
        // PRINT LIGHT POSITION
        // std::cout << "LIGHT POSITION:" << std::endl;
        // std::cout << j.pos.x << " " << j.pos.y << " " << j.pos.z << std::endl;

        glm::vec3 hp_to_light_direction = j.pos - hp.intersection_point;

        // std::cout << "LIGHT TO HITPOINT DIRECTION:" << std::endl;
        // std::cout << hp_to_light_direction.x << " " << hp_to_light_direction.y << " " << hp_to_light_direction.z << std::endl;


        glm::vec3 hp_to_light_direction_normalized = glm::normalize(hp_to_light_direction);
        Ray hp_to_light = {hp.intersection_point, hp_to_light_direction};
        Ray hp_to_light_offset = {hp.intersection_point + (hp_to_light_direction_normalized / 10.0f), hp_to_light_direction};
        // std::cout << hp.intersection_point.x << " " << hp.intersection_point.y << " " << hp.intersection_point.z << std::endl;
        //std::cout << hp.intersection_point.x << " " << hp.intersection_point.y << " " << hp.intersection_point.z << std::endl;

        // Calculate distance between hitpoint and light
        float dist_light_hp = {sqrt(pow(j.pos.x - hp.intersection_point.x, 2) + pow(j.pos.y - hp.intersection_point.y, 2) + pow(j.pos.z - hp.intersection_point.z, 2))};

        Ray hp_to_light_inverted = {j.pos, (hp.intersection_point - j.pos)};
        for (auto const &k : objects)
        {
          if (k->intersect(hp_to_light_inverted).intersect)
          {
            // std::cout << k->name_ << std::endl;
            HitPoint hp2 = k->intersect(hp_to_light_inverted);
            // std::cout << "INTERSECTION POINT LIGHT -> OBJECT:" << std::endl;
            // std::cout << hp2.intersection_point.x << " " << hp2.intersection_point.y << " " << hp2.intersection_point.z << std::endl;
            float dist = {sqrt(pow(hp2.intersection_point.x - hp.intersection_point.x, 2) + pow(hp2.intersection_point.y - hp.intersection_point.y, 2) + pow(hp2.intersection_point.z - hp.intersection_point.z, 2))};
            // std::cout << "Ditance of hitpoints: " << std::endl;
            // std::cout << dist << std::endl;
            // Case: Shadow area
            /*
            if (dist > 0.001f){
              return Color{0, 0, 0.0f};
            }
            */
            
            
            
              
          }
          
        }
        // Diffuse part
        float l_p = j.brightness;
        Material mat = i->mat_;
        glm::vec3 kd = mat.kd;
        glm::vec3 ka = mat.ka;
        glm::vec3 ks = mat.ks;
        int reflection_coeff = mat.m;
        glm::vec3 normal = i->calcNormal(hp.intersection_point);
        glm::vec3 normal_normalized = glm::normalize(i->calcNormal(hp.intersection_point));
        glm::vec3 vec_to_light = j.pos-hp.intersection_point;
        glm::vec3 vec_to_light_normalized = glm::normalize(j.pos-hp.intersection_point);
       
        float angle_diff = std::max(glm::dot(normal_normalized,vec_to_light_normalized),0.0f);

        glm::vec3 i_diff = glm::vec3{kd.x*angle_diff, kd.y*angle_diff, kd.z*angle_diff};

        Color diff_clr = Color{i_diff.x, i_diff.y, i_diff.z};

        // Ambient part
        float l_a = 0.9f;
        Color amb_clr = Color{l_a*ka.x, l_a*ka.y, l_a*ka.z};


        float specular = 0.0;
        Color spec_clr = Color{0,0,0};

        // Specular part
        if (angle_diff > 0.0) {
        glm::vec3 vec_to_camera_normalized = glm::normalize(cam.cam_pos_ - hp.intersection_point);
        glm::vec3 vec_reflection_normalized = glm::normalize(-glm::reflect(hp_to_light_direction_normalized,normal_normalized));

        glm::vec3 R_test = glm::normalize(hp_to_light_direction_normalized-2*(glm::dot(-hp_to_light_direction_normalized,normal_normalized))*normal_normalized);
      
        
        float angle_spec = std::max((glm::dot(vec_reflection_normalized,vec_to_camera_normalized)),0.0f);

        specular = std::pow(angle_spec, reflection_coeff);

        spec_clr = Color{(ks.x*specular),(ks.y*specular),(ks.z*specular)};
        
        }
        float clr_r = {l_a*ka.x+l_p*(kd.x*angle_diff+ks.x*specular)};
        float clr_g = {l_a*ka.y+l_p*(kd.y*angle_diff+ks.y*specular)};
        float clr_b = {l_a*ka.z+l_p*(kd.z*angle_diff+ks.z*specular)};
        return Color{clr_r,clr_g,clr_b};

        // return Color{(diff_clr.r+amb_clr.r+spec_clr.r),(diff_clr.g+amb_clr.g+spec_clr.g),(diff_clr.b+amb_clr.b+spec_clr.b)};

      }
      return Color{0.0f, 0.0f, 1.0f};
    }
  }
  return Color{.1f,.1f,.1f};
}