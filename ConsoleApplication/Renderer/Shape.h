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
		void MoveTo(Vector newPos);
		//Change center position by adding
		void Move(Vector deltaPos);

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
		//Change rotation by adding
		void RotateAround(Vector rotation, Vector position);

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
		RegularPolygon(Vector center, int verticesCount, int radius);
	};
}