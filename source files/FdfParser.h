#pragma once

#include <iostream>
#include <vector>


struct Point
{
    int x;
    int y;
    int z;
};

std::vector<std::vector<Point>> ParseFdf(const std::string& file);