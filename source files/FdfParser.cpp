#include <fstream>
#include <iostream>
#include <sstream> 
#include <string>
#include <vector>
#include "FdfParser.h"



std::vector<std::vector<Point>> ParseFdf(const std::string& file)
{
    std::vector<std::vector<Point>> outerVector{};
    std::ifstream f(file);

    if (!f.is_open())
    {
        std::cerr <<"Error: Unable to open the file." << std::endl;
        return outerVector;
    }

    int num;
    int rowIndex = 0;
    std::string line;

    while (std::getline(f, line))
    {
        std::vector<Point> points {};
        std::stringstream ss(line);
        int colIndex  = 0;
        
        while (ss >> num)
        {
            Point p;
            p.x = colIndex;
            colIndex += 1;
            p.y = rowIndex;
            p.z = num;
            points.push_back(p);
        }
        
        rowIndex += 1;
        outerVector.push_back(points);
    }

    return outerVector;

}
