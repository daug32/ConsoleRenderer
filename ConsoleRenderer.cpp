#include "ConsoleRenderer.h"
#include <iostream>
using std::cout;
using std::endl;
#include <cmath>
using std::sqrt;
using std::abs;

namespace Renderer {

	ConsoleRenderer::ConsoleRenderer(int width, int height, int depth) {
		this->height = height;
		this->width = width;
		this->depth = depth;
		screen = new char[width * height];
		for (int i = 0; i < height * width; i++) screen[i] = ' ';
	}
	void ConsoleRenderer::Draw() {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++)
				cout << screen[y * width + x];
			cout << endl;
		}
	}
	void ConsoleRenderer::PutPoint(int x, int y, int z) {
		if (x < 0 || x > width - 1 ||
			y < 0 || y > height - 1) return;
		float intensivity = (float)(depth - z) / depth * 100;
		float oldIntensivity = GetIntensivity(screen[y * width + x]);
		if (oldIntensivity > intensivity) return;
		char col = GetColor(intensivity);
		screen[y * width + x] = col;
	}
	void ConsoleRenderer::PutLine(int x0, int y0, int z0, int x1, int y1, int z1) {
		register int dx = 1;
		int a = x1 - x0;
		if (a < 0) dx = -1, a = -a;
		register int dy = 1;
		int b = y1 - y0;
		if (b < 0) dy = -1, b = -b;
		int two_a = 2 * a;
		int two_b = 2 * b;
		int xcrit = -b + two_a;
		register int eps = 0;

		Vector startPos(x0, y0, z0);
		float maxDist = Vector::GetSqDistance(
			Vector(x0, y0),
			Vector(x1, y1)
		);
		float dist = 0;
		float deltaZ = z1 - z0;
		float z = 0;

		while (true) {
			dist = Vector::GetSqDistance(
				Vector(x0, y0),
				startPos
			);
			z = deltaZ * sqrt(dist / maxDist) + startPos.z;

			PutPoint(x0, y0, z);
			if (x0 == x1 && y0 == y1) break;
			if (eps <= xcrit) x0 += dx, eps += two_b;
			if (eps >= a || a < b) y0 += dy, eps -= two_a;
		}
	}
	void ConsoleRenderer::Clear() {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++)
				screen[y * width + x] = ' ';
		}
		system("CLS");
	}
}
