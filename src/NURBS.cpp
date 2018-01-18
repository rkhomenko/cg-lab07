// Computer graphic lab 7
// Variant 20
// Copyright © 2017-2018 Roman Khomenko (8O-308)
// All rights reserved

#include <NURBS.hpp>

NURBS::NURBS(int n, int k, const std::vector<Point>& p) : N{n}, K{k}, P{p} {
    const auto step = 1.0f / (N + K);
    T.resize(N + K + 1);
    for (auto i = 0; i <= N + K; i++) {
        T[i] = i * step;
    }
}

Point NURBS::R(float t) const {
    auto p = Point();

    for (auto i = 0; i <= N; i++) {
        p += P[i] * P[i].GetW()* N_i_k(i, K, t);
    }

    auto coeff = 0.0f;
    for (auto i = 0; i <= N; i++) {
        coeff += P[i].GetW()* N_i_k(i, K, t);
    }

    return (coeff == 0.0f) ? p : p / coeff;
}

float NURBS::N_i_k(int i, int k, float t) const {
    if (k == 1) {
        if (T[i] <= t && t < T[i + 1]) {
            return 1;
        } else {
            return 0;
        }
    }

    float t1 = (t - T[i]) / (T[i + k - 1] - T[i]);
    float t2 = (T[i + k] - t) / (T[i + k] - T[i + 1]);
    return t1 * N_i_k(i, k - 1, t) + t2 * N_i_k(i + 1, k - 1, t);
}
