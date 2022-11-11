#pragma once

#include <cmath>
#include <raylib.h>

template<typename T>
struct TVec2 {
    T x, y;
    TVec2() : x(0), y(0) {}
    TVec2(T v) : x(v), y(v) {}
    TVec2(T x, T y) : x(x), y(y) {}
    TVec2(const Vector2 &v) : x(v.x), y(v.y) {}
    template<typename Q>
    TVec2(const TVec2<Q> &v) : x((T)v.x), y((T)v.y) {}

    T mag2() const { return x * x + y * y; }
    T mag() const { return std::sqrt(mag2()); }
    void normalise() { T m = mag(); if (m) *this /= m; }
    TVec2 normal() const { TVec2 v = *this; v.normalise(); return v; }

    TVec2 operator+(const TVec2 &v) const { return { x + v.x, y + v.y }; }
    TVec2 operator-(const TVec2 &v) const { return { x - v.x, y - v.y }; }
    TVec2 operator*(const TVec2 &v) const { return { x * v.x, y * v.y }; }
    TVec2 operator/(const TVec2 &v) const { return { x / v.x, y / v.y }; }

    TVec2 &operator+=(const TVec2 &v) { x += v.x; y += v.y; return *this; }
    TVec2 &operator-=(const TVec2 &v) { x -= v.x; y -= v.y; return *this; }
    TVec2 &operator*=(const TVec2 &v) { x *= v.x; y *= v.y; return *this; }
    TVec2 &operator/=(const TVec2 &v) { x /= v.x; y /= v.y; return *this; }

    TVec2 operator+(T v) const { return { x + v, y + v }; }
    TVec2 operator-(T v) const { return { x - v, y - v }; }
    TVec2 operator*(T v) const { return { x * v, y * v }; }
    TVec2 operator/(T v) const { return { x / v, y / v }; }

    TVec2 &operator+=(T v) { x += v; y += v; return *this; }
    TVec2 &operator-=(T v) { x -= v; y -= v; return *this; }
    TVec2 &operator*=(T v) { x *= v; y *= v; return *this; }
    TVec2 &operator/=(T v) { x /= v; y /= v; return *this; }

    bool operator==(const TVec2 &v) const { return x == v.x && y == v.y; }
    bool operator!=(const TVec2 &v) const { return x != v.x || y != v.y; }

    operator Vector2() const { return { (float)x, (float)y }; }
};

template<typename T>
struct TVec3 {
    T x, y, z;
    TVec3() : x(0), y(0), z(0) {}
    TVec3(T v) : x(v), y(v), z(v) {}
    TVec3(T x, T y, T z) : x(x), y(y), z(z) {}
    TVec3(const Vector3 &v) : x(v.x), y(v.y), z(v.z) {}
    template<typename Q>
    TVec3(const TVec3<Q> &v) : x((T)v.x), y((T)v.y), z((T)v.z) {}

    TVec3 operator+(const TVec3 &v) const { return { x + v.x, y + v.y, z + v.z }; }
    TVec3 operator-(const TVec3 &v) const { return { x - v.x, y - v.y, z - v.z }; }
    TVec3 operator*(const TVec3 &v) const { return { x * v.x, y * v.y, z * v.z }; }
    TVec3 operator/(const TVec3 &v) const { return { x / v.x, y / v.y, z / v.z }; }

    TVec3 &operator+=(const TVec3 &v) { x += v.x; y += v.y; z += v.z ; return *this; }
    TVec3 &operator-=(const TVec3 &v) { x -= v.x; y -= v.y; z -= v.z ; return *this; }
    TVec3 &operator*=(const TVec3 &v) { x *= v.x; y *= v.y; z *= v.z ; return *this; }
    TVec3 &operator/=(const TVec3 &v) { x /= v.x; y /= v.y; z /= v.z ; return *this; }

    TVec3 operator+(T v) const { return { x + v, y + v, z + v }; }
    TVec3 operator-(T v) const { return { x - v, y - v, z - v }; }
    TVec3 operator*(T v) const { return { x * v, y * v, z * v }; }
    TVec3 operator/(T v) const { return { x / v, y / v, z / v }; }

    TVec3 &operator+=(T v) { x += v; y += v; z += v; return *this; }
    TVec3 &operator-=(T v) { x -= v; y -= v; z -= v; return *this; }
    TVec3 &operator*=(T v) { x *= v; y *= v; z *= v; return *this; }
    TVec3 &operator/=(T v) { x /= v; y /= v; z /= v; return *this; }

    operator Vector3() const { return { (float)x, (float)y, (float)z }; }
};

template<typename T>
struct TRect {
    union {
        struct { T x, y, w, h; };
        struct { TVec2<T> pos, size; };
    };
    TRect() : x(0), y(0), w(0), h(0) {}
    TRect(T v) : x(v), y(v), w(v), h(v) {}
    TRect(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
    TRect(const TVec2<T> &p, const TVec2<T> &s) : pos(p), size(s) {}
    template<typename Q>
    TRect(const TRect<Q> &v) : x((T)v.x), y((T)v.y), w((T)v.w), h((T)v.h) {}

    TRect operator+(const TRect &v) const { return { x + v.x, y + v.y, w + v.w, h + v.h }; }
    TRect operator-(const TRect &v) const { return { x - v.x, y - v.y, w - v.w, h - v.h }; }
    TRect operator*(const TRect &v) const { return { x * v.x, y * v.y, w * v.w, h * v.h }; }
    TRect operator/(const TRect &v) const { return { x / v.x, y / v.y, w / v.w, h / v.h }; }

    TRect &operator+=(const TRect &v) { x += v.x; y += v.y; w += v.w; h += v.h ; return *this; }
    TRect &operator-=(const TRect &v) { x -= v.x; y -= v.y; w -= v.w; h -= v.h ; return *this; }
    TRect &operator*=(const TRect &v) { x *= v.x; y *= v.y; w *= v.w; h *= v.h ; return *this; }
    TRect &operator/=(const TRect &v) { x /= v.x; y /= v.y; w /= v.w; h /= v.h ; return *this; }

    TRect operator+(T v) const { return { x + v, y + v, w + v, h + v }; }
    TRect operator-(T v) const { return { x - v, y - v, w - v, h - v }; }
    TRect operator*(T v) const { return { x * v, y * v, w * v, h * v }; }
    TRect operator/(T v) const { return { x / v, y / v, w / v, h / v }; }

    TRect &operator+=(T v) { x += v; y += v; w += v; h += v; return *this; }
    TRect &operator-=(T v) { x -= v; y -= v; w -= v; h -= v; return *this; }
    TRect &operator*=(T v) { x *= v; y *= v; w *= v; h *= v; return *this; }
    TRect &operator/=(T v) { x /= v; y /= v; w /= v; h /= v; return *this; }

    operator Rectangle() const { return { (float)x, (float)y, (float)w, (float)h }; }
};

using vec2  = TVec2<float>;
using vec2d = TVec2<double>;
using vec2u = TVec2<unsigned int>;
using vec2i = TVec2<int>;

using vec3  = TVec3<float>;
using vec3d = TVec3<double>;
using vec3u = TVec3<unsigned int>;
using vec3i = TVec3<int>;

using rect  = TRect<float>;
using rectd = TRect<double>;
using rectu = TRect<unsigned int>;
using recti = TRect<int>;