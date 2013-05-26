#include <math.h>
#ifndef _H_VECTOR2D
#include "Vector2d.h"
#define _H_VECTOR2D
#endif

#define PI (3.1415926)

class GravityHandler
{
 public:
  Vector2d calculate_force(long double mass_1, long double mass_2, double x_1, double y_1, double x_2, double y_2);
};
