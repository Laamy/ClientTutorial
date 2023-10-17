#pragma once

struct UIColor {
	// took the union idea from horions math structures (or was it echos idfr nor does it matter to much)
	// union allows for the same memory location to be accessed with different names
	union {
		struct { float r, g, b, a; };
		float arr[4]{};
	};

	// constructor that initializes r, g, b, and a to provided values, and divides them by 255.0
	// a default value of 255 is provided for a if not specified
	UIColor(const float r, const float g, const float b, const float a = 255) {
		this->r = r / 255.0f;
		this->g = g / 255.0f;
		this->b = b / 255.0f;
		this->a = a / 255.0f;
	};
};