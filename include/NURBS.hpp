// Computer graphic lab 7
// Variant 20
// Copyright © 2017-2018 Roman Khomenko (8O-308)
// All rights reserved

#ifndef CG_LAB_NURBS_HPP_
#define CG_LAB_NURBS_HPP_

#include <Point.hpp>

#include <vector>

class NURBS {
public:
    NURBS(int n, int k, const std::vector<Point>& p);

    Point R(float t) const;

private:
    float N_i_k(int i, int k, float t) const;

    int N;
    int K;
    std::vector<float> T;
    std::vector<Point> P;
};

#endif  // CG_LAB_NURBS_HPP_
