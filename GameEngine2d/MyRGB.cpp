#include "MyRGB.hpp"

MyRGB::MyRGB(double r, double g, double b) : r(r), g(g), b(b) {}

MyRGB::MyRGB()
{
    r = 0.0;
    g = 0.0;
    b = 0.0;
}

MyRGB operator-(const MyRGB& first, const MyRGB& second)
{
    return MyRGB(first.r - second.r, first.g - second.g, first.b - second.b);
}

bool operator==(const MyRGB& first, const MyRGB& second)
{
    return (first.r == second.r && first.g == second.g && first.b == second.b);
}

bool operator!=(const MyRGB& first, const MyRGB& second)
{
    return (first.r != second.r || first.g != second.g || first.b != second.b);
}
