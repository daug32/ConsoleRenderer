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

static Vector center;
static int timer = 0;

#define oneDegree (Renderer::PI / 180)
static Vector changer;	//there can be rotation, translation and etc
static POINT mousePos;
static POINT lastMousePos;

static R::Shape3D *obj;

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
	try {
		render = new R::ConsoleRenderer(width, height, true, 500);
	}
	catch (std::exception ex) {
		cout << endl << "render wasn't created.";
		return 0;
	}

	render->isOrtographic = false;
	render->SetZnear(70);
	center = Vector(width / 2, height / 2, render->Znear + 100);

	obj = new R::Cube(center, 60);

	changer = center;
	changer.x = 90;

	GetCursorPos(&mousePos);

	return 1;
}

//-------------------------------------------
// This is rednering. It is unexpected for function with this kind of name, right? :D
//-------------------------------------------
int Draw() {
	while (true) {
		render->Clear();
		
		GetCursorPos(&mousePos);
		changer.x = (float)mousePos.x / 1920 * width;
		lastMousePos = mousePos;

		obj->MoveTo(Vector(changer.x, obj->center.y, obj->center.z));
		obj->RotateX(oneDegree * 4);
		obj->RotateY(oneDegree * 4);
		obj->Draw(render);

		string str =  "center.x: " + to_string(obj->center.x) +
					"\ncenter.y: " + to_string(obj->center.y) +
					"\ncenter.z: " + to_string(obj->center.z) + 
					"\nZnear: " + to_string(render->Znear) + 
					"\nFOV: " + to_string(render->Znear);
		render->PutStr(0, height - 1, str.c_str());

		Sleep(19);
		timer++;
	}

	return 1;
}