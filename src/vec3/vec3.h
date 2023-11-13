#pragma once

#include <cmath>
#include <utils/utils.h>

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

    Vec3 operator*(const double t) const {
        return t * *this;
    }

    Vec3 operator/(const double t) const {
        return (1 / t) * *this;
    }

    [[nodiscard]] double length() const { return sqrt(lengthSquared()); }

    [[nodiscard]] double lengthSquared() const { return data_[0]*data_[0] + data_[1]*data_[1] + data_[2]*data_[2]; }

    // Returns true if the vector is close to zero in all dimensions.
    [[nodiscard]] bool isNearZero() const {
        return (fabs(data_[0]) < 1e-8) && (fabs(data_[1]) < 1e-8) && (fabs(data_[2]) < 1e-8);
    }

    [[nodiscard]] Vec3 reflect(const Vec3 unit_vec) const {
        return *this - 2 * dot(*this, unit_vec) * unit_vec;
    }

    [[nodiscard]] Vec3 cross(const Vec3 &v) const {
        return { this->data_[1] * v.data_[2] - this->data_[2] * v.data_[1],
                    this->data_[2] * v.data_[0] - this->data_[0] * v.data_[2],
                    this->data_[0] * v.data_[1] - this->data_[1] * v.data_[0] };
    }

    [[nodiscard]] static Vec3 refract(Vec3 unit_vec, Vec3 normal_vec, double etai_over_etat);

    [[nodiscard]] static Vec3 generateRandomVec3() {
        return { utils::getRandomNormalDouble(), utils::getRandomNormalDouble(), utils::getRandomNormalDouble() };
    }

    [[nodiscard]] static Vec3 generateRandomVec3(const double min, const double max) {
        return { utils::getRandomDouble(min, max), utils::getRandomDouble(min, max), utils::getRandomDouble(min, max) };
    }

    // Generate a random vector in the unit sphere using the rejection method (brute force)
    [[nodiscard]] static Vec3 generateRandomVec3InUnitSphere();

    [[nodiscard]] static Vec3 getUnitVector(const Vec3 vec) {
        return vec / vec.length();
    }

    [[nodiscard]] static Vec3 generateRandomUnitVector() {
        return { getUnitVector(generateRandomVec3InUnitSphere()) };
    }

    [[nodiscard]] static Vec3 generateRandomVec3InHemisphere(const Vec3 normal) {
        if (const auto on_unit_sphere = generateRandomUnitVector(); dot(on_unit_sphere, normal) > 0.0)
            return on_unit_sphere;
        else
            // Need to flip the direction of the vector if it's not in the same hemisphere as the normal
            return -on_unit_sphere;
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
