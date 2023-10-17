#pragma once

// constant to convert degrees to radians
static constexpr float DEG_RAD = 180.0f / 3.1415927f; // calculates on compile

template <typename T>
struct Vector3 {
	// union allows for the same memory location to be accessed with different names
	union {
		struct {
			T x, y, z;
		};
		T arr[3];
	};

	// constructor that initializes x, y, and z to provided values, or 0 if not provided
	Vector3(T x = 0, T y = 0, T z = 0) {
		this->x = x; this->y = y; this->z = z;
	};

	// overload == operator to allow for comparison between two Vector3 objects
	bool operator == (Vector3 v) { return v.x == x && v.y == y && v.z == z; };

	// overload != operator to allow for comparison between two Vector3 objects
	bool operator != (Vector3 v) { return v.x != x || v.y != y || v.z != z; };

	// overload *= operator to allow for multiplication of two Vector3 objects
	Vector3 operator *= (Vector3 v) { return Vector3(v.x * x, v.y * y, v.z * z); };

	// returns a new Vector3 object that is the difference between this Vector3 and another
	Vector3 submissive(const Vector3& o) const { return Vector3(x - o.x, y - o.y, z - o.z); }

	// returns the squared length of the vector (x^2 + y^2 + z^2)
	T squaredlen() const { return x * x + y * y; }

	// returns the magnitude (length) of the vector
	T magnitude() const { return sqrtf(squaredlen()); }

	// returns the magnitude of the vector ignoring the z component
	T magnitudexy() const { return sqrtf(x * x + y * y); }

	// returns the magnitude of the vector ignoring the y component
	T magnitudexz() const { return sqrtf(x * x + z * z); }

	// returns a new Vector3 object that is the result of linearly interpolating between this Vector3 and another
	Vector3 lerp(const Vector3& pos, T x1, T y1, T z1) {
		Vector3 This;
		This.x = x + x1 * (pos.x - x);
		This.y = y + y1 * (pos.y - y);
		This.z = z + z1 * (pos.z - z);
		return This;
	}

	// returns the distance between this Vector3 and another
	T distance(Vector3 v) {
		T dX = x - v.x;
		T dY = y - v.y;
		T dZ = z - v.z;
		return std::sqrt(dX * dX + dY * dY + dZ * dZ);
	}

	// static method to calculate the angles between two Vector3 objects
	const static Vector2<float> CalcAngle(Vector3 ths, Vector3 dst) {
		Vector3 diff = dst.submissive(ths);

		diff.y = diff.y / diff.magnitude();
		Vector2 angles;
		angles.x = asinf(diff.y) * -DEG_RAD;
		angles.y = (T)-atan2f(diff.x, diff.z) * DEG_RAD;

		return angles;
	}
};