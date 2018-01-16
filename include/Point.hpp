// Computer graphic lab 7
// Variant 20
// Copyright © 2017-2018 Roman Khomenko (8O-308)
// All rights reserved

#ifndef CG_LAB_POINT_HPP_
#define CG_LAB_POINT_HPP_

#include <cstddef>

class Point {
public:
    constexpr Point() : Point(0, 0) {}
    constexpr Point(float x, float y) {
        Data[0] = x;
        Data[1] = y;
    }

    constexpr Point(const Point& point) = default;

    constexpr float GetX() const noexcept { return Data[0]; }
    constexpr float GetY() const noexcept { return Data[1]; }

    static constexpr auto GetOffset() noexcept { return offsetof(Point, Data); }
    static constexpr auto GetStride() noexcept { return sizeof(Point); }
    static constexpr auto GetTupleSize() noexcept { return TUPLE_SIZE; }

private:
    static constexpr auto TUPLE_SIZE = 2;

    float Data[2];
}

#endif  // CG_LAB_POINT_HPP_
