#pragma once

/**
 * TrQuadTree.hpp
 */

template <class T>
class TrQuadTree {
 public:
  T m_dat;
  TrQuadTree* m_children[4];

  TrQuadTree();
  virtual ~TrQuadTree();
};