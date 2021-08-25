#include "stdafx.h"
namespace R = Renderer;

//-------------------------------------------
//Functions
//-------------------------------------------
int main();
void Draw();

//-------------------------------------------
// Global variables
//-------------------------------------------
static R::ConsoleRenderer* render;
static int height = 46;
static int width = 58;
static int depth = 100;
static Vector center = Vector(width / 2, height / 2, width / 2);

R::RegularPolygon* circles;
int circlesCount = 2;
R::RegularPolygon* triangle;

static Vector rotation = Vector(5, 5, 0);
static Vector direction = Vector(0, 0, 0);


//-------------------------------------------
// Entering point
//-------------------------------------------
int main() {
	render = new R::ConsoleRenderer(width, height, depth);

	int size = 23;
	circles = new R::RegularPolygon[] {
		R::RegularPolygon(center, 16, size),
		R::RegularPolygon(center, 16, size)
	};
	circles[1].RotateY(R::PI / 2);
	circles[1].RotateX(R::PI / 2);
	triangle = new R::RegularPolygon(center, 3, 13);

	Draw();

	delete[] circles;
	delete triangle;
	delete render;
	return 1;
}

void Draw() {
	while (true) {
		render->Clear();

		circles[0].RotateDegree(rotation);
		triangle->RotateZ(-R::PI / 36);
		circles[1].RotateDegree(rotation);

		circles[1].Draw(render);
		triangle->Draw(render);
		circles[0].Draw(render);

		render->Draw();
		Sleep(40);
	}
}