#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#ifndef _H_VECTOR2D
#include "Vector2d.h"
#define _H_VECTOR2D
#endif

//Define this somewhere else later but it goes here for now
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class Planet
{
 private:
  double x_pos;
  double y_pos;
  double x_vel;
  double y_vel;
  double x_acc;
  double y_acc;

  long double mass;

  int old_time;

  SDL_Surface pic;

 public:
  Planet();
  Planet(long double m, double x_p, double y_p, double x_v, double y_v, SDL_Surface img);
  void handle_events(SDL_Event event);
  void handle_logic();

  double get_x_pos();
  double get_y_pos();
  double get_mass();
  SDL_Surface* get_pic();

  void run_time();
  void zero_acc();
  void set_force(Vector2d force);
  void set_x_vel(double v);
  void set_y_vel(double v);
  void set_mass(long double m);
  void set_x_pos(double x);
  void set_y_pos(double y);
  void set_pic(SDL_Surface img);
};
