#include "MiniginPCH.h"
#include "Vectors.h"
#include "Math.h"

//----------------------------------------
//				Vector2
//----------------------------------------

Vector2::Vector2()
	: Vector2{ 0.f, 0.f }
{}

Vector2::Vector2(float x, float y)
	: x{ std::move(x) }
	, y{ std::move(y) }
{}

Vector2::Vector2(const Vector2& vec2)
	: x{ vec2.x }
	, y{ vec2.y }
{}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2{ this->x + other.x, this->y + other.y };
}

void Vector2::operator+=(const Vector2& other)
{
	this->x += other.x;
	this->y += other.y;
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2{ this->x - other.x, this->y - other.y };
}

void Vector2::operator-=(const Vector2& other)
{
	this->x -= other.x;
	this->y -= other.y;
}

Vector2 Vector2::operator*(const float scalar) const
{
	return Vector2{ this->x * scalar, this->y * scalar };
}

void Vector2::operator*=(const float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
}

Vector2 Vector2::operator/(const float scalar) const
{
	return Vector2{ this->x / scalar, this->y / scalar };
}

void Vector2::operator/=(const float scalar)
{
	this->x /= scalar;
	this->y /= scalar;
}

float Vector2::Magnitude() const
{
	return sqrtf(powf(this->x, 2) + powf(this->y, 2));
}

float Vector2::Dot(const Vector2& other) const
{
	return this->x * other.x + this->y * other.y;
}

float Vector2::Cross(const Vector2& other) const
{
	return this->x * other.y - this->y * other.x;
}

float Vector2::OrientOnWorld()
{
	return Vector2::Dot(Vector2{ 1.f, 0.f });
}

float Vector2::Normalize()
{
	const float magnitude = Magnitude();
	*this /= magnitude;
	return magnitude;
}

Vector2 Vector2::GetNormalized() const
{
	const float magnitude = Magnitude();
	return Vector2{ this->x / magnitude, this->y / magnitude };
}

//----------------------------------------
//				Vector3
//----------------------------------------


Vector3::Vector3()
	: Vector3{ 0.f, 0.f, 0.f }
{}

Vector3::Vector3(float x, float y, float z)
	: Vector2{ std::move(x), std::move(y) }
	, z{ std::move(z) }
{}

Vector3::Vector3(Vector2 vec2, float z)
	: Vector3{ std::move(vec2.x), std::move(vec2.y), std::move(z) }
{}

Vector3::Vector3(const Vector2& vec2, float z)
	: Vector3{ vec2.x, vec2.y, std::move(z) }
{}

Vector3::Vector3(const Vector3& vec3)
	: Vector3{ vec3.x, vec3.y, vec3.z }
{}

Vector3 Vector3::operator+(const Vector3& other) const
{
	return Vector3{ this->x + other.x, this->y + other.y, this->z + other.z };
}

void Vector3::operator+=(const Vector3& other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	return Vector3{ this->x - other.x, this->y - other.y, this->z - other.z };
}

void Vector3::operator-=(const Vector3& other)
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
}

Vector3 Vector3::operator*(const float scalar) const
{
	return Vector3{ this->x * scalar, this->y * scalar, this->z * scalar };
}

void Vector3::operator*=(const float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
}

Vector3 Vector3::operator/(const float scalar) const
{
	return Vector3{ this->x / scalar, this->y / scalar, this->z / scalar };
}

void Vector3::operator/=(const float scalar)
{
	this->x /= scalar;
	this->y /= scalar;
	this->z /= scalar;
}

float Vector3::Magnitude() const
{
	return sqrtf(powf(this->x, 2) + powf(this->y, 2) + powf(this->z, 2));
}

float Vector3::Dot(const Vector3& other) const
{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

Vector3 Vector3::Cross(const Vector3& other) const
{
	return Vector3{ this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x };
}

float Vector3::Normalize()
{
	const float magnitude = Magnitude();
	*this /= magnitude;
	return magnitude;
}

Vector3 Vector3::GetNormalized() const
{
	const float magnitude = Magnitude();
	return Vector3{ this->x / magnitude, this->y / magnitude, this->z / magnitude };
}

//----------------------------------------
//				Vector4
//----------------------------------------

Vector4::Vector4()
	: Vector4{ 0.f, 0.f, 0.f, 0.f }
{}

Vector4::Vector4(float x, float y, float z, float w)
	: Vector3{ std::move(x), std::move(y), std::move(z) }
	, w{ std::move(w) }
{}

Vector4::Vector4(const Vector2& vec2, float z, float w)
	: Vector4{ vec2.x, vec2.y, std::move(z), std::move(w) }
{}

Vector4::Vector4(Vector2 vec2, float z, float w)
	: Vector4{ std::move(vec2.x), std::move(vec2.y), std::move(z), std::move(w) }
{}

Vector4::Vector4(const Vector3& vec3, float w)
	: Vector4{ vec3.x, vec3.y, vec3.z, std::move(w) }
{}

Vector4::Vector4(Vector3 vec3, float w)
	: Vector4{ std::move(vec3.x), std::move(vec3.y), std::move(vec3.z), std::move(w) }
{}

Vector4::Vector4(const Vector4& vec4)
	: Vector4{ vec4.x, vec4.y, vec4.z, vec4.w }
{}

Vector4 Vector4::operator+(const Vector4& other) const
{
	return Vector4{ this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w };
}

void Vector4::operator+=(const Vector4& other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	this->w += other.w;
}

Vector4 Vector4::operator-(const Vector4& other) const
{
	return Vector4{ x - other.x, y - other.y, z - other.z, w - other.w };
}

void Vector4::operator-=(const Vector4& other)
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	this->w -= other.w;
}

Vector4 Vector4::operator*(const float scalar) const
{
	return Vector4{ this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar };
}

void Vector4::operator*=(const float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
	this->w *= scalar;
}

Vector4 Vector4::operator/(const float scalar) const
{
	return Vector4{ this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar };
}

void Vector4::operator/=(const float scalar)
{
	this->x /= scalar;
	this->y /= scalar;
	this->z /= scalar;
	this->w /= scalar;
}