#pragma once

/**
 * TrMath.hpp
 */

#include <glm/glm.hpp>

#define TR_ASSIGN_MOD(x, y) \
  {                         \
    while ((x) >= (y)) {        \
      (x) -= (y);               \
    }                       \
  }

using namespace glm;

// TODO: check this
#define TR_CART_TO_SPH(a, e, r, x, y, z) \
  {                                      \
    (r) = sqrt((x) * (x) + (y) * (y) + (z) * (z));     \
    (e) = atan2(z, sqrt((x) * (x) + (y) * (y)));   \
    (a) = atan2(x, y);                     \
  }

#define TR_SPH_TO_CART(a, e, r, x, y, z) \
  {                                      \
    (z) = (r) * sin(e);                      \
    (x) = cos(e) * sin(a);                 \
    (y) = cos(e) * cos(a);                 \
  }

inline dvec3 cartToSph(const dvec3 xyz) {
  return dvec3(length(xyz), atan2(xyz.z, length(dvec2(xyz))),
               atan2(xyz.x, xyz.y));
}

inline dvec3 sphToCart(const dvec3 rea) {
  return dvec3(cos(rea.y) * sin(rea.z), cos(rea.y) * cos(rea.z),
               rea.x * sin(rea.y));
}
