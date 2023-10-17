#pragma once

template <typename T>
struct Vector2 {
	// union allows for the same memory location to be accessed with different names
	union {
		struct {
			T x, y;
		};
		T arr[2];
	};

	// constructor that initializes x and y to provided values, or 0 if not provided
	Vector2(T x = 0, T y = 0) {
		this->x = x; this->y = y;
	};
};