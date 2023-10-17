#pragma once

template <typename T>
struct Vector4 {
	// union allows for the same memory location to be accessed with different names
	union {
		struct {
			T w, x, y, z;
		};
		T arr[4];
	};

	// constructor that initializes w, x, y, and z to provided values, or 0 if not provided
	Vector4(T w = 0, T x = 0, T y = 0, T z = 0) {
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	};

	// constructor that initializes the vector based on a position and size (w and y are position, x and z are size)
	Vector4(Vector2<float> position, Vector2<float> size) {
		this->w = position.x;
		this->x = position.x + size.x;
		this->y = position.y;
		this->z = position.y + size.y;
	};

	// overload == operator to allow for comparison between two Vector4 objects
	bool operator == (Vector4 v) { return v.w == w && v.x == x && v.y == y && v.z == z; };

	// overload != operator to allow for comparison between two Vector4 objects
	bool operator != (Vector4 v) { return v.w != w || v.x != x || v.y != y || v.z != z; };
};