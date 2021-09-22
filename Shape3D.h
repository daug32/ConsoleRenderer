#pragma once
#include "Vector.h"
#include "ConsoleRenderer.h"
#include <cmath>
#include "Shape.h"

namespace Renderer {
	class Shape3D : public Shape
	{
	public:
		bool** connections;		//Contain information about connection of all vertices. Size is Y*X 
		//-------------------------------------------
		// Methods
		//-------------------------------------------
		Shape3D(int vertexCount);
		~Shape3D();
		void Draw(ConsoleRenderer* renderer);
	};

	class Cube : public Shape3D {
	public:
		int size;
		Cube(Vector pos, int size) : Shape3D(8), size(size) {
			center = pos;

			//======================
			float step = size * 0.5;
			baseVertices[0] = { -step, step, -step };
			baseVertices[1] = { -step, step,  step };
			baseVertices[2] = {  step, step,  step };
			baseVertices[3] = {  step, step, -step };

			baseVertices[4] = { -step, -step, -step };
			baseVertices[5] = { -step, -step,  step };
			baseVertices[6] = {  step, -step,  step };
			baseVertices[7] = {  step, -step, -step };

			//======================
			connections[0][1] = connections[0][3] = connections[0][4] = true;
			connections[1][0] = connections[1][2] = connections[1][5] = true;
			connections[2][1] = connections[2][3] = connections[2][6] = true;
			connections[3][0] = connections[3][2] = connections[3][7] = true;

			connections[4][0] = connections[4][5] = connections[4][7] = true;
			connections[5][1] = connections[5][4] = connections[5][6] = true;
			connections[6][2] = connections[6][5] = connections[6][7] = true;
			connections[7][3] = connections[7][4] = connections[7][6] = true;
		}
	};
	
	//Torus is not done yet
	class Torus : public RegularPolygon{
	public:
		RegularPolygon** circles;
		int elementCount;
		Torus(Vector center, int bigCapacity, int bigRadius, int litCapacity, int litRadius) : 
			RegularPolygon(center, bigCapacity, bigRadius)
		{
			circles = new RegularPolygon * [bigCapacity];
			elementCount = bigCapacity;
			for (int i = 0; i < bigCapacity; i++) {
				circles[i] = new RegularPolygon(baseVertices[i], litCapacity, litRadius);
			}
		}
		~Torus() {
			for (int i = 0; i < elementCount; i++) delete circles[i];
			delete[] circles;
		}
		void Draw(ConsoleRenderer* renderer) {
			if (haveToCalc) {
				Calculate();
				for (int i = 0; i < elementCount; i++) {
					circles[i][0].MoveTo(drawableVertices[i]);
					circles[i][0].RotateAround(center, rotation);
				}
			}
			for (int i = 0; i < elementCount; i++)
				circles[i][0].Draw(renderer);
		}
	};
}
