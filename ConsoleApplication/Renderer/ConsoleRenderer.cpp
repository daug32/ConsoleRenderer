#include "ConsoleRenderer.h"

namespace Renderer
{
	//-------------------------------------------
	// Constructors
	//-------------------------------------------
	ConsoleRenderer::ConsoleRenderer(
		int width,
		int height,
		bool depthMode,
		float zfar,
		float zNar
	) : depthMode(depthMode) {
		int size = width * height;

		this->height = height;
		this->width = width;
		this->zFar = zfar;
		this->Znear = zNar;
		this->FOV = degrees(std::atan(height / 2 / zNar) * 2);

		this->isOrtographic = true;

		colorBuffer = new char[size];
		for (int i = 0; i < size; i++) {
			colorBuffer[i] = colors[0];
		}

		if (depthMode) {
			colors = fullColors;
		}
		else {
			colors = {
				fullColors[0],
				fullColors[sizeof(fullColors) - 2]
			};
		}

		depthBuffer = new char[size];
		for (int i = 0; i < size; i++) {
			depthBuffer[i] = 0;
		}

		handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (!GetConsoleScreenBufferInfo(handle, &csbi)) {
			throw std::exception();
		}
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

	void ConsoleRenderer::SetDepthMode(bool activity) {
		depthMode = activity;
		if (activity) colors = fullColors;
		else colors = { fullColors[0], fullColors[sizeof(fullColors) - 2] };
	}

	void ConsoleRenderer::SetFieldOfView(float fov) {
		if (fov < 0) throw 0;
		this->FOV = fov;
		this->Znear = height / std::tan(radians(fov) / 2) / 2;
	}

	void ConsoleRenderer::SetZNear(float zNear) {
		if (zNear < 0) throw 0;
		this->Znear = zNear;
		this->FOV = degrees(std::atan(height / 2 / zNear) * 2);
	}

	void ConsoleRenderer::PutPoint(const Vector& point) {
		if (point.x < 0 || point.x > width - 1 ||
			point.y < 0 || point.y > height - 1 ||
			point.z < Znear || point.z > zFar)
		{
			return;
		}

		int pos = point.y * width + point.x;

		char col = colors[colors.length() - 1];

		float intensivity = (float)(zFar - point.z) / zFar * 100;
		float oldIntensivity = depthBuffer[pos];

		//If there is something more near:
		if (oldIntensivity > intensivity) {
			return;
		}

		col = colors[intensivity / 100 * colors.size()];
		depthBuffer[pos] = intensivity;
		colorBuffer[pos] = col;

		std::cout.flush();
		SetConsoleCursorPosition(handle, { (short)point.x, (short)point.y });

		cout << col;
	}

	void ConsoleRenderer::PutLine(Vector start, Vector end) {
		if (!isOrtographic) {
			start = PerspectiveProjection3Dto2D(start);
			end = PerspectiveProjection3Dto2D(end);
		}

		start.x = (int)start.x;
		start.y = (int)start.y;
		start.z = (int)start.z;
		end.x = (int)end.x;
		end.y = (int)end.y;
		end.z = (int)end.z;

		register int dx = 1;
		int a = end.x - start.x;
		if (a < 0) dx = -1, a = -a;
		register int dy = 1;
		int b = end.y - start.y;
		if (b < 0) dy = -1, b = -b;
		int two_a = 2 * a;
		int two_b = 2 * b;
		int xcrit = -b + two_a;
		register int eps = 0;

		float maxDist = (end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y) + 0.1f;
		int actualDist;
		int deltaZ = end.z - start.z;
		float progress;
		int z;

		while (true) {
			actualDist = (end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y);
			progress = std::sqrt(actualDist / maxDist);
			z = deltaZ * progress + start.z;

			PutPoint(start.x, start.y, z);
			if (start.x == end.x && start.y == end.y) {
				break;
			}

			if (eps <= xcrit) {
				start.x += dx, eps += two_b;
			}

			if (eps >= a || a < b) {
				start.y += dy, eps -= two_a;
			}
		}
	}

	void ConsoleRenderer::PutStr(int x, int y, const std::string& str) {
		if (x < 0 || x > width - 1 ||
			y < 0 || y > height - 1)
		{
			return;
		}

		SetConsoleCursorPosition(handle, { (short)x, (short)y });

		cout << str;
	}

	inline Vector ConsoleRenderer::PerspectiveProjection3Dto2D(const Vector& point3d) {
		Vector result = Vector(point3d);

		if (point3d.z < Znear || point3d.z > zFar) {
			return result;
		}

		//Position of the center of the screen;
		Vector coordCenter = Vector(width / 2, height / 2);

		//Translate origin to the center of the screen;
		result -= coordCenter;

		result.x = point3d.x * Znear / point3d.z;
		result.y = point3d.y * Znear / point3d.z;

		//Translate back
		result += coordCenter;

		return result;
	}
}
