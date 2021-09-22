#include "stdafx.h"
namespace R = Renderer;

//-------------------------------------------
//Functions
//-------------------------------------------
int main();
int Setup();
int Draw();

//-------------------------------------------
// Global variables
//-------------------------------------------
static R::ConsoleRenderer* render;
static int height = 60;
static int width = 100;
static int depth = width;

static Vector center = Vector(width / 2, height / 2, depth / 2);
static int timer = 0;

#define oneDegree (Renderer::PI / 180)
static Vector changer;	//there can be rotation, translation and etc
static POINT actualMousePos;
static POINT lastMousePos;

static R::RegularPolygon *obj;

//-------------------------------------------
// Entering point
//-------------------------------------------
int main() {
	Setup();
	Draw();
	return 1;
}

//-------------------------------------------
// Prepare for rendering
//-------------------------------------------
int Setup() {
	//Init renderer
	bool result;
	render = new R::ConsoleRenderer(&result, width, height, true);
	if (!result) {
		cout << endl << "render wasn't created.";
		return 0;
	}

	//============ Init all drawable objects ===============
	//
	obj = new R::RegularPolygon(Vector(10, 10, depth / 2), 4, 10);
	//
	//=======================================================

	changer = oneDegree * 10;
	changer.z = 0;
	//Get cursor position
	GetCursorPos(&actualMousePos);
	lastMousePos = actualMousePos;

	return 1;
}

//-------------------------------------------
// In general... This is rednering
//-------------------------------------------
int Draw() {
	while (true) {
		render->Clear();
		
		GetCursorPos(&actualMousePos);
		changer.x = -(actualMousePos.y - lastMousePos.y) * 0.01f; 
		changer.y = (actualMousePos.x - lastMousePos.x) * 0.01f;
		lastMousePos.x = actualMousePos.x;
		lastMousePos.y = actualMousePos.y;

		obj->Rotate(changer);
		obj->Draw(render);

		render->PutSymbol(obj->center.x, obj->center.y, '1', 1);
		render->PutLine(Vector(width - 1, 0, 10), Vector(width - 1, height - 1, 10));
		render->PutLine(Vector(0, height - 1, 10), Vector(width - 1, height - 1, 10));

		cout << endl;
		cout << "x: " << obj->center.x << "; y: " << obj->center.y << "; z: " << obj->center.z;


		Sleep(70);
		timer++;
	}
	return 1;
}