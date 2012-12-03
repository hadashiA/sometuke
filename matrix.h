#pragma once
#include "vector.h"

namespace kawaii {

template <typename T>
struct Matrix2 {
    Matrix2() {
        x.x = 1; x.y = 0;
        y.x = 0; y.y = 1;
    }

    Matrix2(const T* m) {
        x.x = m[0]; x.y = m[1];
        y.x = m[2]; y.y = m[3];
    }

    vec2 x;
    vec2 y;
};

template <typename T>
struct Matrix3 {
    Matrix3() {
        x.x = 1; x.y = 0; x.z = 0;
        y.x = 0; y.y = 1; y.z = 0;
        z.x = 0; z.y = 0; z.z = 1;
    }

    Matrix3(const T* m) {
        x.x = m[0]; x.y = m[1]; x.z = m[2];
        y.x = m[3]; y.y = m[4]; y.z = m[5];
        z.x = m[6]; z.y = m[7]; z.z = m[8];
    }

    Matrix3 Transposed() const {
        Matrix3 m;
        m.x.x = x.x; m.x.y = y.x; m.x.z = z.x;
        m.y.x = x.y; m.y.y = y.y; m.y.z = z.y;
        m.z.x = x.z; m.z.y = y.z; m.z.z = z.z;
        return m;
    }

    const T* Pointer() const {
        return &x.x;
    }

    vec3 x;
    vec3 y;
    vec3 z;
};

template <typename T>
struct Matrix4 {
    Matrix4() : x(1, 0, 0, 0),
                y(0, 1, 0, 0),
                z(0, 0, 1, 0),
                w(0, 0, 0, 1) {}

    Matrix4(const Matrix3<T>& m) : x(m.x), y(m.y), z(m.z), w(m.w) {}

    Matrix4(const T* m) :
        x(m[0],  m[1],  m[2],  m[3]),
        y(m[4],  m[5],  m[6],  m[7]),
        z(m[8],  m[9],  m[10], m[11]),
        w(m[12], m[13], m[14], m[15]) {}

    Matrix4(T xx, T xy, T xz, T xw,
            T yx, T yy, T yz, T yw,
            T zx, T zy, T zz, T zw,
            T wx, T wy, T wz, T ww) :
        x(xx, xy, xz, xw),
        y(yx, yy, yz, yw),
        z(zx, zy, zz, zw),
        w(wx, wy, wz, ww) {}

    Matrix4 operator * (const Matrix4& b) const {
        Matrix4 m;
        m.x.x = x.x * b.x.x + x.y * b.y.x + x.z * b.z.x + x.w * b.w.x;
        m.x.y = x.x * b.x.y + x.y * b.y.y + x.z * b.z.y + x.w * b.w.y;
        m.x.z = x.x * b.x.z + x.y * b.y.z + x.z * b.z.z + x.w * b.w.z;
        m.x.w = x.x * b.x.w + x.y * b.y.w + x.z * b.z.w + x.w * b.w.w;
        m.y.x = y.x * b.x.x + y.y * b.y.x + y.z * b.z.x + y.w * b.w.x;
        m.y.y = y.x * b.x.y + y.y * b.y.y + y.z * b.z.y + y.w * b.w.y;
        m.y.z = y.x * b.x.z + y.y * b.y.z + y.z * b.z.z + y.w * b.w.z;
        m.y.w = y.x * b.x.w + y.y * b.y.w + y.z * b.z.w + y.w * b.w.w;
        m.z.x = z.x * b.x.x + z.y * b.y.x + z.z * b.z.x + z.w * b.w.x;
        m.z.y = z.x * b.x.y + z.y * b.y.y + z.z * b.z.y + z.w * b.w.y;
        m.z.z = z.x * b.x.z + z.y * b.y.z + z.z * b.z.z + z.w * b.w.z;
        m.z.w = z.x * b.x.w + z.y * b.y.w + z.z * b.z.w + z.w * b.w.w;
        m.w.x = w.x * b.x.x + w.y * b.y.x + w.z * b.z.x + w.w * b.w.x;
        m.w.y = w.x * b.x.y + w.y * b.y.y + w.z * b.z.y + w.w * b.w.y;
        m.w.z = w.x * b.x.z + w.y * b.y.z + w.z * b.z.z + w.w * b.w.z;
        m.w.w = w.x * b.x.w + w.y * b.y.w + w.z * b.z.w + w.w * b.w.w;
        return m;
    }

