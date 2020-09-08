#include <renderer.hpp>
#include <window.hpp>
#include <sdfparser.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>
#include <box.hpp>
#include <sdfparser.hpp>

//now single threaded again
int main(int argc, char* argv[])
{

  SdfParser parser = {"scene1.sdf"};

  RenderProps render_props = parser.getRenderProperties();

  unsigned const image_width = render_props.x_res;
  unsigned const image_height = render_props.y_res;
  std::string const filename = render_props.filename;

  Renderer renderer{image_width, image_height, filename};

  renderer.render();

  Window window{{image_width, image_height}};

  while (!window.should_close()) {
    if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      window.close();
    }
    window.show(renderer.color_buffer());
  }

  return 0;
}
