// Computer graphic lab 7
// Variant 20
// Copyright © 2017-2018 Roman Khomenko (8O-308)
// All rights reserved

#ifndef CG_LAB_POINT_HPP_
#define CG_LAB_POINT_HPP_

#include <cstddef>

class Point {
public:
    constexpr Point() : Point(0, 0, 0) {}
    constexpr Point(float x, float y, float w) : Data{x, y, w} {}

    constexpr Point(const Point& point) = default;

    constexpr float GetX() const noexcept { return Data[0]; }
    constexpr float GetY() const noexcept { return Data[1]; }
    constexpr float GetW() const noexcept { return Data[2]; }

    constexpr Point operator*(float alpha) const noexcept {
        return Point(GetX() * alpha, GetY() * alpha, GetW());
    }

    constexpr Point operator+(const Point& p) const noexcept {
        return Point(GetX() + p.GetX(), GetY() + p.GetY(), GetW());
    }

    constexpr Point operator/(float alpha) const noexcept {
        return Point(GetX() / alpha, GetY() / alpha, GetW());
    }

    constexpr const Point& operator+=(const Point& p) noexcept {
        *this = *this + p;
        return *this;
    }

    constexpr bool operator==(const Point& other) const noexcept {
        return GetX() == other.GetX() && GetY() == other.GetY();
    }

    static constexpr auto GetOffset() noexcept { return offsetof(Point, Data); }
    static constexpr auto GetStride() noexcept { return sizeof(Point); }
    static constexpr auto GetTupleSize() noexcept { return TUPLE_SIZE; }

private:
    static constexpr auto TUPLE_SIZE = 2;

    float Data[3];
};

#endif  // CG_LAB_POINT_HPP_
