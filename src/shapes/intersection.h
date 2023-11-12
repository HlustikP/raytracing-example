#pragma once

#include "../ray/ray.h"
#include "../vec3/vec3.h"

class Intersection {
public:
    Point3 p {};
    Vec3 normal {};
    double t {};
    bool front_face {};
    std::shared_ptr<Material> material {};

    // Sets the hit record normal vector.
    void setFaceNormal(const Ray& ray, const Vec3& outward_normal) {
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(ray.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};
