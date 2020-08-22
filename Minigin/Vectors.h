#pragma once

struct Vector2
{
	Vector2();
	//move constructor
	Vector2(float x, float y);
	//copy constructor
	Vector2(const Vector2& vec2);
	virtual ~Vector2() = default;

	//vector2 addition
	Vector2 operator+(const Vector2& other) const;
	void operator+=(const Vector2& other);
	//vector2 subtraction
	Vector2 operator-(const Vector2& other) const;
	void operator-=(const Vector2& other);
	//vector2 scalar multiplication
	Vector2 operator*(const float scalar) const;
	void operator*=(const float scalar);
	//vector2 scalar division
	Vector2 operator/(const float scalar) const;
	void operator/=(const float scalar);

	virtual float Magnitude() const;
	float Dot(const Vector2& other) const;
	float Cross(const Vector2& other) const;
	//gives an angle in degrees oriented to the world's right vector
	float OrientOnWorld();
	virtual float Normalize();
	Vector2 GetNormalized() const;

	float x;
	float y;
};

struct Vector3 : public Vector2
{
	Vector3();
	//move constructor
	Vector3(float x, float y, float z = 0.f);
	//move constructor
	Vector3(Vector2 vec2, float z = 0.f);
	//copy constructor
	Vector3(const Vector2& vec2, float z = 0.f);
	//copy constructor
	Vector3(const Vector3& vec3);
	virtual ~Vector3() = default;

	//vector3 addition
	Vector3 operator+(const Vector3& other) const;
	void operator+=(const Vector3& other);
	//vector3 subtraction
	Vector3 operator-(const Vector3& other) const;
	void operator-=(const Vector3& other);
	//vector3 scalar multiplication
	Vector3 operator*(const float scalar) const;
	void operator*=(const float scalar);
	//vector3 scalar division
	Vector3 operator/(const float scalar) const;
	void operator/=(const float scalar);

	virtual float Magnitude() const override;
	float Dot(const Vector3& other) const;
	Vector3 Cross(const Vector3& other) const;
	virtual float Normalize() override;
	Vector3 GetNormalized() const;

	float z;
};

struct Vector4 final : public Vector3
{
	Vector4();
	//copy constructor
	Vector4(float x, float y, float z, float w = 1.f);
	//copy constructor
	Vector4(const Vector2& vec2, float z, float w = 1.f);
	//move constructor
	Vector4(Vector2 vec2, float z, float w = 1.f);
	//copy constructor
	Vector4(const Vector3& vec3, float w = 1.f);
	//move constructor
	Vector4(Vector3 vec3, float w = 1.f);
	//copy constructor
	Vector4(const Vector4& vec4);
	virtual ~Vector4() = default;

	//vector3 addition
	Vector4 operator+(const Vector4& other) const;
	void operator+=(const Vector4& other);
	//vector3 subtraction
	Vector4 operator-(const Vector4& other) const;
	void operator-=(const Vector4& other);
	//vector3 scalar multiplication
	Vector4 operator*(const float scalar) const;
	void operator*=(const float scalar);
	//vector3 scalar division
	Vector4 operator/(const float scalar) const;
	void operator/=(const float scalar);

	float w;
};