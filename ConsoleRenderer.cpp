#include "ConsoleRenderer.h"

namespace Renderer {
//-------------------------------------------
// Constructors
//-------------------------------------------
	ConsoleRenderer::ConsoleRenderer(bool* result, int width, int height, bool depthMode, int maxZ, int minZ, float FOV) : depthMode(depthMode) {
		*result = true;
		int size = width * height;

		this->height = height;
		this->width = width;
		this->maxZ = maxZ;
		this->minZ = minZ;
		this->FOV = FOV;

		colorBuffer = new char[size];
		for (int i = 0; i < size; i++) colorBuffer[i] = colors[0];

		if (depthMode) colors = fullColors;
		else colors = { fullColors[0], fullColors[sizeof(fullColors) - 2] };

		depthBuffer = new char[size];
		for (int i = 0; i < size; i++) depthBuffer[i] = 0;

		handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!GetConsoleScreenBufferInfo(handle, &csbi)) *result = false;

	}
	ConsoleRenderer::~ConsoleRenderer() {
		delete[]  colorBuffer;
		delete[]  depthBuffer;
	}

//-------------------------------------------
// Methods
//-------------------------------------------
	void ConsoleRenderer::Redraw() {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				cout << colorBuffer[y * width + x];
			}
			cout << endl;
		}
	}
	void ConsoleRenderer::Clear() {
		int size = width * height;
		memset(depthBuffer, 0, size);
		memset(colorBuffer, 0, size);

		dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

		FillConsoleOutputCharacter(
			handle,
			fullColors[0],
			dwConSize,
			coordScreen,
			&cCharsWritten
		);

		GetConsoleScreenBufferInfo(handle, &csbi);

		FillConsoleOutputAttribute(
			handle,
			csbi.wAttributes,
			dwConSize,
			coordScreen,
			&cCharsWritten
		);

		SetConsoleCursorPosition(handle, coordScreen);
	}

	void ConsoleRenderer::PrintBuffers() {
		cout << "colorBuffer:\n";
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				cout << (int)colorBuffer[y * width + x];
			}
			cout << endl;
		}
		cout << "\n\ndepthBUffer:\n";
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				cout << (int)depthBuffer[y * width + x];
			}
			cout << endl;
		}
	}

	void ConsoleRenderer::DepthMode(bool activity) {
		depthMode = activity;
		if (activity) colors = fullColors;
		else colors = { fullColors[0], fullColors[sizeof(fullColors) - 2] };
	}

	void ConsoleRenderer::PutPoint(int x, int y, int z) {
		if (x < 0 || x > width - 1  ||
			y < 0 || y > height - 1 ||
			z < minZ || z > maxZ) 
			return;
		int pos = y * width + x;

		char col = colors[colors.length() - 1];

		float intensivity = (float)(maxZ - z) / maxZ * 100;
		float oldIntensivity = depthBuffer[pos];

		//If there is something more near:
		if (oldIntensivity > intensivity) return;

		col = colors[intensivity / 100 * colors.size()];
		depthBuffer[pos] = intensivity;
		colorBuffer[pos] = col;

		std::cout.flush();
		SetConsoleCursorPosition(handle, { (short)x, (short)y });
		cout << col;
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
		
		float maxDist = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) + 0.1f;
		int actualDist;
		int deltaZ = z1 - z0;
		float progress;
		int z;

		while (true) {
			actualDist = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
			progress = std::sqrt(actualDist / maxDist);
			z = deltaZ * progress + z0;

			PutPoint(x0, y0, z);
			if (x0 == x1 && y0 == y1) break;
			if (eps <= xcrit) x0 += dx, eps += two_b;
			if (eps >= a || a < b) y0 += dy, eps -= two_a;
		}
	}
	void ConsoleRenderer::PutSymbol(int x, int y, char symbol, char activity) {
		if (x < 0 || x > width - 1  ||
			y < 0 || y > height - 1 ||
			activity < minZ || activity > maxZ)
			return;

		std::cout.flush();
		SetConsoleCursorPosition(handle, { (short)x, (short)y });
		cout << symbol;

		int pos = y * width + x;
		depthBuffer[pos] = activity;
		colorBuffer[pos] = symbol;
	}

	inline Vector ConsoleRenderer::CalculatePerspective(Vector point) {
		float aspectRatio = width / height;
		float top = std::tan(0.00872664625 * FOV) * minZ;
		float right = top * aspectRatio;

		Vector a;
		a.x = point.x * minZ / right;
		a.y = point.y * minZ / top;
		a.z = point.z;
		return a;
	}
}
