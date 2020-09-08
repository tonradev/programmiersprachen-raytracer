#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <box.hpp>
#include <sphere.hpp>
#include <iostream>
#include "light.hpp"

/*
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

// EX. 5.6
TEST_CASE("intersect_ray_sphere", "[intersect]")
{
  // Ray
  glm::vec3 ray_origin{0.0f, 0.0f, 0.0f};
  // ray direction has to be normalized!
  // you can use:
  // v = glm::normalize(some_vector)
  glm::vec3 ray_direction{0.0f, 0.0f, 1.0f};

  glm::vec3 ray_direction_normalized = glm::normalize(ray_direction);

  // Sphere
  glm::vec3 sphere_center{0.0f, 0.0f, 5.0f};
  float sphere_radius{1.0f};

  Sphere sp1 = {sphere_center,sphere_radius,"sp1"};
  Sphere sp2 = {{25.0f,10.0f,20.0f},10.0f,"sp2",{0.7f,0.9f,1.0f}};

  Ray ra1 = {ray_origin, ray_direction};
  Ray ra2 = {{-90.0f, 10.0f, 0.0f},{0.0f, 1.0f, 0.0f}};
  Ray ra3 = {{0.0f,10.0f,20.0f},{1.0f,0.0f,0.0f}};
  Ray ra4 = {{25.0f,10.0f,20.0f},{0.5f,0.5f,0.5f}};
  Ray ra5 = {{50.0f,10.0f,30.0f},{-1.0f,0.0f,0.0f}};

  /*
  HitPoint hp1 = sp1.intersect(ra1);

  HitPoint hp2 = sp1.intersect(ra2);

  HitPoint hp3 = sp2.intersect(ra3);

  HitPoint hp4 = sp2.intersect(ra4);

  HitPoint hp5 = sp2.intersect(ra5);

  // sp1 x ra1
  REQUIRE(hp1.intersect == true);
  REQUIRE(hp1.distance == Approx(4.0f));
  REQUIRE(hp1.object_name == "sp1");

  // sp1 x ra2
  REQUIRE(hp2.intersect == false);

  // sp2 x ra3
  REQUIRE(hp3.intersect == true);
  REQUIRE(hp3.distance == Approx(15.0f));
  REQUIRE(hp3.object_name == "sp2");
  glm::vec3 isp_hp3 = {15.0f,10.0f,20.0f};
  REQUIRE(hp3.intersection_point == isp_hp3);

  // sp2 x ra4
  REQUIRE(hp4.intersect == true);
  REQUIRE(hp4.distance == Approx(10.0f));
  REQUIRE(hp4.object_name == "sp2");

  // sp2 x ra5
  REQUIRE(hp5.intersect == true);
  REQUIRE(hp5.distance == Approx(25.0f));
  REQUIRE(hp5.object_name == "sp2");
  glm::vec3 isp_hp5 = {25.0f,10.0f,30.0f};
  REQUIRE(hp5.intersection_point == isp_hp5);

  float distance = 0.0f;
  auto result = glm::intersectRaySphere(ray_origin, ray_direction_normalized, sphere_center, sphere_radius * sphere_radius, distance);
  
  REQUIRE(distance == Approx(4.0f));
  */

/*
}

TEST_CASE("intersect_ray_box", "[intersect]")
{
  // Ray
  Ray ra1 = {{25.0f,5.0f,10.0f},{0.0f,0.0f,-1.0f}};
  Ray ra2 = {{55.0f,5.0f,10.0f},{0.0f,0.0f,-1.0f}};
  Ray ra3 = {{25.0f,14.2f,10.0f},{0.0f,0.0f,-1.0f}};
  Ray ra4 = {{25.0f,5.0f,10.0f},{0.0f,1.0f,0.0f}};

  Box b9 = Box{{-50.0f,0.0f,0.0f},{50.0f,10.0f,-20.0f},"Box9"};

  REQUIRE(b9.intersect(ra1) == true);
  REQUIRE(b9.intersect(ra2) == false);
  REQUIRE(b9.intersect(ra3) == false);
  REQUIRE(b9.intersect(ra4) == false);
}

TEST_CASE("Destructors", "[Destructors]")
{
  std::cout << "CONSTRUCTOR/DESTRUCTOR TEST (EX. 5.8)" << std::endl;
  Color red{255,0,0};
  glm::vec3 position{0.0f,0.0f,0.0f};
  Sphere* s1 = new Sphere{position, 1.2f, "sphere0", red};
  Shape* s2 = new Sphere{position, 1.2f, "sphere1", red};
  
  s1->print(std::cout);
  s2->print(std::cout);

  std::cout << "Delete s1:" << std::endl;
  delete s1;
  std::cout << "Delete s2:" << std::endl;
  delete s2;
}
*/

