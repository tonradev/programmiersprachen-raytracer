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
#include <vector>
#include <memory>

Renderer::Renderer(unsigned w, unsigned h, std::string const& file)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , ppm_(width_, height_)
{}

Camera c1 = Camera{{0.0f,0.0f,0.0f},{0.0f,0.0f,-1.0f},800,600};
Sphere sp2 = {{0.0f,0.0f,-40.0f},5.5f,"sp2",{0.7f,0.9f,1.0f}};
// Box b9 = Box{{0.0f,0.0f,0.0f},{3.0f,4.2f,-4.0f},"Box9"};
Box b9 = Box{{0.0f,0.0f,-40.0f},{50.0f,10.0f,-50.0f},"Box9"};

std::vector<Shape*> objects;


void Renderer::render()
{
  // std::unique_ptr<Shape> sp2ptr = std::make_unique<Sphere>(sp2);
  // objects.push_back(sp2ptr);
  // objects.push_back(sp2ptr);
  // objects.push_back(new Box{{0.0f,0.0f,-10.0f},{50.0f,10.0f,-50.0f},"Box9"});
  objects.push_back(new Sphere{{0.0f,0.0f,-40.0f},5.5f,"sp2",{0.7f,0.9f,1.0f}});

  std::size_t const checker_pattern_size = 20;


  for (unsigned y = 0; y < height_; ++y) {
    for (unsigned x = 0; x < width_; ++x) {
      Pixel p(x,y);
      p.color = trace(compute_ray(c1,p));
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

void Renderer::write(Pixel const& p)
{
  // flip pixels, because of opengl glDrawPixels
  size_t buf_pos = (width_*p.y + p.x);
  if (buf_pos >= color_buffer_.size() || (int)buf_pos < 0) {
    std::cerr << "Fatal Error Renderer::write(Pixel p) : "
      << "pixel out of ppm_ : "
      << (int)p.x << "," << (int)p.y
      << std::endl;
  } else {
    color_buffer_[buf_pos] = p.color;
  }

  ppm_.write(p);
}

Ray Renderer::compute_ray(Camera const& cam, Pixel const& p)
{
  // float pix_width = 1.0f/cam.plane_width_;
  float plane_width = tan(60*M_PI/180);
  float pix_width = plane_width/800;
  float plane_height = cam.plane_height_*pix_width;

  glm::vec3 pixel_on_plane = {((-plane_width/2.0f)+(pix_width*p.x)),((plane_height/2.0f)-(pix_width*p.y)),-1.0f};

  // glm::vec3 pixel_on_plane = {(cam.plane_pos_.x-((cam.plane_width_/2.0f)*pix_width)+(p.x*pix_width)),(cam.plane_pos_.y-((cam.plane_height_/2.0f)*pix_width)+(p.y*pix_width)),-1.0f};
  Ray eye_ray = {cam.cam_pos_,{pixel_on_plane.x,pixel_on_plane.y,pixel_on_plane.z}};
  if (p.x == 0 && p.y == 0)
  {
    std::cout << "X" << eye_ray.direction.x << "Y" << eye_ray.direction.y << "Z" << eye_ray.direction.z << std::endl;
  }
  return eye_ray;
}

Color Renderer::trace(Ray r)
{
  for (auto const& i : objects)
  {
    if (i->intersect(r))
    {
      return Color{255.0f,0.0f,0.0f};
    }
    else 
    {
    return Color{255.0f,255.0f,255.0f};
    }
  }

}