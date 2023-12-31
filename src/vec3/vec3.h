#pragma once

#include <cmath>
#include <utils/utils.h>

using std::sqrt;

/**
 * Class representing a 3D vector.
 */
class Vec3 {
public:
    Vec3() : data_{0, 0, 0} {}
    Vec3(double e0, double e1, double e2) : data_{e0, e1, e2} {}

    /**
     * \return the x component of the vector
     */
    [[nodiscard]] double x() const { return data_[0]; }
    /**
     * \return the y component of the vector
     */
    [[nodiscard]] double y() const { return data_[1]; }
    /**
     * \return the z component of the vector
     */
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

    /**
     * \return the length of the vector
     */
    [[nodiscard]] double length() const { return sqrt(lengthSquared()); }

    /**
     * \return the squared length of the vector
     */
    [[nodiscard]] double lengthSquared() const { return data_[0]*data_[0] + data_[1]*data_[1] + data_[2]*data_[2]; }

    /**
     * \brief approximate 0-equality check
     * \return true if the vector is near zero in all dimensions
     */
    [[nodiscard]] bool isNearZero() const {
        return (fabs(data_[0]) < 1e-8) && (fabs(data_[1]) < 1e-8) && (fabs(data_[2]) < 1e-8);
    }

    /**
     * \brief Calculate the reflection of this vector off a surface with the given normal.
     * \param unit_vec the unit vector to reflect off of
     * \return the vector representing the reflection
     */
    [[nodiscard]] Vec3 reflect(const Vec3 unit_vec) const {
        return *this - 2 * dot(*this, unit_vec) * unit_vec;
    }

    /**
     * \brief Calculate the cross product of the vector with another vector.
     * \param v the other vector
     * \return the cross product
     */
    [[nodiscard]] Vec3 cross(const Vec3 &v) const {
        return { this->data_[1] * v.data_[2] - this->data_[2] * v.data_[1],
                    this->data_[2] * v.data_[0] - this->data_[0] * v.data_[2],
                    this->data_[0] * v.data_[1] - this->data_[1] * v.data_[0] };
    }

    /**
     * \brief Calculate the refraction of this vector off a surface
     * \param unit_vec the unit vector to refract off of
     * \param normal_vec the normal vector of the surface
     * \param etai_over_etat the ratio of the refractive indices of the material
     * \return the vector representing the refraction
     */
    [[nodiscard]] static Vec3 refract(Vec3 unit_vec, Vec3 normal_vec, double etai_over_etat);

    /**
     * \return a random vector with each component in the range [0, 1]
     */
    [[nodiscard]] static Vec3 generateRandomVec3() {
        return { utils::getRandomNormalDouble(), utils::getRandomNormalDouble(), utils::getRandomNormalDouble() };
    }

    /**
     * \param min the minimum value for each component
     * \param max the maximum value for each component
     * \return a random vector with each component in the inclusive range [min, max]
     */
    [[nodiscard]] static Vec3 generateRandomVec3(const double min, const double max) {
        return { utils::getRandomDouble(min, max), utils::getRandomDouble(min, max), utils::getRandomDouble(min, max) };
    }

    /**
     * \brief Generate a random vector in the unit sphere using the rejection method (brute force)
     * \return a random vector in the unit sphere
     */
    [[nodiscard]] static Vec3 generateRandomVec3InUnitSphere();

    /**
     * \param vec the vector to get the unit vector of
     * \return the unit vector of the given vector
     */
    [[nodiscard]] static Vec3 getUnitVector(const Vec3 vec) {
        return vec / vec.length();
    }

    /**
     * \return a random unit vector
     */
    [[nodiscard]] static Vec3 generateRandomUnitVector() {
        return { getUnitVector(generateRandomVec3InUnitSphere()) };
    }

    /**
     * \brief Generate a random vector in the unit hemisphere using the rejection method (brute force)
     * \param normal the normal vector of the sphere
     * \return the random vector in the unit hemisphere
     */
    [[nodiscard]] static Vec3 generateRandomVec3InHemisphere(const Vec3 normal) {
        if (const auto on_unit_sphere = generateRandomUnitVector(); dot(on_unit_sphere, normal) > 0.0)
            return on_unit_sphere;
        else
            // Need to flip the direction of the vector if it's not in the same hemisphere as the normal
            return -on_unit_sphere;
    }

    /**
     * \return a random vector in the unit sphere using the rejection method (brute force)
     */
    [[nodiscard]] static Vec3 generate_random_vec_in_unit_sphere() {
        while (true) {
            if (const auto result = Vec3(utils::getRandomDouble(-1, 1), utils::getRandomDouble(-1, 1), 0);
                    result.lengthSquared() < 1) {
                return result;
            }
        }
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
