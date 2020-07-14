#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include <glm/vec3.hpp>
#include <box.hpp>
#include <sphere.hpp>
#include <iostream>

TEST_CASE("Box area", "[Box_area]")
{
  glm::vec3 minimum1 = {1.0f, 0.0f, 5.0f};
  glm::vec3 maximum1 = {16.0f, 10.0f, 0.0f};
  Box b1 = Box{minimum1, maximum1, "Box1", {0.5f,0.5f,0.5f}};
  REQUIRE(b1.area() == 550.0f);

  glm::vec3 minimum2 = {-5.7f, 2.1f, -7.5f};
  glm::vec3 maximum2 = {19.2f, 5.6f, -15.5f};
  Box b2 = Box{minimum2, maximum2, "Box2", {0.3f,0.8f,0.9f}};
  REQUIRE(b2.area() == Approx(628.7f).epsilon(0.01));
}

TEST_CASE("Box volume", "[Box_volume]")
{
  glm::vec3 minimum1 = {1.0f, 0.0f, 5.0f};
  glm::vec3 maximum1 = {16.0f, 10.0f, 0.0f};
  Box b1 = Box{minimum1, maximum1, "Box1", {0.7f,0.7f,0.2f}};
  REQUIRE(b1.volume() == 750.0f);

  glm::vec3 minimum2 = {-5.7f, 2.1f, -7.5f};
  glm::vec3 maximum2 = {19.2f, 5.6f, -15.5f};
  Box b2 = Box{minimum2, maximum2, "Box2", {0.1f,0.6f,0.3f}};
  REQUIRE(b2.volume() == Approx(697.2f).epsilon(0.01));
}

TEST_CASE("Sphere area", "[Sphere_area]")
{
  glm::vec3 center1 = {50.0f, 50.0f, 0.0f};
  float radius1 = 10.0f;
  Sphere s1 = Sphere{center1, radius1, "Sphere1", {0.3f,0.5f,0.4f}};
  REQUIRE(s1.area() == Approx(1256.64).epsilon(0.01));

  glm::vec3 center2 = {100.0f, 100.0f, 50.0f};
  float radius2 = 2.4f;
  Sphere s2 = Sphere{center2, radius2, "Sphere2", {0.2f,0.9f,0.32f}};
  REQUIRE(s2.area() == Approx(72.38).epsilon(0.01));
}

TEST_CASE("Sphere volume", "[Sphere_volume]")
{
  glm::vec3 center1 = {50.0f, 50.0f, 0.0f};
  float radius1 = 10.0f;
  Sphere s1 = Sphere{center1, radius1};
  REQUIRE(s1.volume() == Approx(4188.79).epsilon(0.01));

  glm::vec3 center2 = {100.0f, 100.0f, 50.0f};
  float radius2 = 2.4f;
  Sphere s2 = Sphere{center2, radius2, "Sphere2", {0.75f,0.3f,0.62f}};
  REQUIRE(s2.volume() == Approx(57.905).epsilon(0.01));
}

TEST_CASE("Print function", "[Print]")
{
  Box b1 = Box{{4.7f, 1.0f, 0.0f}, {14.53f, 9.5f, -5.55f}, "Box1", {1.0f,1.0f,0.4f}};
  std::cout << b1 << std::endl;
  Box b2 = Box{{-7.3f, 6.2f, 10.3f}, {2.4f, 14.1f, 0.3f}, "Box2", {0.1f,0.6f,0.3f}};
  std::cout << b2 << std::endl;

  Sphere s1 = Sphere{{-10.0f, 23.4f, 0.5f}, 12.5f, "Sphere1", {0.9f,0.73f,0.2f}};
  std::cout << s1 << std::endl;
  Sphere s2 = Sphere{{34.5f, 27.0f, 90.5f}, 3.9f, "Sphere2", {0.2f,0.9f,0.32f}};
  std::cout << s2 << std::endl;
}


int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}
