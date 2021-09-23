#include "ConsoleRenderer.h"

namespace Renderer {
//-------------------------------------------
// Constructors
//-------------------------------------------
	ConsoleRenderer::ConsoleRenderer(bool* result, int width, int height, bool depthMode, float Zfar, float Znear) : depthMode(depthMode) {
		*result = true;
		int size = width * height;

		this->height = height;
		this->width = width;
		this->Zfar = Zfar;
		this->Znear = Znear;
		this->FOV = degrees(std::atan(height / 2 / Znear) * 2);

		this->isOrtographic = true;

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
	void ConsoleRenderer::SetFieldOfView(float FOV) {
		if (FOV < 0) throw 0;
		this->FOV = FOV;
		this->Znear = height / std::tan(radians(FOV) / 2) / 2;
	}
	void ConsoleRenderer::SetZnear(float Znear) {
		if (Znear < 0) throw 0;
		this->Znear = Znear;
		this->FOV = degrees(std::atan(height / 2 / Znear) * 2);
	}

	void ConsoleRenderer::PutPoint(Vector p) {
		if (p.x < 0 || p.x > width - 1  ||
			p.y < 0 || p.y > height - 1 ||
			p.z < Znear || p.z > Zfar)
			return;
		int pos = p.y * width + p.x;

		char col = colors[colors.length() - 1];

		float intensivity = (float)(Zfar - p.z) / Zfar * 100;
		float oldIntensivity = depthBuffer[pos];

		//If there is something more near:
		if (oldIntensivity > intensivity) return;

		col = colors[intensivity / 100 * colors.size()];
		depthBuffer[pos] = intensivity;
		colorBuffer[pos] = col;

		std::cout.flush();
		SetConsoleCursorPosition(handle, { (short)p.x, (short)p.y });
		cout << col;
	}
	void ConsoleRenderer::PutLine(Vector p1, Vector p2) {

		if (!isOrtographic) {
			p1 = PerspectiveProjection3Dto2D(p1);
			p2 = PerspectiveProjection3Dto2D(p2);
		}
		p1.x = (int)p1.x;
		p1.y = (int)p1.y;
		p1.z = (int)p1.z;
		p2.x = (int)p2.x;
		p2.y = (int)p2.y;
		p2.z = (int)p2.z;


		register int dx = 1;
		int a = p2.x - p1.x;
		if (a < 0) dx = -1, a = -a;
		register int dy = 1;
		int b = p2.y - p1.y;
		if (b < 0) dy = -1, b = -b;
		int two_a = 2 * a;
		int two_b = 2 * b;
		int xcrit = -b + two_a;
		register int eps = 0;
		
		float maxDist = (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y) + 0.1f;
		int actualDist;
		int deltaZ = p2.z - p1.z;
		float progress;
		int z;

		while (true) {
			actualDist = (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
			progress = std::sqrt(actualDist / maxDist);
			z = deltaZ * progress + p1.z;

			PutPoint(p1.x, p1.y, z);
			if (p1.x == p2.x && p1.y == p2.y) break;
			if (eps <= xcrit) p1.x += dx, eps += two_b;
			if (eps >= a || a < b) p1.y += dy, eps -= two_a;
		}
	}
	void ConsoleRenderer::PutStr(int x, int y, std::string str) {
		if (x < 0 || x > width - 1  ||
			y < 0 || y > height - 1)
			return;

		SetConsoleCursorPosition(handle, { (short)x, (short)y });
		int pos;
		for (int i = 0; i < str.size(); i++)
			cout << str[i];
	}

	inline Vector ConsoleRenderer::PerspectiveProjection3Dto2D(Vector point3D) {
		if (point3D.z < Znear || point3D.z > Zfar) return point3D;
		//Position of the center of the screen;
		Vector coordCenter = Vector(width / 2, height / 2);
		//Translate origin to the center of the screen;
		point3D -= coordCenter;

		point3D.x = point3D.x * Znear / point3D.z;
		point3D.y = point3D.y * Znear / point3D.z;

		//Translate back
		point3D += coordCenter;

		return point3D;
	}
}
