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

Vec3 Vec3::generateRandomVec3InUnitSphere() {
    while (true) {
        if (auto vecCandidate = generateRandomVec3(-1,1); vecCandidate.lengthSquared() < 1) {
            return vecCandidate;
        }
    }
}
