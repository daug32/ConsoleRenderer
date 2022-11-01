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
	Rectangle::Rectangle(const Vector& center, int width, int height, int depth) : Shape(4) {
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

	Rectangle::Rectangle(const Vector& center, const Vector vertices[4]) : Shape(4) {
		for (int i = 0; i < 4; i++) {
			baseVertices[i] = vertices[i];
			drawableVertices[i] = vertices[i];
		}

		this->center = center;
	}

	RegularPolygon::RegularPolygon(const Vector& center, int verticesCount, int radius) : Shape(verticesCount) {
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

	Triangle::Triangle(const Vector& center, const Vector vertices[3]) : Shape(3) {
		for (int i = 0; i < 3; i++) {
			this->baseVertices[i] = vertices[i];
			this->drawableVertices[i] = vertices[i];
		}

		this->center = center;
	}

	//-------------------------------------------
	// Position
	//-------------------------------------------
	void Shape::MoveTo(const Vector& newPos) {
		this->center = newPos;
		this->haveToCalc = true;
	}

	void Shape::Move(const Vector& deltaPos) {
		this->center += deltaPos;
		this->haveToCalc = true;
	}

	//-------------------------------------------
	// Scaling
	//-------------------------------------------
	void Shape::SetScale(const Vector& scale) {
		this->scale = scale;
		this->haveToCalc = true;
	}

	//-------------------------------------------
	// Rotation
	//-------------------------------------------
	void Shape::SetRotation(const Vector& rotation) {
		this->rotation = rotation;
		this->haveToCalc = true;
	}

	void Shape::Rotate(const Vector& deltaRotation) {
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

	void Shape::RotateDegree(const Vector& deltaRotation) {
		this->rotation.x += radians(deltaRotation.x);
		this->rotation.y += radians(deltaRotation.y);
		this->rotation.z += radians(deltaRotation.z);
		this->haveToCalc = true;
	}

	void Shape::SetRotationDegree(const Vector& rotation) {
		this->rotation.x = radians(rotation.x);
		this->rotation.y = radians(rotation.y);
		this->rotation.z = radians(rotation.z);
		this->haveToCalc = true;
	}

	void Shape::RotateAround(const Vector& rotation, const Vector& position) {
		this->rotation += rotation;

		for (int i = 0; i < vertexCount; i++) {
			baseVertices[i] = baseVertices[i] + center - position;
		}

		Calculate();

		for (int i = 0; i < vertexCount; i++) {
			baseVertices[i] = baseVertices[i] - center + position;
		}
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
		register float cos = 0;
		register float sin = 0;

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
			cos = std::cos(rotation.x);
			sin = std::sin(rotation.x);
			a.y = b.y * cos - b.z * sin;
			a.z = b.y * sin + b.z * cos;

			b = a;
			cos = std::cos(rotation.y);
			sin = std::sin(rotation.y);
			a.x = b.x * cos + b.z * sin;
			a.z = -b.x * sin + b.z * cos;

			b = a;
			cos = std::cos(rotation.z);
			sin = std::sin(rotation.z);
			a.x = b.x * cos - b.y * sin;
			a.y = b.x * sin + b.y * cos;

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
