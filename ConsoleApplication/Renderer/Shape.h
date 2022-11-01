#pragma once
#include "ConsoleRenderer.h"
#include "Vector.h"
#include <cmath>

namespace Renderer {

	class Shape
	{
	protected:
		Vector* drawableVertices;				//Vertices with changed position.
		bool haveToCalc;						//When true, drawableVertices are recoordinates.

	public:
		//-------------------------------------------
		// Variables
		//-------------------------------------------
		Vector center;							//Corrdinates for origin in local metric.
		Vector* baseVertices;					//Base vertices in local coordinates. They aren't changing.
		int vertexCount;						//Quentity of vertices
		Vector rotation;						//Vector that contains rotation (in radians) for each axis
		Vector scale;							//Using for increasing the size in each direction.
		Vector translation;						//To rotate around...

		//-------------------------------------------
		// Methods
		//-------------------------------------------
		Shape(int vertexCount);
		~Shape();

		virtual	void Draw(ConsoleRenderer* renderer);

		//Set the center position on the choosed coords
		void MoveTo(const Vector& newPos);
		//Change center position by adding
		void Move(const Vector& deltaPos);

		void SetScale(const Vector& scale);

		//Rotation in radians
		void Rotate(const Vector& deltaRotation);
		//Rotation in radians
		void RotateX(float deltaAngles);
		//Rotation in radians
		void RotateY(float deltaAngles);
		//Rotation in radians
		void RotateZ(float deltaAngles);
		//Rotation in radians
		void SetRotation(const Vector& rotation);
		//Change rotation by adding
		void RotateAround(const Vector& rotation, const Vector& position);

		//Rotation in degrees
		void RotateDegree(const Vector& deltaRotation);
		//Rotation in degrees
		void SetRotationDegree(const Vector& rotation);

	protected:
		void Calculate();
	};

	class Rectangle : public Shape {
	public:
		//Make rectangle around the center position
		Rectangle(const Vector& center, int width, int height, int depth = 10);
		Rectangle(const Vector& center, const Vector vertices[4]);
	};

	class Triangle : public Shape {
	public:
		//Vertices in local coordinates
		Triangle(const Vector& center, const Vector vertices[3]);
	}; 
	

	class RegularPolygon : public Shape {
	public: 
		RegularPolygon(const Vector& center, int verticesCount, int radius);
	};
}