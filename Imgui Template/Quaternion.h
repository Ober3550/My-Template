#pragma once
#include <cmath>
#define M_PI 3.14159265358979323846
struct EulerAngles {
public:
	float roll = 0;
	float pitch = 0;
	float yaw = 0;
	EulerAngles operator+(const EulerAngles& other) {
		this->roll += other.roll;
		this->pitch += other.pitch;
		this->yaw += other.yaw;
		return *this;
	}
};
struct Quaternion {
	float w = 0;
	float i = 0;
	float j = 0;
	float k = 0;
	Quaternion operator+(const Quaternion& other) {
		this->w += other.w;
		this->i += other.i;
		this->j += other.j;
		this->k += other.k;
		return *this;
	}
	void operator=(const Quaternion& other) {
		this->w = other.w;
		this->i = other.i;
		this->j = other.j;
		this->k = other.k;
	}
	Quaternion operator*(const Quaternion& other) {
		this->w = (this->w * other.w) - (this->i * other.i) - (this->j * other.j) - (this->k * other.k);
		this->i = (this->w * other.i) + (this->i * other.w) + (this->j * other.k) - (this->k * other.j);
		this->j = (this->w * other.j) + (this->j * other.w) + (this->k * other.i) - (this->i * other.k);
		this->k = (this->w * other.k) + (this->k * other.w) + (this->i * other.j) - (this->j * other.i);
		return *this;
	}
	Quaternion dot(const Quaternion& other) {
		this->w *= other.w;
		this->i *= other.i;
		this->j *= other.j;
		this->k *= other.k;
		return *this;
	}
	Quaternion operator/(const float scalar) {
		this->w /= scalar;
		this->i /= scalar;
		this->j /= scalar;
		this->k /= scalar;
		return *this;
	}
	Quaternion operator*(const float scalar) {
		this->w *= scalar;
		this->i *= scalar;
		this->j *= scalar;
		this->k *= scalar;
		return *this;
	}
	Quaternion mul(const float rot) {
		Quaternion returnVal;
		returnVal.w = this->w * cos(rot);
		returnVal.i = this->i * sin(rot);
		returnVal.j = this->j * sin(rot);
		returnVal.k = this->k * sin(rot);
		return returnVal;
	}
	static float abs(const Quaternion& input) {
		return sqrt(input.w * input.w + input.i * input.i + input.j * input.j + input.k * input.k);
	}
};
Quaternion ToQuaternion(EulerAngles input);
EulerAngles ToEulerAngles(Quaternion input);