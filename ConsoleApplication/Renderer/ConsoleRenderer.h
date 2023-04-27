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

namespace Renderer
{
    //All possible colors. Will be used if depthMode activated;
    const char fullColors[] = " .,-^;!&%#@";

    const float PI = 3.14159265f;

    //Convert degrees to radians
    inline double radians(float degree)
    {
        return degree * PI / static_cast<float>(180);
    }

    //Convert radians to degrees
    inline double degrees(float radian)
    {
        return radian * static_cast<float>(180) / PI;
    }

    class ConsoleRenderer
    {
    public:
        char* colorBuffer; // There will be saved color for each pixel
        char* depthBuffer; // There will be saved depth (in %) for each pixel: from 0 to 100.

        std::string colors; // Depend on depthMode 
        bool depthMode; // Will render set colors or not. If true, generate second buffer;
        int width; // X size of screen
        int height; // Y size of screen
        float FOV; // The filed of view. In degrees.
        int zFar; // The farest visible position
        int Znear; // The nearest visible position
        bool isOrtographic; // True in default. If false, use perspective projection. 

        /**
         * \param width width is the width of the screen 
         * \param height height is the height of the screen 
         * \param depthMode darkness the distant elements if enabled 
         * \param zFar is the farthest point that can be seen 
         * \param zNar is the nearest point that can be seen
         */
        ConsoleRenderer(int width, int height, bool depthMode = true, float zFar = 500, float zNar = 1);
        ~ConsoleRenderer();

        void Redraw();
        void Clear();

        /**
         * \brief Sets depth mode. If true, brightness of point depend on its position.z. 
         */
        void SetDepthMode(bool activity);

        /**
         * \brief Sets new FieldOfView parameter. Znear will be recalculated
         * \param fov Field Of View, in degrees
         */
        void SetFieldOfView(float fov);

        /**
         * \brief Sets new ZNear value. FOV will be recalculated 
         * \param zNear It is the nearest point that can be seen
         */
        void SetZNear(float zNear);

        /**
         * \brief Sets a painted point at the target position
         * \param point A point that should be painted
         */
        void PutPoint(const Vector& point);

        /**
         * \brief Sets a painted point at the target position
         */
        void PutPoint(int x, int y, int z = 1)
        {
            PutPoint(Vector(x, y, z));
        }

        /**
         * \brief Sets a line that starting from Point1 to Point2
         */
        void PutLine(Vector start, Vector end);

        /**
         * \brief Sets a line that starting from Point1 to Point2 
         */
        void PutLine(
            const int startX,
            const int startY,
            const int startZ,
            const int endX,
            const int endY,
            const int endZ)
        {
            PutLine(Vector(startX, startY, startZ), Vector(endX, endY, endZ));
        }

        /**
         * \brief Sets a string at the target position
         * \param x A horizontal coordinate where string starts
         * \param y A vertical coordinate where string starts
         * \param str A string that will be rendered
         */
        void PutStr(int x, int y, const std::string& str);
        
    protected:
        HANDLE handle; //Handle of this console
        CONSOLE_SCREEN_BUFFER_INFO csbi; //Contain all information about console
        DWORD cCharsWritten; //Writted in console words
        DWORD dwConSize; //Console size = X * Y
        COORD coordScreen = {0, 0}; //The position for the cursor after calling 'Clear()'

        /**
         * \brief Creates a prospective projection of the 3D point into 2D space
         * \param point3d Original point that should be projected
         * \return 2D prospective projection of the given point 
         */
        inline Vector PerspectiveProjection3Dto2D(const Vector& point3d);
    };
}
