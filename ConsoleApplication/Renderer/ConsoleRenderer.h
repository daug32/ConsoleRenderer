#pragma once
#include <windows.h>
#include "Vector.h"
#include <iostream>
using std::cout;
using std::endl;
#include <cmath>
using std::sqrt;
using std::abs;
#include <string>

namespace Renderer {
	//All possible colors. Will be used if depthMode activated;
	const char fullColors[] = " .,-^;!&%#@";	

	const float PI = 3.14159265f;
	//Convert degrees to radians
	inline double radians(float degree) {
		return degree * PI / (float)180;
	}
	//Convert radians to degrees
	inline double degrees(float radian) {
		return radian * (float)180 / PI;
	}

	class ConsoleRenderer
	{
	protected:

	//-------------------------------------------
	// Variables of class
	//-------------------------------------------
		HANDLE handle;						//Handle of this console
		CONSOLE_SCREEN_BUFFER_INFO csbi;	//Contain all information about console
		DWORD cCharsWritten;				//Writted in console words
		DWORD dwConSize;					//Console size = X * Y
		COORD coordScreen = { 0, 0 };		//The position for the cursor after calling 'Clear()'

	public:
		//Buffers
		char* colorBuffer;					//There will be saved color for each pixel
		char* depthBuffer;					//There will be saved depth (in %) for each pixel: from 0 to 100.

		std::string colors;		//Depend on depthMode 
		bool depthMode;			//Will render set colors or not. If true, generate second buffer;
		int width;				//X size of screen
		int height;				//Y size of screen
		float FOV;				//The filed of view. In degrees.
		int Zfar;				//The farest visible position
		int Znear;				//The nearest visible position
		bool isOrtographic;		//True in default. If false, use perspective projection. 

	//-------------------------------------------
	// Constructors & destructors
	//-------------------------------------------

		/*
		(result == false) means that render wasn't created right
		width is the width of the screen
		height is the height of the screen
		Znear is the nearest point you can see
		FOV is a "field of view", in radians
		*/
		ConsoleRenderer(bool* result, int width, int height, bool depthMode = true, float Zfar = 500, float Znear = 1);
		~ConsoleRenderer();

	//-------------------------------------------
	// Methods of class
	//-------------------------------------------
	
		//Redraw everything. In old versions was a function "Draw".
		void Redraw();
		//Reset all buffers.
		void Clear();
		//Print on screen buffers. Used for debugging.
		void PrintBuffers();

		//While true, brightness of point depend on its position.z.
		void DepthMode(bool activity);
		/*FOV = filed of view, in degrees
		Znear will be recalculated*/
		void SetFieldOfView(float FOV);
		/*Znear is the nearest point you can see,
		FOV will be recalculated (in degrees)*/
		void SetZnear(float Znear);

		//Set the data to all buffers and draw it on screen.
		void PutPoint(Vector p);
		//Set the data to all buffers and draw it on screen.
		inline void PutPoint(int x, int y, int z = 1) {
			PutPoint(Vector(x, y, z)); 
		}
		//Draw line using the PutPoint functions.
		void PutLine(Vector p1, Vector p2);
		//Draw line using the PutPoint functions.
		void PutLine(int x0, int y0, int z0, int x1, int y1, int z1) { 
			PutLine(Vector(x0, y0, z0), Vector(x1, y1, z1)); 
		}
		//Put string on the screen
		void PutStr(int x, int y, std::string str);

		inline Vector PerspectiveProjection3Dto2D(Vector Point3D);
	};
}