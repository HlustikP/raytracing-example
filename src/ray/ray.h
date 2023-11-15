#pragma once
#include "../vec3/vec3.h"

/**
 * \brief Class representing a ray shot into the scene from a camera.
 */
class Ray {
public:
    Ray() = delete;

    Ray(const Point3 &origin, const Vec3 &direction) : orig(origin), dir(direction) {}

    /**
     * \return the origin of the ray
     */
    [[nodiscard]] Point3 origin() const { return orig; }
    /**
     * \return the direction of the ray
     */
    [[nodiscard]] Vec3 direction() const { return dir; }
    /**
     * \brief Calculates a point along the ray given a distance from the origin.
     * \param distance the distance from the origin
     * \return the point on ray
     */
    [[nodiscard]] Point3 at(const double distance) const { return orig + distance * dir; }

private:
    Point3 orig {};
    Vec3 dir {};
};
