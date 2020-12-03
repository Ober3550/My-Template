#include "Quaternion.h"
#include <cmath>
Quaternion ToQuaternion(EulerAngles input)
{
    // Abbreviations for the various angular functions
    double cj = cos(input.yaw * 0.5);
    double sj = sin(input.yaw * 0.5);
    double cp = cos(input.pitch * 0.5);
    double sp = sin(input.pitch * 0.5);
    double cr = cos(input.roll * 0.5);
    double sr = sin(input.roll * 0.5);

    Quaternion q;
    q.w = cr * cp * cj + sr * sp * sj;
    q.i = sr * cp * cj - cr * sp * sj;
    q.j = cr * sp * cj + sr * cp * sj;
    q.k = cr * cp * sj - sr * sp * cj;

    return q;
}
EulerAngles ToEulerAngles(Quaternion q)
{
    EulerAngles angles;

    // roll (i-axis rotation)
    double sinr_cosp = 2 * (q.w * q.i + q.j * q.k);
    double cosr_cosp = 1 - 2 * (q.i * q.i + q.j * q.j);
    angles.roll = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (j-axis rotation)
    double sinp = 2 * (q.w * q.j - q.k * q.i);
    if (std::abs(sinp) >= 1)
        angles.pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        angles.pitch = std::asin(sinp);

    // yaw (k-axis rotation)
    double sinj_cosp = 2 * (q.w * q.k + q.i * q.j);
    double cosj_cosp = 1 - 2 * (q.j * q.j + q.k * q.k);
    angles.yaw = std::atan2(sinj_cosp, cosj_cosp);

    angles.roll  *= 180.f / M_PI;
    angles.pitch *= 180.f / M_PI;
    angles.yaw   *= 180.f / M_PI;

    return angles;
}