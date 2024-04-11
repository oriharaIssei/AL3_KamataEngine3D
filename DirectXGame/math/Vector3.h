#pragma once

#include <cmath>

struct Vector3 {
	float x, y, z;

	Vector3(float x, float y, float z) : x(x), y(y), z(z){};
	Vector3() : x(0), y(0), z(0){};

	///
	///	演算子
	///
	Vector3& operator=(const Vector3& another) {
		this->x = another.x;
		this->y = another.y;
		this->z = another.z;
		return *this;
	}

	Vector3 operator+(const Vector3& another) const { return Vector3(this->x + another.x, this->y + another.y, this->z + another.z); }
	Vector3& operator+=(const Vector3& another) {
		this->x += another.x;
		this->y += another.y;
		this->z += another.z;
		return *this;
	}

	Vector3 operator-(const Vector3& another) { return Vector3(this->x - another.x, this->y - another.y, this->z - another.z); }
	Vector3 operator-(const Vector3& another) const { return Vector3(this->x - another.x, this->y - another.y, this->z - another.z); }
	Vector3& operator-=(const Vector3& another) {
		this->x -= another.x;
		this->y -= another.y;
		this->z -= another.z;
		return *this;
	}

	Vector3 operator*(const float& scalar) const { return Vector3(this->x * scalar, this->y * scalar, this->z * scalar); }
	Vector3& operator*=(const Vector3& another) {
		this->x *= another.x;
		this->y *= another.y;
		this->z *= another.z;
		return *this;
	}

	float dot(const Vector3& another) const { return this->x * another.x + this->y * another.y + this->z * another.z; }
	float length() const{ return std::sqrtf(dot(*this)); }

	Vector3 Normalize() {
		float len = this->length();
		if (len == 0.0f) {
			return *this;
		}
		return Vector3(this->x / len, this->y / len, this->z / len);
	}

	 // 等価比較演算子
	bool operator==(const Vector3& other) const {
		// 浮動小数点数の比較には、ほぼ等しいかを判断するためにある程度の許容誤差（epsilon）を設けることが一般的です。
		// ここでは単純な比較を行いますが、実際には適切な許容誤差を考慮することが重要です。
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}

	// 不等価比較演算子
	bool operator!=(const Vector3& other) const {
		// `==`演算子の結果を利用して定義します。
		return !(*this == other);
	}
};