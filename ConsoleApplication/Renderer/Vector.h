#pragma once

struct Vector{
public:

	//-------------------------------------------
	// Variables
	//-------------------------------------------
	float x;
	float y;
	float z;

	//-------------------------------------------
	// Constructors
	//-------------------------------------------
	Vector(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector(float x, float y) {
		this->x = x;
		this->y = y;
		this->z = 0;
	}
	Vector(float value) {
		this->x = value;
		this->y = value; 
		this->z = value;
	}
	Vector() {
		x = 0;
		y = 0;
		z = 0;
	}

	//-------------------------------------------
	// Operators
	//-------------------------------------------
	void operator+=(Vector v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
	}
	void operator-=(Vector v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
	}
	void operator*=(float v) {
		this->x *= v;
		this->y *= v;
		this->z *= v;
	}
	void operator/=(float v) {
		this->x /= v;
		this->y /= v;
		this->z /= v;
	}

	Vector operator+(Vector a) {
		return {
			this->x + a.x,
			this->y + a.y,
			this->z + a.z
		};
	}
	Vector operator-(Vector a) {
		return {
			this->x - a.x,
			this->y - a.y,
			this->z - a.z
		};
	}
	Vector operator*(float a) {
		return {
			this->x * a,
			this->y * a,
			this->z * a
		};
	}
	Vector operator/(float a) {
		return {
			this->x / a,
			this->y / a,
			this->z / a
		};
	}

	//-------------------------------------------
	// Other 
	//-------------------------------------------
	static Vector MeanPosition(Vector* arr, int count) {
		Vector result;
		for (int i = 0; i < count; i++) result += arr[i];
		result /= count;
		return result;
	}
	static inline double GetSqLength(Vector v) {
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}
	static inline double GetSqDistance(Vector pos1, Vector pos2) {
		return 
			(pos1.x - pos2.x) * (pos1.x - pos2.x) + 
			(pos1.y - pos2.y) * (pos1.y - pos2.y) + 
			(pos1.z - pos2.z) * (pos1.z - pos2.z);
	}
};