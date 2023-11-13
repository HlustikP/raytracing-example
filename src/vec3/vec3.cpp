#include "vec3.h"

Vec3 &Vec3::operator+=(const Vec3 &v) {
    data_[0] += v.data_[0];
    data_[1] += v.data_[1];
    data_[2] += v.data_[2];
    return *this;
}

Vec3 &Vec3::operator*=(double t) {
    data_[0] *= t;
    data_[1] *= t;
    data_[2] *= t;
    return *this;
}

Vec3 Vec3::refract(Vec3 unit_vec, Vec3 normal_vec, const double etai_over_etat) {
    const auto cos_theta = fmin(dot(-unit_vec, normal_vec), 1.0);
    const auto r_out_perp =  etai_over_etat * (unit_vec + cos_theta * normal_vec);
    const auto r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.lengthSquared())) * normal_vec;
    return r_out_perp + r_out_parallel;
}

Vec3 Vec3::generateRandomVec3InUnitSphere() {
    while (true) {
        if (auto vecCandidate = generateRandomVec3(-1,1); vecCandidate.lengthSquared() < 1) {
            return vecCandidate;
        }
    }
}
