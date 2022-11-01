#include "Shape3D.h"

namespace Renderer 
{
	Shape3D::Shape3D(int vertexCount) : Shape(vertexCount) {
		connections = new bool* [vertexCount];
		for (int i = 0; i < vertexCount; i++) {
			connections[i] = new bool[vertexCount];
			for (int j = 0; j < vertexCount; j++) connections[i][j] = 0;
		}
	}
	Shape3D::~Shape3D() {
		for (int i = 0; i < vertexCount; i++)
			delete[] connections[i];
		delete[] connections;
	}

	//-------------------------------------------
	// Other
	//-------------------------------------------
	void Shape3D::Draw(ConsoleRenderer* renderer) {
		if (haveToCalc) Calculate();

		for (int y = 0; y < vertexCount; y++) 
		{
			for (int x = y + 1; x < vertexCount; x++)
				if (connections[y][x])
					renderer->PutLine(drawableVertices[y], drawableVertices[x]);
		}
	}
}