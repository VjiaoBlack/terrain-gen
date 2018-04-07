#pragma once

/**
 * TrMath.hpp
 */

#define TR_ASSIGN_MOD(x, y) \
  {                         \
    while (x >= y) {        \
      x -= y;               \
    }                       \
  }

// TODO: check this
#define TR_CART_TO_SPH(a, e, r, x, y, z) \
  {                                      \
    r = sqrt(x * x + y * y + z * z);     \
    e = atan2(z, sqrt(x * x + y * y));   \
    a = atan2(x, y);                     \
  }

#define TR_SPH_TO_CART(a, e, r, x, y, z) \
  {                                      \
    z = r * sin(e);                      \
    x = cos(e) * sin(a);                 \
    y = cos(e) * cos(a);                 \
  }
