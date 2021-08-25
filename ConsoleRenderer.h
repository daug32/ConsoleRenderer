#pragma once
#include "Vector.h"

namespace Renderer {

	static char GetColor(float intensivity) {
		if (intensivity > 80) return '@';
		if (intensivity > 70) return '#';
		if (intensivity > 60) return '%';
		if (intensivity > 50) return '&';
		if (intensivity > 40) return '!';
		if (intensivity > 30) return '^';
		if (intensivity > 20) return '-';
		if (intensivity < 5) return ' ';
		return '.';
	}
	static int GetIntensivity(char col) {
		if (col == '#') return 99;
		if (col == '@') return 79;
		if (col == '%') return 69;
		if (col == '&') return 59;
		if (col == '!') return 49;
		if (col == '^') return 39;
		if (col == '-') return 29;
		if (col == '.') return 19;
		return 0;
	}

	class ConsoleRenderer
	{
	public:
		int width;
		int height;
		float depth;
		char* screen;

		ConsoleRenderer(int width, int height, int depth = 100);

		void Draw();
		void Clear();

		inline void PutPoint(Vector p) { PutPoint(p.x, p.y, p.z); };
		void PutPoint(int x, int y, int z = 1);

		inline void PutLine(Vector p1, Vector p2) { PutLine(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z); };
		void PutLine(int x0, int y0, int z0, int x1, int y1, int z1);
	};
}