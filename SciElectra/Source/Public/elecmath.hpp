#pragma once
#include <d2d1.h>
#include <math.h>
#define Vector(x,y) Vector2(x,y)
#define Angle(pitch) Angle(pitch)
#define GRAVITATIONAL_CONSTANT 6.67430e-11f
#define PI 3.1415926535926535



struct Vector2
{
	float x, y;
    Vector2(float x_ = 0, float y_ = 0) {x = x_;y = y_;}
    float getLength() {
        return sqrt((float)(x * x + y * y));
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
    Vector2 operator+(const Vector2& a) { return Vector2(x + a.x, y + a.y); }
    Vector2 operator-(const Vector2& a) { return Vector2(x - a.x, y - a.y); }
    Vector2 operator*(const Vector2& a) { return Vector2(x * a.x, y * a.y); }
    Vector2 operator/(const Vector2& a) { return Vector2(x / a.x, y / a.y);}
    Vector2 operator*(const float& a) { return Vector2(x * a, y * a); }
    Vector2 operator/(const float& a) { return Vector2(x / a, y / a); }
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
};