    Vector4<T> operator * (const Vector4<T>& b) const {
        return Vector4<T>(x.x * b.x + x.y * b.y + x.z * b.z + x.w * b.w,
                          y.x * b.x + y.y * b.y + y.z * b.z + y.w * b.w,
                          z.x * b.x + z.y * b.y + z.z * b.z + z.w * b.w,
                          w.x * b.x + w.y * b.y + w.z * b.z + w.w * b.w);
    }

    Matrix4& operator *= (const Matrix4& b) {
        Matrix4 m = *this * b;
        return (*this = m);
    }

    Matrix4 Transposed() const {
        Matrix4 m;
        m.x.x = x.x; m.x.y = y.x; m.x.z = z.x; m.x.w = w.x;
        m.y.x = x.y; m.y.y = y.y; m.y.z = z.y; m.y.w = w.y;
        m.z.x = x.z; m.z.y = y.z; m.z.z = z.z; m.z.w = w.z;
        m.w.x = x.w; m.w.y = y.w; m.w.z = z.w; m.w.w = w.w;
        return m;
    }

    Matrix3<T> ToMat3() const {
        Matrix3<T> m;
        m.x.x = x.x; m.y.x = y.x; m.z.x = z.x;
        m.x.y = x.y; m.y.y = y.y; m.z.y = z.y;
        m.x.z = x.z; m.y.z = y.z; m.z.z = z.z;
        return m;
    }

    const T* Pointer() const {
        return &x.x;
    }

    static Matrix4<T> Identity() {
        return Matrix4();
    }

    static Matrix4<T> Translate(T x, T y, T z) {
        Matrix4 m;
        m.x.x = 1; m.x.y = 0; m.x.z = 0; m.x.w = 0;
        m.y.x = 0; m.y.y = 1; m.y.z = 0; m.y.w = 0;
        m.z.x = 0; m.z.y = 0; m.z.z = 1; m.z.w = 0;
        m.w.x = x; m.w.y = y; m.w.z = z; m.w.w = 1;
        return m;
    }

    static Matrix4<T> Scale(T s) {
        Matrix4 m;
        m.x.x = s; m.x.y = 0; m.x.z = 0; m.x.w = 0;
        m.y.x = 0; m.y.y = s; m.y.z = 0; m.y.w = 0;
        m.z.x = 0; m.z.y = 0; m.z.z = s; m.z.w = 0;
        m.w.x = 0; m.w.y = 0; m.w.z = 0; m.w.w = 1;
        return m;
    }

    static Matrix4<T> Rotate(T degrees) {
        T radians = degrees * 3.14159f / 180.0f;
        T s = std::sin(radians);
        T c = std::cos(radians);
        
        Matrix4 m;
        m.x.x =  c; m.x.y = s; m.x.z = 0; m.x.w = 0;
        m.y.x = -s; m.y.y = c; m.y.z = 0; m.y.w = 0;
        m.z.x =  0; m.z.y = 0; m.z.z = 1; m.z.w = 0;
        m.w.x =  0; m.w.y = 0; m.w.z = 0; m.w.w = 1;
        return m;
    }

    static Matrix4<T> Perspective(float fovY, float aspect, float zNear, float zFar) {
	float r = DegreesToRadians(fovY / 2);
        float deltaZ = zFar - zNear;
        float s = std::sin(r);

        float cotangent = std::cos(r) / s;

	float xx = cotangent / aspect;
	float yy =  cotangent;
	float zz = -(zFar + zNear) / deltaZ;
	float zw = -1;
	float wz = -2 * zNear * zFar / deltaZ;

        return Matrix4(xx,  0,  0,  0,
                        0, yy,  0,  0,
                        0,  0, zz, zw,
                        0,  0, wz,  0);
    }

    static Matrix4<T> LookAt(const Vector3<T>& eye,
                             const Vector3<T>& target,
                             const Vector3<T>& up) {
        Vector3<T> z = (eye - target).Normalized();
        Vector3<T> x = up.Cross(z).Normalized();
        Vector3<T> y = z.Cross(x).Normalized();
        
        Matrix4<T> m;
        m.x = Vector4<T>(x, 0);
        m.y = Vector4<T>(y, 0);
        m.z = Vector4<T>(z, 0);
        m.w = Vector4<T>(0, 0, 0, 1);
        
        Vector4<T> eyePrime = m * Vector4<T>(-eye, 1);
        m = m.Transposed();
        m.w = eyePrime;
        
        return m;
    }

    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
};

typedef Matrix2<float> mat2;
typedef Matrix3<float> mat3;
typedef Matrix4<float> mat4;

} // namespace kawaii