TEST_CASE("TEST INTERSECTIONS", "[intersect]")
{
  Ray r1 = Ray{{0,0,0},{0,0,-1.0f}};
  /*
  Sphere s1 = Sphere{{0,0,-40.0f},10.0f,"Sphere1","red"};
  std::cout << s1.intersect(r1).intersect << std::endl;
  std::cout << s1.intersect(r1).intersection_point.x << std::endl;
    std::cout << s1.intersect(r1).intersection_point.y << std::endl;

  std::cout << s1.intersect(r1).intersection_point.z << std::endl;
  */
  Box b1 = Box{{-5.0f,-5.0f,-40.0f},{5.0f,5.0f,-50.0f},"Box1","red"};

  // glm::vec3 hp = b1.intersect(r1).intersection_point;

  Ray r_fromleft = Ray{{-20.0f,0,-45.0f},{1.0f,0,0}};

  Ray r_fromtop = Ray{{0,15.0f,-45.0f},{0,-1.0f,0}};

  Ray r_fromright = Ray{{20.0f,0,-45.0f},{-1.0f,0,0}};

  Ray r_frombottom = Ray{{0,-30.0f,-42.0f},{0,1.0f,0}};

  Ray r_fromback = Ray{{0,0,-62.0f},{0,0,1.0f}};

  glm::vec3 expected_hp_r1 = glm::vec3{0,0,-40.0f};
  glm::vec3 expected_hp_r_fromleft = glm::vec3{-5.0f,0,-45.0f};
  glm::vec3 expected_hp_r_fromtop = glm::vec3{0,5.0f,-45.0f};
  glm::vec3 expected_hp_r_fromright = glm::vec3{5.0f,0,-45.0f};
  glm::vec3 expected_hp_r_frombottom = glm::vec3{0,-5.0f,-42.0f};
  glm::vec3 expected_hp_r_fromback = glm::vec3{0,0,-50.0f};


  std::cout << "INTERSECT FROM BACK" << std::endl;
  std::cout << "DISTANCE: " << b1.intersect(r_fromback).distance << std::endl;
  std::cout << b1.intersect(r_fromback).intersection_point.x << std::endl;
  std::cout << b1.intersect(r_fromback).intersection_point.y << std::endl;
  std::cout << b1.intersect(r_fromback).intersection_point.z << std::endl;


  REQUIRE(b1.intersect(r1).intersection_point == expected_hp_r1);
  REQUIRE(b1.intersect(r_fromleft).intersection_point == expected_hp_r_fromleft);
  REQUIRE(b1.intersect(r_fromtop).intersection_point == expected_hp_r_fromtop);
  REQUIRE(b1.intersect(r_fromright).intersection_point == expected_hp_r_fromright);
  REQUIRE(b1.intersect(r_frombottom).intersection_point == expected_hp_r_frombottom);
  REQUIRE(b1.intersect(r_fromback).intersection_point == expected_hp_r_fromback);

  Light l1 = Light{"licht1",glm::vec3{0,-90.0f,-45.0f},Color{.2f,.2f,.2f},100};
  glm::vec3 hitpt = b1.intersect(r_fromtop).intersection_point;
  std::cout << b1.calcNormal(b1.calcNormal(hitpt)).x << " " << b1.calcNormal(b1.calcNormal(hitpt)).y << " " << b1.calcNormal(b1.calcNormal(hitpt)).z << std::endl;

  glm::vec3 N = glm::normalize(b1.calcNormal(hitpt));
  glm::vec3 L = glm::normalize(l1.pos-hitpt);

  std::cout << "L VECTOR: " << L.x << " " << L.y << " " << L.z << std::endl;

  std::cout << "DOT PRODUCT: " << glm::dot(N,L) << std::endl;

  glm::vec3 N2 = glm::vec3{0,1.0f,0};

  glm::vec3 L2 = glm::vec3{0.5f,0.5f,0};

  glm::vec3 R2 = -glm::reflect(L2,N2);

  std::cout << "REFLECT VECTOR TEST" << std::endl;
  std::cout << R2.x << " " << R2.y << " " << R2.z << std::endl;

  float cos_angle_spec = glm::dot(R2,N2);

  float cos_angle_2 = glm::dot(L2,N2);

  std::cout << cos_angle_spec << std::endl;

  std::cout << cos_angle_2 << std::endl;
}


int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}
