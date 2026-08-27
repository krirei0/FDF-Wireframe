#include <stdio.h>
#include <cmath>
#include <SDL2/SDL.h>
#include "FdfParser.h"
#include "Projection.h"



std::vector<std::vector<ScrCoordinates>> IsometricProjection(const std::vector<std::vector<RotatedPoint>>& roatatedPoints)
{
    std::vector<std::vector<ScrCoordinates>> scrPoints{};

    for (int i = 0; i < roatatedPoints.size(); i++)
    {
        std::vector<ScrCoordinates> scrp{};

        for (int j = 0; j < roatatedPoints[i].size(); j++) 
        {
            double pi = 3.14159265358979323846;
            double angle = pi / 6.0;

            ScrCoordinates pc;
            double x = roatatedPoints[i][j].x;
            double y = roatatedPoints[i][j].y;
            double z = roatatedPoints[i][j].z;

            float zHeight = 0.1; 

            pc.screenX = (x - y) * std::cos(angle);
            pc.screenY = (x + y) * std::sin(angle) - z * zHeight;
            pc.z = z;

            scrp.push_back(pc);
        }
        
        scrPoints.push_back(scrp);
    }
    return scrPoints;
}

Boundaries FindBoundaries(const std::vector<std::vector<ScrCoordinates>>& projectionPoints)

{
    Boundaries boundaryCoordinates;
    boundaryCoordinates.minX = projectionPoints[0][0].screenX;
    boundaryCoordinates.maxX = projectionPoints[0][0].screenX;
    boundaryCoordinates.minY = projectionPoints[0][0].screenY;
    boundaryCoordinates.maxY = projectionPoints[0][0].screenY;
    boundaryCoordinates.minZ = projectionPoints[0][0].z;
    boundaryCoordinates.maxZ = projectionPoints[0][0].z;

    for (int i = 0; i < projectionPoints.size(); i++)
    {
        for (int j = 0; j < projectionPoints[i].size(); j++)
        {

            if (boundaryCoordinates.minX > projectionPoints[i][j].screenX)
            {
                boundaryCoordinates.minX = projectionPoints[i][j].screenX;
            }

            if (boundaryCoordinates.maxX < projectionPoints[i][j].screenX)
            {
                boundaryCoordinates.maxX = projectionPoints[i][j].screenX;
            }


            if (boundaryCoordinates.minY > projectionPoints[i][j].screenY)
            {
                boundaryCoordinates.minY = projectionPoints[i][j].screenY;
            }

            if (boundaryCoordinates.maxY < projectionPoints[i][j].screenY)
            {
                boundaryCoordinates.maxY = projectionPoints[i][j].screenY;
            }


            if (boundaryCoordinates.minZ > projectionPoints[i][j].z)
            {
                boundaryCoordinates.minZ = projectionPoints[i][j].z;
            }

            if (boundaryCoordinates.maxZ < projectionPoints[i][j].z)
            {
                boundaryCoordinates.maxZ = projectionPoints[i][j].z;
            }
        }
    } 
    return boundaryCoordinates;
}

Center FindCenter(const Boundaries& boundarycoordinates)
{
    Center projectionCenter;
    projectionCenter.x = boundarycoordinates.minX + ((boundarycoordinates.maxX - boundarycoordinates.minX) / 2);
    projectionCenter.y = boundarycoordinates.minY + ((boundarycoordinates.maxY - boundarycoordinates.minY) / 2);

    return projectionCenter;
}

Translation CalculateTranslation(const Center& projectionCenter, double windowWidth, double windowHeight)
{
    Translation translationCoordinates;
    translationCoordinates.x = (windowWidth / 2) - projectionCenter.x;
    translationCoordinates.y = (windowHeight / 2) - projectionCenter.y;

    return translationCoordinates;
};

std::vector<std::vector<ScrCoordinates>> ApplyTranslation(std::vector<std::vector<ScrCoordinates>>& projectionPoints, Translation translationCoordinates)
{
    for (int i = 0; i < projectionPoints.size(); i++)
    {
        for (int j = 0; j < projectionPoints[i].size(); j++)
        {
            projectionPoints[i][j].screenX = projectionPoints[i][j].screenX + translationCoordinates.x;
            projectionPoints[i][j].screenY = projectionPoints[i][j].screenY + translationCoordinates.y;
        
        }   
    } 
    return projectionPoints; 
};

double FindScaleFactor(const Boundaries& boundarycoordinates, double windowWidth, double windowHeight)
{
    double modelWidth = boundarycoordinates.maxX - boundarycoordinates.minX;
    double modelHeight = boundarycoordinates.maxY - boundarycoordinates.minY;

    double targetWidth = 0.8 * windowWidth;
    double targetHeight = 0.8 * windowHeight;

    double scaleFactor1 = targetWidth / modelWidth;
    double scaleFactor2 = targetHeight / modelHeight;

    if (scaleFactor1 > scaleFactor2)
    {
        return scaleFactor2;
    }
    else
    {
        return scaleFactor1;
    }
};

