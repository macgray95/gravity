#include "Planet.h"

Planet::Planet(long double m, double x_p, double y_p, double x_v, double y_v, SDL_Surface img)
{
  x_pos = x_p;
  y_pos = y_p;

  x_vel = x_v;
  y_vel = y_v;
  
  x_acc = 0;
  y_acc = 0;

  mass = m;

  pic = img;

  old_time = SDL_GetTicks();
}

void Planet::handle_events(SDL_Event event)
{
}

void Planet::handle_logic()
{
  int temp_time = SDL_GetTicks();
  x_vel = (x_acc * (temp_time - old_time) / 1000) + x_vel; //Change from milliseconds to seconds
  y_vel = (y_acc * (temp_time - old_time) / 1000) + y_vel; //v=at

  x_pos = x_vel * (temp_time - old_time) + x_pos;
  y_pos = SCREEN_HEIGHT - (y_vel * (temp_time - old_time) + (SCREEN_HEIGHT - y_pos)); //Flip over the middle of the screen because of SDL's weird coordinates

  old_time = SDL_GetTicks();
}

double Planet::get_x_pos()
{
  return x_pos;
}

double Planet::get_y_pos()
{
  return y_pos;
}

double Planet::get_mass()
{
  return mass;
}

SDL_Surface* Planet::get_pic()
{
  return &pic;
}

void Planet::run_time()
{
  old_time = SDL_GetTicks();
}

void Planet::zero_acc()
{
  x_acc = 0;
  y_acc = 0;
}

void Planet::set_force(Vector2d force)
{
  double x = force.magnitude * cos(force.direction);
  double y = force.magnitude * sin(force.direction);

  x_acc += x / mass;
  y_acc += y / mass;
}
