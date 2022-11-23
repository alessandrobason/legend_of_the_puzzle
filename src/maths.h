#pragma once

#include <cmath>
#include <raymath.h>

template<typename T>
struct TVec2 {
    T x, y;
    TVec2() : x(0), y(0) {}
    TVec2(T v) : x(v), y(v) {}
    TVec2(T x, T y) : x(x), y(y) {}
    TVec2(const Vector2 &v) : x((T)v.x), y((T)v.y) {}
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

    TVec2 operator-() const { return { -x, -y }; }

    bool operator==(const TVec2 &v) const { return x == v.x && y == v.y; }
    bool operator!=(const TVec2 &v) const { return x != v.x || y != v.y; }
    bool operator<(const TVec2 &v) const  { return x <  v.x && y <  v.y; }
    bool operator>(const TVec2 &v) const  { return x >  v.x && y >  v.y; }
    bool operator<=(const TVec2 &v) const { return x <= v.x && y <= v.y; }
    bool operator>=(const TVec2 &v) const { return x >= v.x && y >= v.y; }

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

    TVec3 operator-() const { return { -x, -y, -z }; }

    bool operator==(const TVec3 &v) const { return x == v.x && y == v.y; && z == v.z; }
    bool operator!=(const TVec3 &v) const { return x != v.x || y != v.y; || z != v.z; }
    bool operator<(const TVec3 &v) const  { return x <  v.x && y <  v.y; && z <  v.z; }
    bool operator>(const TVec3 &v) const  { return x >  v.x && y >  v.y; && z >  v.z; }
    bool operator<=(const TVec3 &v) const { return x <= v.x && y <= v.y; && z <= v.z; }
    bool operator>=(const TVec3 &v) const { return x >= v.x && y >= v.y; && z >= v.z; }

    operator Vector3() const { return { (float)x, (float)y, (float)z }; }
};

template<typename T>
struct TRect {
    union {
        struct { T x, y, w, h; };
        struct { T left, top, width, height; };
        struct { TVec2<T> pos, size; };
    };
    TRect() : x(0), y(0), w(0), h(0) {}
    TRect(T v) : x(v), y(v), w(v), h(v) {}
    TRect(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
    TRect(const TVec2<T> &p, const TVec2<T> &s) : pos(p), size(s) {}
    TRect(const Rectangle &r) : x(r.x), y(r.y), w(r.width), h(r.height) {}
    template<typename Q>
    TRect(const TRect<Q> &v) : x((T)v.x), y((T)v.y), w((T)v.w), h((T)v.h) {}

    TVec2<T> centre() const { return pos + size / (T)2; }
    bool intersects(const TRect &v) const { return CheckCollisionRecs(*this, v); }
    bool contains(TVec2<T> p) const { return CheckCollisionPointRec(p, *this); }

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

    bool operator==(const TRect &v) const { return x == v.x && y == v.y && w == v.w && h == v.h; }
    bool operator!=(const TRect &v) const { return !(*this == v); }

    operator Rectangle() const { return { (float)x, (float)y, (float)w, (float)h }; }
};

struct mat4 {
    union {
        float data[16];
        Matrix rl_mat;
    };

    mat4() : rl_mat(MatrixIdentity()) {}
    mat4(const Matrix &m) : rl_mat(m) {}

    void translate(TVec2<float> pos) { rl_mat = MatrixTranslate(pos.x, pos.y, 0.f); }
    void scale(TVec2<float> s) { rl_mat = MatrixScale(s.x, s.y, 0.f); }
    void rotateX(float angle) { rl_mat = MatrixRotateX(angle); }
    void rotateY(float angle) { rl_mat = MatrixRotateY(angle); }
    void rotateZ(float angle) { rl_mat = MatrixRotateZ(angle); }
    void rotate(float angle) { rotateZ(angle); }

    TVec2<float> transformPoint(TVec2<float> p) const { return Vector2Transform(p, rl_mat); }

    const float *getMatrix() const { return data; }
    
    mat4 operator+(const mat4 &m) const { return MatrixAdd(rl_mat, m.rl_mat); }
    mat4 operator-(const mat4 &m) const { return MatrixSubtract(rl_mat, m.rl_mat); }
    mat4 operator*(const mat4 &m) const { return MatrixMultiply(rl_mat, m.rl_mat); }

    mat4 &operator+=(const mat4 &m) { rl_mat = MatrixAdd(rl_mat, m.rl_mat); return *this; }
    mat4 &operator-=(const mat4 &m) { rl_mat = MatrixSubtract(rl_mat, m.rl_mat); return *this; }
    mat4 &operator*=(const mat4 &m) { rl_mat = MatrixMultiply(rl_mat, m.rl_mat); return *this; }

    float &operator[](size_t index) { return data[index]; }
    const float &operator[](size_t index) const { return data[index]; }

    operator Matrix() const { return rl_mat; }
};

using vec2  = TVec2<float>;
using vec2d = TVec2<double>;
using vec2u = TVec2<unsigned int>;
using vec2i = TVec2<int>;

using vec3  = TVec3<float>;
using vec3d = TVec3<double>;
using vec3u = TVec3<unsigned int>;
using vec3i = TVec3<int>;

using rectf = TRect<float>;
using rectd = TRect<double>;
using rectu = TRect<unsigned int>;
using recti = TRect<int>;