std::vector<std::vector<ScrCoordinates>> ApplyScale(std::vector<std::vector<ScrCoordinates>>& projectionPoints, double scaleFactor)
{
    for (int i = 0; i < projectionPoints.size(); i++)
    {
        for (int j = 0; j < projectionPoints[i].size(); j++)
        {
            projectionPoints[i][j].screenX = projectionPoints[i][j].screenX * scaleFactor;
            projectionPoints[i][j].screenY = projectionPoints[i][j].screenY * scaleFactor;
        
        }   
    } 
    return projectionPoints; 

};

int DrawConnection(std::vector<std::vector<ScrCoordinates>>& translatedPoints, SDL_Renderer *renderer, const Boundaries& boundaries)
{
    for (int i = 0; i < translatedPoints.size(); i++)
    {  
        for (int j = 0; j < translatedPoints[i].size(); j++)
        {
            if (j+1 < translatedPoints[i].size())
            {
                int x1 = (int)translatedPoints[i][j].screenX;
                int y1 = (int)translatedPoints[i][j].screenY;
                int x2 = (int)translatedPoints[i][j+1].screenX;
                int y2 = (int)translatedPoints[i][j+1].screenY;

                SDL_Color color = GetColor(translatedPoints[i][j], boundaries);
    
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                DrawGradientLine(renderer,
                 translatedPoints[i][j],
                 translatedPoints[i][j + 1],
                 boundaries);
            }

            if (i+1 < translatedPoints.size() && j < translatedPoints[i+1].size())
            {
                int x1H = (int)translatedPoints[i][j].screenX;
                int y1H = (int)translatedPoints[i][j].screenY;
                int x2H = (int)translatedPoints[i+1][j].screenX;
                int y2H = (int)translatedPoints[i+1][j].screenY;

                SDL_Color color = GetColor(translatedPoints[i][j], boundaries);
    
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                DrawGradientLine(renderer, translatedPoints[i][j], translatedPoints[i + 1][j],boundaries);
            }
            
        }
    } 
    return 0;
};

int DrawZ(std::vector<std::vector<ScrCoordinates>>& translatedPoints,  SDL_Renderer *renderer, const Boundaries& boundaries)
{
    for (int i = 0; i < translatedPoints.size(); i++)
    {  
        for (int j = 0; j < translatedPoints[i].size(); j++)
        {
            SDL_Color color = GetColor(translatedPoints[i][j], boundaries);
    
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

            int x = translatedPoints[i][j].screenX;
            int y = translatedPoints[i][j].screenY;

            SDL_RenderDrawPoint(renderer, x,y);
        }
    }
    return 0;
};

void DrawGradientLine(SDL_Renderer* renderer,
                      const ScrCoordinates& point1,
                      const ScrCoordinates& point2,
                      const Boundaries& boundaries)
{
    double dx = point2.screenX - point1.screenX;
    double dy = point2.screenY - point1.screenY;

    double distance = std::sqrt(dx * dx + dy * dy);

    for (int i = 0; i < distance; i++)
    {
        double t = i / distance;

        double x = point1.screenX + dx * t;
        double y = point1.screenY + dy * t;

        double z = point1.z + (point2.z - point1.z) * t;

        SDL_Color color = GetGradientColor(z, boundaries);

        SDL_SetRenderDrawColor(
            renderer,
            color.r,
            color.g,
            color.b,
            color.a
        );

        SDL_RenderDrawPoint(renderer, (int)x, (int)y);
    }
}

/*
Color
*/
SDL_Color GetGradientColor(double z, const Boundaries& boundaries)
{
    double t = (z - boundaries.minZ) /
               (boundaries.maxZ - boundaries.minZ);

    SDL_Color color;

    if (t < 0.5)
    {
        // White -> Yellow
        double localT = t / 0.5;

        color.r = 255;
        color.g = 255;
        color.b = 255 * (1.0 - localT);
        color.a = 255;
    }
    else
    {
        // Yellow -> Pink -> Purple
        double localT = (t - 0.5) / 0.5;

        color.r = 255 - (98 * localT);
        color.g = 255 - (177 * localT);
        color.b = 10 + (211 * localT);
        color.a = 255;
    }

    return color;
}


SDL_Color GetColor(ScrCoordinates pc, const Boundaries& boundaries)
{
    SDL_Color color = {0,0,0,0};
   
    if (pc.z == boundaries.minZ)
    {
        color = {255, 255, 255, 255};
    }

    if (pc.z > boundaries.minZ && pc.z < boundaries.maxZ)
    {
        color = {255, 77, 141, 255};
    }

    if (pc.z == boundaries.maxZ)
    {
        color = {157, 78, 221, 255};
    }

    return color;
};


