#include "Shape.h"

namespace Renderer {
	//-------------------------------------------
	// Constructors
	//-------------------------------------------
	Shape::Shape() {
		this->baseVertices = std::vector<Vector>();
		this->drawableVertices = std::vector<Vector>();
		this->center = Vector();
		this->rotation = Vector();
		this->scale = Vector(1, 1, 1);
		this->haveToCalc = true;
	}
	Triangle::Triangle(Vector center, Vector vertices[3])
	{
		for (int i = 0; i < 3; i++) {
			this->baseVertices.push_back(vertices[i]);
			this->drawableVertices.push_back(vertices[i]);
		}
		this->center = center;
	}
	Rectangle::Rectangle(Vector center, int width, int height, int depth) {
		Vector vertices[4] = {
			Vector(width / 2, height / 2, depth),
			Vector(width / 2, -height / 2, depth),
			Vector(-width / 2, -height / 2, depth),
			Vector(-width / 2, height / 2, depth)
		}; 
		for (int i = 0; i < 4; i++) {
			this->baseVertices.push_back(vertices[i]);
			this->drawableVertices.push_back(vertices[i]);
		}
		this->center = center;
	}
	Rectangle::Rectangle(Vector center, Vector vertices[4]) {
		for (int i = 0; i < 4; i++) {
			baseVertices.push_back(vertices[i]);
			drawableVertices.push_back(vertices[i]);
		}
		this->center = center;
	}
	RegularPolygon::RegularPolygon(Vector center, int verticesCount, int radius) {
		Vector v;
		float angle = radians(360 / verticesCount);
		for (int i = 0; i < verticesCount; i++) {
			v.x = radius * cos(angle * i);
			v.y = radius * sin(angle * i);
			baseVertices.push_back(v);
			drawableVertices.push_back(v);
		}
		this->center = center;
	}

	//-------------------------------------------
	// Position
	//-------------------------------------------
	void Shape::Move(Vector newPos) {
		this->center = newPos;
		this->haveToCalc = true;
	}
	//-------------------------------------------
	// Scaling
	//-------------------------------------------
	void Shape::SetScale(Vector scale) {
		this->scale = scale;
		this->haveToCalc = true;
	}

	//-------------------------------------------
	// Rotation
	//-------------------------------------------
	void Shape::SetRotation(Vector rotation) {
		this->rotation = rotation;
		this->haveToCalc = true;
	}
	void Shape::Rotate(Vector deltaRotation) {
		this->rotation += deltaRotation;
		this->haveToCalc = true;
	}
	void Shape::RotateX(float deltaAngles) {
		this->rotation.x += deltaAngles;
		this->haveToCalc = true;
	}
	void Shape::RotateY(float deltaAngles) {
		this->rotation.y += deltaAngles;
		this->haveToCalc = true;
	}
	void Shape::RotateZ(float deltaAngles) {
		this->rotation.z += deltaAngles;
		this->haveToCalc = true;
	}
	void Shape::RotateDegree(Vector deltaRotation) {
		this->rotation.x += radians(deltaRotation.x);
		this->rotation.y += radians(deltaRotation.y);
		this->rotation.z += radians(deltaRotation.z);
		this->haveToCalc = true;
	}
	void Shape::SetRotationDegree(Vector rotation) {
		this->rotation.x = radians(rotation.x);
		this->rotation.y = radians(rotation.y);
		this->rotation.z = radians(rotation.z);
		this->haveToCalc = true;
	}

	//-------------------------------------------
	// Other
	//-------------------------------------------
	void Shape::Draw(ConsoleRenderer* renderer) {
		if (haveToCalc) Calculate();
		for (int i = 0; i < drawableVertices.size() - 1; i++)
			renderer->PutLine(drawableVertices[i], drawableVertices[i + 1]);
		renderer->PutLine(drawableVertices[0], drawableVertices[drawableVertices.size() - 1]);
	}
	void Shape::Calculate() {
		Vector a;
		Vector b;
		float cos = 0;
		float sin = 0;

		for (int i = 0; i < baseVertices.size(); i++) {
			a = baseVertices[i];

			//-------------------------------------------
			// Scaling
			a.x *= scale.x;
			a.y *= scale.y;
			a.z *= scale.z;

			//-------------------------------------------
			// Rotating
			b = a;
			cos = std::cos(rotation.x);
			sin = std::sin(rotation.x);
			a.x = b.x;
			a.y = b.y * cos + b.z * (-sin);
			a.z = b.y * sin + b.z * cos;

			b = a;
			cos = std::cos(rotation.y);
			sin = std::sin(rotation.y);
			a.x = b.x * cos + b.z * sin;
			a.y = b.y;
			a.z = b.x * (-sin) + b.z * cos;

			b = a;
			cos = std::cos(rotation.z);
			sin = std::sin(rotation.z);
			a.x = b.x * cos + b.y * (-sin);
			a.y = b.x * sin + b.y * cos;
			a.z = b.z;

			//-------------------------------------------
			// Transforming
			a += center;

			//-------------------------------------------
			// Saving
			drawableVertices[i] = a;
		}
		haveToCalc = false;
	}
}
