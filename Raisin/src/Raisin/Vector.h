#pragma once

struct Vector2D
{
	Vector2D() = default;
	Vector2D(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
	Vector2D(const Vector2D& _v)
	{
		x = _v.x;
		y = _v.y;
	}
	// Sobrecargar *, -, *, /
	Vector2D operator -(Vector2D _a)
	{
		return Vector2D(_a.x - x, _a.y - y);
	}
	Vector2D operator -=(Vector2D _a)
	{
		x -= _a.x;
		y -= _a.y;
		return *this;
	}

	Vector2D operator *(Vector2D _a)
	{
		return Vector2D(_a.x * x, _a.y * y);
	}
	Vector2D operator *=(Vector2D _a)
	{
		x *= _a.x;
		y *= _a.y;
		return *this;
	}

	Vector2D operator +(Vector2D _a)
	{
		return Vector2D(_a.x + x, _a.y + y);
	}
	Vector2D operator +=(Vector2D _a)
	{
		x += _a.x;
		y += _a.y;
		return *this;
	}

	Vector2D operator /(Vector2D _a)
	{
		return Vector2D(_a.x / x, _a.y / y);
	}
	Vector2D operator /=(Vector2D _a)
	{
		x /= _a.x;
		y /= _a.y;
		return *this;
	}

	float x, y;
};

struct Vector3D
{
	Vector3D() = default;
	Vector3D(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	Vector3D(const Vector3D& _v)
	{
		x = _v.x;
		y = _v.y;
		z = _v.z;
	}
	Vector3D(const Vector2D& _v)
	{
		x = _v.x;
		y = _v.y;
		z = 0.f;
	}

	float x, y, z;
};

struct Vector4D
{
	Vector4D() = default;
	Vector4D(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
	Vector4D(const Vector4D& _v)
	{
		x = _v.x;
		y = _v.y;
		z = _v.z;
		w = _v.w;
	}
	// Promocion
	Vector4D(const Vector3D& _v)
	{
		x = _v.x;
		y = _v.y;
		z = _v.z;
		w = 1.f;
	}
	float x, y, z, w;
};