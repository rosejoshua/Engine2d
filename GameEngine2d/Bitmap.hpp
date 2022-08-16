#pragma once

#include <string>
#include <cstdint>
#include <memory>

#include "MyRGB.hpp"

using namespace std;

class Bitmap {
private:
    int32_t m_width{ 0 };
    int32_t m_widthPaddingBytes{ 0 };
    int32_t m_height{ 0 };
    unique_ptr<uint8_t[]> m_pPixels{ nullptr };
    void applyColorToGrayscalePixel(uint8_t* pAddrToColorChannel, MyRGB* colorToApply);
public:
    Bitmap(int width, int height);
    Bitmap(string filename, string directory);
    Bitmap(string filename);
    int getWidth();
    int getHeight();
    void scale(int numerator, int denominator);
    void applyColorToGrayscale(MyRGB* colorToApply);
    bool write(string filename);
    bool writeToFolderWindowsOS(string filename, string directoryName);
    void setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue);
    MyRGB getPixel(int x, int y);
    virtual ~Bitmap();
};

