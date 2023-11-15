#pragma once

#include "../ray/ray.h"
#include "../vec3/vec3.h"

/**
 * \brief Class representing an intersection between a ray and a shape.
 */
class Intersection {
public:
    Point3 p {};
    Vec3 normal {};
    double t {};
    bool front_face {};
    std::shared_ptr<Material> material {};

    /**
     * \brief Sets the hit record normal vector.
     * \param ray the ray that hit the shape
     * \param outward_normal the normal vector of the shape at the hit point.
     * It is assumed that the outward_normal has unit length.
     */
    void setFaceNormal(const Ray& ray, const Vec3& outward_normal) {
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(ray.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};