/*
Rotation
*/
std::vector<std::vector<RotatedPoint>> RotatePoint(const std::vector<std::vector<Point>>& worldPoints, double angleX, double angleY, double angleZ)
{
    double pi = 3.14159265358979323846;
    double radiansX = angleX * pi / 180.0;
    double radiansY = angleY * pi / 180.0;
    double radiansZ = angleZ * pi / 180.0;

    std::vector<std::vector<RotatedPoint>> rotPoints{};

    for (int i = 0; i < worldPoints.size(); i++)
    {
        std::vector<RotatedPoint> rotp{};

        for (int j = 0; j < worldPoints[i].size(); j++) 
        {

            RotatedPoint rotatedPoint;
            double x = worldPoints[i][j].x;
            double y = worldPoints[i][j].y;
            double z = worldPoints[i][j].z;

            rotatedPoint.x = x;
            rotatedPoint.y = y * std::cos(radiansX) - z * std::sin(radiansX);
            rotatedPoint.z = y * std::sin(radiansX) + z * std::cos(radiansX);

            double x1 = rotatedPoint.x;
            double y1 = rotatedPoint.y;
            double z1 =rotatedPoint.z;

            rotatedPoint.x = x1 * std::cos(radiansY) + z1 * std::sin(radiansY);
            rotatedPoint.y = y1;
            rotatedPoint.z = -x1 * std::sin(radiansY) + z1 * std::cos(radiansY);

            double x2 = rotatedPoint.x;
            double y2 = rotatedPoint.y;
            double z2 =rotatedPoint.z;

            rotatedPoint.x = x2 * std::cos(radiansZ) - y2 * std::sin(radiansZ);
            rotatedPoint.y = x2 * std::sin(radiansZ) + y2 * std::cos(radiansZ);
            rotatedPoint.z = z2;


            rotp.push_back(rotatedPoint);
        }
        
        rotPoints.push_back(rotp);
    }
    return rotPoints;
};


std::vector<std::vector<RotatedPoint>> RotatePointX(const std::vector<std::vector<Point>>& worldPoints, double angle)
{
    double pi = 3.14159265358979323846;
    double radians = angle * pi / 180.0;

    std::vector<std::vector<RotatedPoint>> rotPoints{};

    for (int i = 0; i < worldPoints.size(); i++)
    {
        std::vector<RotatedPoint> rotp{};

        for (int j = 0; j < worldPoints[i].size(); j++) 
        {

            RotatedPoint rotatedPoint;
            double x = worldPoints[i][j].x;
            double y = worldPoints[i][j].y;
            double z = worldPoints[i][j].z;

            rotatedPoint.x = x;
            rotatedPoint.y = y * std::cos(radians) - z * std::sin(radians);
            rotatedPoint.z = y * std::sin(radians) + z * std::cos(radians);

            rotp.push_back(rotatedPoint);
        }
        
        rotPoints.push_back(rotp);
    }
    return rotPoints;
};

std::vector<std::vector<RotatedPoint>> RotatePointY(const std::vector<std::vector<Point>>& worldPoints, double angle)
{
    double pi = 3.14159265358979323846;
    double radians = angle * pi / 180.0;

    std::vector<std::vector<RotatedPoint>> rotPoints{};

    for (int i = 0; i < worldPoints.size(); i++)
    {
        std::vector<RotatedPoint> rotp{};

        for (int j = 0; j < worldPoints[i].size(); j++) 
        {

            RotatedPoint rotatedPoint;
            double x = worldPoints[i][j].x;
            double y = worldPoints[i][j].y;
            double z = worldPoints[i][j].z;

            rotatedPoint.x = x * std::cos(radians) + z * std::sin(radians);
            rotatedPoint.y = y;
            rotatedPoint.z = -x * std::sin(radians) + z * std::cos(radians);

            rotp.push_back(rotatedPoint);
        }
        
        rotPoints.push_back(rotp);
    }
    return rotPoints;
};

std::vector<std::vector<RotatedPoint>> RotatePointZ(const std::vector<std::vector<Point>>& worldPoints, double angle)
{
    double pi = 3.14159265358979323846;
    double radians = angle * pi / 180.0;

    std::vector<std::vector<RotatedPoint>> rotPoints{};

    for (int i = 0; i < worldPoints.size(); i++)
    {
        std::vector<RotatedPoint> rotp{};

        for (int j = 0; j < worldPoints[i].size(); j++) 
        {

            RotatedPoint rotatedPoint;
            double x = worldPoints[i][j].x;
            double y = worldPoints[i][j].y;
            double z = worldPoints[i][j].z;


            rotatedPoint.x = x * std::cos(radians) - y * std::sin(radians);
            rotatedPoint.y = x * std::sin(radians) + y * std::cos(radians);
            rotatedPoint.z = z;

            rotp.push_back(rotatedPoint);
        }
        
        rotPoints.push_back(rotp);
    }
    return rotPoints;
};
