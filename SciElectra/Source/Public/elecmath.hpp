#pragma once
#include <d2d1.h>
#include <math.h>
#include <sstream>

#define Vector(x,y) Vector2(x,y)
//#define Angle(pitch) Angle(pitch)
#define GRAVITATIONAL_CONSTANT 6.67430e-11f
#define PI 3.141592

struct Angle
{
	float pitch;
	Angle(float pitch_) { pitch = pitch_; }
	Angle& operator+=(Angle const& other) {
		pitch += other.pitch;
		return *this;
	}
	Angle& operator-=(Angle const& other) {
		pitch -= other.pitch;
		return *this;
	}
	Angle& operator*=(Angle const& other) {
		pitch *= other.pitch;
		return *this;
	}
	Angle& operator/=(Angle const& other) {
		pitch /= other.pitch;
		return *this;
	}
	Angle& operator =(const Angle& a)
	{
		pitch = a.pitch;
		return *this;
	}
	Angle operator+(const Angle& a) { return Angle(pitch + a.pitch); }
	Angle operator-(const Angle& a) { return Angle(pitch - a.pitch); }
	Angle operator*(const Angle& a) { return Angle(pitch * a.pitch); }
	Angle operator/(const Angle& a) { return Angle(pitch / a.pitch); }

};
struct Vector2
{
    float x, y;
    Vector2(float x_ = 0, float y_ = 0) { x = x_; y = y_; }
    float getLength() {
        return sqrt((float)(x * x + y * y));
    }
    float getDistance(Vector2 from) {
        return sqrt(pow(x - from.x, 2) + pow(y - from.y, 2));
    }
    float dot(Vector2 right) {
        return x * right.x + y * right.y;
    }
	Angle getAngle() {
		return Angle(atan2f(y, x));
	}
	Angle getAngleTo(Vector2 target) {
		return Angle(atan2f(target.y-y, target.x-x));
	}
    void reAngle(Angle angle) {
        float magnitude = getLength();
        x = magnitude * cosf(angle.pitch);
        y = magnitude * sinf(angle.pitch);
    }
    Vector2& operator+=(Vector2 const& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vector2& operator-=(Vector2 const& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Vector2& operator*=(Vector2 const& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }
    /*Vector2& operator*=(Angle const& other) {
        float size = this->getLength();
        x *= size * cosf(other.pitch);
        y *= size * sinf(other.pitch);
        return *this;
    }*/
    Vector2& operator/=(Vector2 const& other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }
    Vector2& operator =(const Vector2& a)
    {
        x = a.x;
        y = a.y;
        return *this;
    }
    /*Vector2 operator*(const Angle& anglus) {
        float size = this->getLength();
        return Vector2(size * cosf(anglus.pitch), size * sinf(anglus.pitch));
    }*/
    Vector2 operator+(const Vector2& a) { return Vector2(x + a.x, y + a.y); }
    Vector2 operator-(const Vector2& a) { return Vector2(x - a.x, y - a.y); }
    Vector2 operator*(const Vector2& a) { return Vector2(x * a.x, y * a.y); }
    Vector2 operator/(const Vector2& a) { return Vector2(x / a.x, y / a.y); }
    Vector2 operator*(const float& a) { return Vector2(x * a, y * a); }
    Vector2 operator/(const float& a) { return Vector2(x / a, y / a); }
    D2D1_POINT_2F D2D1_POINT_2F() { return D2D1::Point2F(x, y); }
    bool operator==(const Vector2& a) const
    {
        return (x == a.x && y == a.y);
    }
    bool operator>(const Vector2& a) const
    {
        /*CONTENTS SOME ARIMETIC OVERFLOW*/
        return sqrt((float)(x * x + y * y)) - sqrt((float)(a.x * a.x + a.y * a.y)) > 0;
    }
    bool operator<(const Vector2& a) const
    {
        /*CONTENTS SOME ARIMETIC OVERFLOW*/
        return sqrt((float)(x * x + y * y)) - sqrt((float)(a.x * a.x + a.y * a.y)) < 0;
    }

};



static void dPrint(const char* title, double number) {
    std::stringstream pps;
    pps << title << " : " << number << "\n";
    OutputDebugStringA(pps.str().c_str());
}
static void dPrint(const char* title, Vector2 vect) {
    std::stringstream pps;
    pps << title << " : " << "(" << vect.x << "," << vect.y << ")\n";
    OutputDebugStringA(pps.str().c_str());
}