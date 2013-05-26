#include "GravityHandler.h"
#include <iostream>

#define G (6.67*pow(10, -11))

Vector2d GravityHandler::calculate_force(long double mass_1, long double mass_2, double x_1, double y_1, double x_2, double y_2)
{
  double distance = pow(pow(x_2-x_1, 2) + pow(y_2 - y_1, 2), .5);
  distance = distance < 64 ? 64 : distance;
  distance *= pow(10, 6); //change from pixels to 'km'

  double direction;

  y_1 = 480 - y_1;
  y_2 = 480 - y_2;
  direction = atan((y_1-y_2)/(x_1-x_2));
  if( x_1 >= x_2 )
    direction = PI + direction;

  long double magnitude = G*mass_1*mass_2*(pow(distance,-2));

  Vector2d force;
  force.magnitude = magnitude;
  force.direction = direction;
  return force;
}
//TODO:
//Change calculate_force so it works from each planet to another planet. This is at best half as efficient so see if there is another way.
