#pragma once
class MyRGB
{
public:
    double r;
    double g;
    double b;
    MyRGB();
    MyRGB(double r, double g, double b);
};

MyRGB operator-(const MyRGB& first, const MyRGB& second);
bool operator==(const MyRGB& first, const MyRGB& second);
bool operator!=(const MyRGB& first, const MyRGB& second);

