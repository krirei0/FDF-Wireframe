#pragma once
#include <vector>


struct ScrCoordinates
{
    double screenX = 0;
    double screenY = 0;
    double z = 0;
};

struct RotatedPoint
{
    double x = 0;
    double y = 0;
    double z = 0;
};

struct Boundaries
{
    double minX;
    double maxX;
    double minY;
    double maxY;
    double minZ;
    double maxZ;
};

struct Center
{
    double x;
    double y;
};

struct Translation
{
    double x;
    double y;
};

std::vector<std::vector<ScrCoordinates>> IsometricProjection(const std::vector<std::vector<RotatedPoint>>& roatatedPoints);

Boundaries FindBoundaries(const std::vector<std::vector<ScrCoordinates>>& projectionPoints);

Center FindCenter(const Boundaries& boundarycoordinates);

Translation CalculateTranslation(const Center& projectionCenter, double windowWidth, double windowHeight);

std::vector<std::vector<ScrCoordinates>> ApplyTranslation(std::vector<std::vector<ScrCoordinates>>& projectionPoints, Translation translationCoordinates);

double FindScaleFactor(const Boundaries& boundarycoordinates, double windowWidth, double windowHeight);

std::vector<std::vector<ScrCoordinates>> ApplyScale(std::vector<std::vector<ScrCoordinates>>& projectionPoints, double scaleFactor);

int DrawConnection(std::vector<std::vector<ScrCoordinates>>& translatedPoints,  SDL_Renderer *renderer, const Boundaries& boundaries);

int DrawZ(std::vector<std::vector<ScrCoordinates>>& translatedPoints,  SDL_Renderer *renderer, const Boundaries& boundaries);

void DrawGradientLine(SDL_Renderer* renderer, const ScrCoordinates& point1, const ScrCoordinates& point2, const Boundaries& boundaries);


/*
Color
*/
SDL_Color GetGradientColor(double z, const Boundaries& boundaries);
SDL_Color GetColor(ScrCoordinates pc, const Boundaries& boundaries);

/*
Rotation
*/
std::vector<std::vector<RotatedPoint>> RotatePoint(const std::vector<std::vector<Point>>& worldPoints, double angleX, double angleY, double angleZ);

std::vector<std::vector<RotatedPoint>> RotatePointX(const std::vector<std::vector<Point>>& worldPoints, double angle);
std::vector<std::vector<RotatedPoint>> RotatePointY(const std::vector<std::vector<Point>>& worldPoints, double angle);
std::vector<std::vector<RotatedPoint>> RotatePointZ(const std::vector<std::vector<Point>>& worldPoints, double angle);