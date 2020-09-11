// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#ifndef BUW_RENDERER_HPP
#define BUW_RENDERER_HPP

#include "color.hpp"
#include "pixel.hpp"
#include "ppmwriter.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include <string>
#include <glm/glm.hpp>
#include "renderprops.hpp"
#include "sdfparser.hpp"


class Renderer
{
public:
  Renderer(unsigned w, unsigned h, std::string const &file, SdfParser const& parser, RenderProps const& render_props, std::vector<Shape*> objects, std::map<std::string, Material> const& materials,std::vector<Light> const& lights, Camera const& cam);

  void render();
  void write(Pixel const& p);
  Ray compute_ray(Camera const& cam, Pixel const& p);
  Color trace(Ray r);

  inline std::vector<Color> const& color_buffer() const
  {
    return color_buffer_;
  }

private:
  unsigned width_;
  unsigned height_;
  std::vector<Color> color_buffer_;
  std::string filename_;
  SdfParser parser_;
  RenderProps render_props_;
  std::vector<Shape *> objects_;
  std::map<std::string, Material> materials_;
  std::vector<Light> lights_;
  Camera cam_;
  PpmWriter ppm_;
  
};

#endif // #ifndef BUW_RENDERER_HPP
