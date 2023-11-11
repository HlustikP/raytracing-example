#pragma once
#include <cmath>
#include <iostream>

using std::sqrt;

class Vec3 {
public:
    Vec3() : data_{0, 0, 0} {}
    Vec3(double e0, double e1, double e2) : data_{e0, e1, e2} {}

    [[nodiscard]] double x() const { return data_[0]; }
    [[nodiscard]] double y() const { return data_[1]; }
    [[nodiscard]] double z() const { return data_[2]; }

    Vec3 operator-() const { return {-data_[0], -data_[1], -data_[2]}; }
    double operator[](int i) const { return data_[i]; }
    double& operator[](int i) { return data_[i]; }

    Vec3& operator+=(const Vec3 &v);

    Vec3& operator*=(double t);

    Vec3& operator/=(double t) { return *this *= 1/t; }

    Vec3 operator+(const Vec3 &v) const {
        return { this->data_[0] + v.data_[0], this->data_[1] + v.data_[1], this->data_[2] + v.data_[2] };
    }

    Vec3 operator-(const Vec3 &v) const {
        return { this->data_[0] - v.data_[0], this->data_[1] - v.data_[1], this->data_[2] - v.data_[2] };
    }

    Vec3 operator*(const Vec3 &v) const {
        return { this->data_[0] * v.data_[0], this->data_[1] * v.data_[1], this->data_[2] * v.data_[2] };
    }

    Vec3 operator*(double t) const {
        return t * *this;
    }

    Vec3 operator/(double t) const {
        return (1/t) * *this;
    }

    [[nodiscard]] double length() const { return sqrt(length_squared()); }

    [[nodiscard]] double length_squared() const { return data_[0]*data_[0] + data_[1]*data_[1] + data_[2]*data_[2]; }

    [[nodiscard]] Vec3 cross(const Vec3 &v) {
        return { this->data_[1] * v.data_[2] - this->data_[2] * v.data_[1],
                    this->data_[2] * v.data_[0] - this->data_[0] * v.data_[2],
                    this->data_[0] * v.data_[1] - this->data_[1] * v.data_[0] };
    }

    [[nodiscard]] Vec3 unit_vector() const {
        return *this / this->length();
    }

    friend std::ostream& operator<<(std::ostream &out, const Vec3 &v);
    friend double dot(const Vec3 &u, const Vec3 &v);
    friend inline Vec3 operator*(double t, const Vec3 &v);

protected:
    double data_[3];
};

// useful for geometric clarity in the code
using Point3 = Vec3;

inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
    return out << v.data_[0] << ' ' << v.data_[1] << ' ' << v.data_[2];
}

inline double dot(const Vec3 &u, const Vec3 &v) {
    return u.data_[0] * v.data_[0]
        + u.data_[1] * v.data_[1]
        + u.data_[2] * v.data_[2];
}

inline Vec3 operator*(double t, const Vec3 &v) {
    return { t*v.data_[0], t*v.data_[1], t*v.data_[2] };
}
