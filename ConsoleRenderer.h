#pragma once
#include <windows.h>
#include "Vector.h"
#include <iostream>
using std::cout;
using std::endl;
#include <cmath>
using std::sqrt;
using std::abs;

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
		float FOV;
		int maxZ;				//The farest visible position
		int minZ;				//The nearest visible position

	//-------------------------------------------
	// Constructors & destructors
	//-------------------------------------------

		/*if result = false, something renderer wasn't created right, 
		width is the width of the screen
		height is the height of the
		maxZ is the farest visible position
		minZ is the neares visible position
		FOV is "field of view", in degree
		*/
		ConsoleRenderer(bool* result, int width, int height, bool depthMode = true, int maxZ = 100, int minZ = 0, float FOV = 90);
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

		//Set the data to all buffers and draw it on screen.
		inline void PutPoint(Vector p) { PutPoint(p.x, p.y, p.z); };
		//Set the data to all buffers and draw it on screen.
		void PutPoint(int x, int y, int z = 1);
		//Draw line using the PutPoint functions.
		inline void PutLine(Vector p1, Vector p2) { PutLine(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z); };
		//Draw line using the PutPoint functions.
		void PutLine(int x0, int y0, int z0, int x1, int y1, int z1);
		//Put symbol on screen. Activity in fuct is Z-coordinate. 
		void PutSymbol(int x, int y, char symbol, char activity);

		inline Vector CalculatePerspective(Vector point);
	};
}