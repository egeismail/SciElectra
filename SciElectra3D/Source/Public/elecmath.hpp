#pragma once
#include <math.h>
#include <sstream>


#define Vector(x,y) Vector2(x,y)
//#define Angle(pitch) Angle(pitch)
#define GRAVITATIONAL_CONSTANT 6.67430e-11f
#define PI 3.141592

struct Angle
{
	float pitch,yaw,roll;
	Angle(float pitch_,
        float yaw_,
        float roll_) {
        pitch = pitch_;
        yaw = yaw_;
        roll = roll_;
    }
	Angle& operator+=(Angle const& other) {
        pitch += other.pitch;
        yaw += other.yaw;
        roll += other.roll;
		return *this;
	}
	Angle& operator-=(Angle const& other) {
        pitch -= other.pitch;
        yaw -= other.yaw;
        roll -= other.roll;
		return *this;
	}
	Angle& operator*=(Angle const& other) {
        pitch *= other.pitch;
        yaw *= other.yaw;
        roll *= other.roll;
		return *this;
	}
	Angle& operator/=(Angle const& other) {
        pitch /= other.pitch;
        yaw /= other.yaw;
        roll /= other.roll;
		return *this;
	}
	Angle& operator =(const Angle& a)
	{
        pitch = a.pitch;
        yaw = a.yaw;
        roll = a.roll;
		return *this;
	}
	Angle operator+(const Angle& a) { return Angle(pitch + a.pitch,yaw +  a.yaw, roll + a.roll);}
    Angle operator-(const Angle& a) { return Angle(pitch - a.pitch, yaw - a.yaw, roll - a.roll); }
    Angle operator*(const Angle& a) { return Angle(pitch * a.pitch, yaw * a.yaw, roll * a.roll); }
    Angle operator/(const Angle& a) { return Angle(pitch / a.pitch, yaw / a.yaw, roll / a.roll); }
};
struct Vector3
{
    float x, y, z;
    Vector3(float x_ = 0, float y_ = 0, float z_ = 0) { x = x_; y = y_; z = z_;}
    Vector3 Copy() {
        return Vector3(x,y);
    }
    float getLength() {
        return sqrt((float)(x * x + y * y + z * z + z * z));
    }
    float getDistance(Vector3 from) {
        return sqrt(pow(x - from.x, 2) + pow(y - from.y, 2) + pow(z - from.z, 2));
    }
    float dot(Vector3 right) {
        return x * right.x + y * right.y + z * right.z;
    }
    void negate() {
        x = -x;
        y = -y;
        y = -z;
    }
    Angle getAngle() {
        return Angle(atan2f(y, sqrt((float)(x * x + y * y))), atan2f(y, x), 0);
    }
    Angle getAngleTo(Vector3 target) {
        return Angle(atan2f(target.y - y, sqrt((float)((target.x - x) * (target.x - x) + (target.y - y) * (target.y - y)))), atan2f(target.y - y, target.x - x), 0);
    }
    void reAngle(Angle angle) {
        float magnitude = getLength();
        x = magnitude * cosf(angle.pitch) * cosf(angle.yaw);
        y = magnitude * cosf(angle.pitch) * sinf(angle.yaw);
        z = magnitude * sinf(angle.pitch);
    }
    Vector3& operator+=(Vector3 const& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Vector3& operator-=(Vector3 const& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    Vector3& operator*=(Vector3 const& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }
    /*Vector3& operator*=(Angle const& other) {
        float size = this->getLength();
        x *= size * cosf(other.pitch);
        y *= size * sinf(other.pitch);
        return *this;
    }*/
    Vector3& operator/=(Vector3 const& other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }
    Vector3& operator =(const Vector3& a)
    {
        x = a.x;
        y = a.y;
        z = a.z;
        return *this;
    }
    /*Vector3 operator*(const Angle& anglus) {
        float size = this->getLength();
        return Vector3(size * cosf(anglus.pitch), size * sinf(anglus.pitch));
    }*/
    Vector3 operator+(const Vector3& a) { return Vector3(x + a.x, y + a.y, z + a.z); }
    Vector3 operator-(const Vector3& a) { return Vector3(x - a.x, y - a.y, z - a.z); }
    Vector3 operator*(const Vector3& a) { return Vector3(x * a.x, y * a.y, z * a.z); }
    Vector3 operator/(const Vector3& a) { return Vector3(x / a.x, y / a.y, z / a.z); }
    Vector3 operator*(const float& a) { return Vector3(x * a, y * a, z * a); }
    Vector3 operator/(const float& a) { return Vector3(x / a, y / a, z / a); }
    bool operator==(const Vector3& a) const
    {
        return (x == a.x && y == a.y && z == a.z);
    }
    bool operator>(const Vector3& a) const
    {
        /*CONTENTS SOME ARIMETIC OVERFLOW*/
        return sqrt((float)(x * x + y * y + z * z)) - sqrt((float)(a.x * a.x + a.y * a.y + a.z * a.z)) > 0;
    }
    bool operator<(const Vector3& a) const
    {
        /*CONTENTS SOME ARIMETIC OVERFLOW*/
        return sqrt((float)(x * x + y * y + z * z)) - sqrt((float)(a.x * a.x + a.y * a.y + a.z*a.z   )) < 0;
    }

};


