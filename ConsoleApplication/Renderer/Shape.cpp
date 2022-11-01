#include "Shape.h"

namespace Renderer {
	//-------------------------------------------
	// Constructors
	//-------------------------------------------
	Shape::Shape(int vertexCount) {
		this->vertexCount = vertexCount;
		baseVertices = new Vector[vertexCount]();
		drawableVertices = new Vector[vertexCount]();

		for (int i = 0; i < vertexCount; i++) {
			baseVertices[i] = Vector();
			drawableVertices[i] = Vector();
		}

		center = Vector();
		rotation = Vector();
		scale = Vector(1, 1, 1);
		translation = Vector();
		haveToCalc = true;
	}

	Shape::~Shape() {
		delete[] baseVertices;
		delete[] drawableVertices;
	}

	//-------------------------------------------
	// Constructors of derived classes
	//-------------------------------------------
	Rectangle::Rectangle(Vector center, int width, int height, int depth) : Shape(4) {
		Vector vertices[4] = {
			Vector(width / 2, height / 2, depth),
			Vector(width / 2, -height / 2, depth),
			Vector(-width / 2, -height / 2, depth),
			Vector(-width / 2, height / 2, depth)
		};

		for (int i = 0; i < 4; i++) {
			this->baseVertices[i] = vertices[i];
			this->drawableVertices[i] = vertices[i];
		}

		this->center = center;
	}

	Rectangle::Rectangle(Vector center, Vector vertices[4]) : Shape(4) {
		for (int i = 0; i < 4; i++) {
			baseVertices[i] = vertices[i];
			drawableVertices[i] = vertices[i];
		}

		this->center = center;
	}

	RegularPolygon::RegularPolygon(Vector center, int verticesCount, int radius) : Shape(verticesCount) {
		Vector v;
		float angle = radians(360 / verticesCount);

		for (int i = 0; i < verticesCount; i++) {
			v.x = radius * cos(angle * i);
			v.y = radius * sin(angle * i);
			baseVertices[i] = v;
			drawableVertices[i] = v;
		}

		this->center = center;
	}

	Triangle::Triangle(Vector center, Vector vertices[3]) : Shape(3) {
		for (int i = 0; i < 3; i++) {
			this->baseVertices[i] = vertices[i];
			this->drawableVertices[i] = vertices[i];
		}

		this->center = center;
	}

	//-------------------------------------------
	// Position
	//-------------------------------------------
	void Shape::MoveTo(Vector newPos) {
		this->center = newPos;
		this->haveToCalc = true;
	}

	void Shape::Move(Vector deltaPos) {
		this->center += deltaPos;
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

	void Shape::RotateAround(Vector rotation, Vector position) {
		this->rotation += rotation;
		for (int i = 0; i < vertexCount; i++)
			baseVertices[i] = baseVertices[i] + center - position;
		Calculate();
		for (int i = 0; i < vertexCount; i++)
			baseVertices[i] = baseVertices[i] - center + position;
	}

	//-------------------------------------------
	// Other
	//-------------------------------------------
	void Shape::Draw(ConsoleRenderer* renderer) {
		if (haveToCalc) Calculate();

		for (int i = 0; i < vertexCount - 1; i++) {
			renderer->PutLine(drawableVertices[i], drawableVertices[i + 1]);
		}
		renderer->PutLine(drawableVertices[0], drawableVertices[vertexCount - 1]);
	}

	void Shape::Calculate() {
		Vector a;
		Vector b;
		float cos = 0;
		float sin = 0;

		for (int i = 0; i < vertexCount; i++) {
			a = baseVertices[i];

			//-------------------------------------------
			// Scaling
			a.x *= scale.x;
			a.y *= scale.y;
			a.z *= scale.z;

			//-------------------------------------------
			// Rotating
			b = a;
			a.y = b.y * std::cos(rotation.x) - b.z * std::sin(rotation.x);
			a.z = b.y * std::sin(rotation.x) + b.z * std::cos(rotation.x);

			b = a;
			a.x = b.x * std::cos(rotation.y) + b.z * std::sin(rotation.y);
			a.z = -b.x * std::sin(rotation.y) + b.z * std::cos(rotation.y);

			b = a;
			a.x = b.x * std::cos(rotation.z) - b.y * std::sin(rotation.z);
			a.y = b.x * std::sin(rotation.z) + b.y * std::cos(rotation.z);

			//-------------------------------------------
			// Translation
			a += translation;
			a += center;

			//-------------------------------------------
			// Saving
			drawableVertices[i] = a;
		}

		haveToCalc = false;
	}
}
