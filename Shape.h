#pragma once
#include "ConsoleRenderer.h"
#include <vector>
#include "Vector.h"
#include <cmath>

namespace Renderer {
	const float PI = 3.14159265f;

	//Convert degrees to radians
	inline double radians(float degree) {
		return degree * PI / (float)180;
	}
	//Convert radians to degrees
	inline double degrees(float radian) {
		return radian * (float)180 / PI;
	}

	class Shape
	{
	public:

		//-------------------------------------------
		// Variables
		//-------------------------------------------
		Vector center;							//Corrdinates for origin in local metric.
		std::vector<Vector> baseVertices;		//Base vertices in local coordinates. They aren't changing.
		Vector rotation;						//
		Vector scale;							//Using for increasing the size in each direction.

		std::vector<Vector> drawableVertices;	//Vertices with changed position.
		bool haveToCalc;						//When true, drawableVertices are recoordinates.

		//-------------------------------------------
		// Methods
		//-------------------------------------------
		Shape();
		void Draw(ConsoleRenderer* renderer);
		//Set the center position on the choosed coords
		void Move(Vector newPos);
		void SetScale(Vector scale);
		//Rotation in radians
		void Rotate(Vector deltaRotation);
		//Rotation in radians
		void RotateX(float deltaAngles);
		//Rotation in radians
		void RotateY(float deltaAngles);
		//Rotation in radians
		void RotateZ(float deltaAngles);
		//Rotation in radians
		void SetRotation(Vector rotation);
		//Rotation in degrees
		void RotateDegree(Vector deltaRotation);
		//Rotation in degrees
		void SetRotationDegree(Vector rotation);

	protected:
		void Calculate();
	};

	class Rectangle : public Shape {
	public:
		//Make rectangle around the center position
		Rectangle(Vector center, int width, int height, int depth = 10);
		Rectangle(Vector center, Vector vertices[4]);
	};

	class Triangle : public Shape {
	public:
		//Vertices in local coordinates
		Triangle(Vector center, Vector vertices[3]);
	};

	class RegularPolygon : public Shape {
	public: 
		RegularPolygon(Vector center, int verticesCount, int size);
	};
}