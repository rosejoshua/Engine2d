#include <fstream>
#include <iostream>
#include <windows.h>
#include "Bitmap.hpp"
#include "BitmapInfoHeader.hpp"
#include "BitmapFileHeader.hpp"

using namespace std;

Bitmap::Bitmap(int width, int height) : m_width(width), m_height(height)
{
    m_widthPaddingBytes = (width * 3) % 4 == 0 ? 0 : 4 - ((width * 3) % 4);
    m_pPixels = unique_ptr<uint8_t[]>(new uint8_t[width * height * 3]{});
}

Bitmap::~Bitmap() {}

bool Bitmap::write(string filename)
{
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;

    fileHeader.fileSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + ((m_width * 3 + m_widthPaddingBytes) * m_height);
    fileHeader.dataOffset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

    infoHeader.width = m_width;
    infoHeader.height = m_height;

    ofstream file;
    file.open(filename, ios::out | ios::binary);
    if (!file)
    {
        return false;
    }

    file.write((char*)&fileHeader, sizeof(fileHeader));
    file.write((char*)&infoHeader, sizeof(infoHeader));


    char* tempArray = nullptr;

    //create a temp buffer of bytes containing zeroes for inFile.write() to use for padding at the end of rows
    if (m_widthPaddingBytes > 0)
        tempArray = new char[m_widthPaddingBytes] {0};

    char* pChar = (char*)m_pPixels.get();
    for (int y = 0; y < m_height; ++y)
    {
        file.write(pChar, m_width * 3);
        pChar += m_width * 3;

        if (m_widthPaddingBytes > 0)
        {
            file.write(tempArray, m_widthPaddingBytes);
        }
    }

    if (m_widthPaddingBytes > 0)
        delete[] tempArray;

    //    pChar = (char*)m_pPixels.get();
    //    for (int y = 0; y < m_height; ++y)
    //    {
    //        for (int x = 0; x < m_width*3; ++x)
    //        {
    //            cout << (int)*pChar << " ";
    //        }
    //    }
    //    cout << endl;

    file.close();
    if (!file)
    {
        return false;
    }

    return true;
}

void Bitmap::setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t* pPixels = m_pPixels.get();
    pPixels += (y * 3) * m_width + (x * 3);
    pPixels[0] = blue;
    pPixels[1] = green;
    pPixels[2] = red;
}

MyRGB Bitmap::getPixel(int x, int y)
{
    uint8_t* pPixels = m_pPixels.get();
    pPixels += (y * 3) * m_width + (x * 3);
    return MyRGB(pPixels[2], pPixels[1], pPixels[0]);
}

Bitmap::Bitmap(string filename) : Bitmap(filename, ".") {}

int Bitmap::getWidth()
{
    return m_width;
}

int Bitmap::getHeight()
{
    return m_height;
}

Bitmap::Bitmap(string filename, string directory)
{
    ifstream inFile;
    inFile.open(directory + "\\" + filename, ios::in | ios::binary);
    if (!inFile)
    {
        cerr << "failure attempting to open inFile \"" << filename << "\" " <<
            "in (\'.\' means default directory) directory: \"" << directory << "\"" << endl;
        return;
    }

    //while(!inFile.eof())
    //    cout << inFile.get() << " ";
    
    //int i = 300;
    //while (!inFile.eof() && i > 0)
    //{
    //    cout << inFile.get() << " ";
    //    i--;
    //}
    //
    //cout << endl;
    //inFile.clear();
    //inFile.seekg(0);

    if (inFile.get() != 66 || inFile.get() != 77)
        cerr << "attempting to open non-bitmap file, exiting process" << endl;
    else
    {
        //reversing little endian order
        int32_t fileSize = int32_t(
            (unsigned char)(inFile.get()) |
            (unsigned char)(inFile.get()) << 8 |
            (unsigned char)(inFile.get()) << 16 |
            (unsigned char)(inFile.get()) << 24
        );
        //        cout << "filesize: " << fileSize << endl;

        inFile.ignore(4);

        int32_t offsetToBitmapBits = int32_t(
            (unsigned char)(inFile.get()) |
            (unsigned char)(inFile.get()) << 8 |
            (unsigned char)(inFile.get()) << 16 |
            (unsigned char)(inFile.get()) << 24
        );
        //        cout << "offset: " << offsetToBitmapBits << endl;

        inFile.ignore(4);

        m_width = int32_t(
            (unsigned char)(inFile.get()) |
            (unsigned char)(inFile.get()) << 8 |
            (unsigned char)(inFile.get()) << 16 |
            (unsigned char)(inFile.get()) << 24
        );

        //        cout << "width: " << m_width << endl;

        m_widthPaddingBytes = (m_width * 3) % 4 == 0 ? 0 : 4 - ((m_width * 3) % 4);

        //        cout << "width padding in bytes: " << m_widthPaddingBytes << endl;

        m_height = int32_t(
            (unsigned char)(inFile.get()) |
            (unsigned char)(inFile.get()) << 8 |
            (unsigned char)(inFile.get()) << 16 |
            (unsigned char)(inFile.get()) << 24
        );

        //        cout << "height: " << m_height << endl;

        inFile.ignore(28);
        //        cout << "size of bitmap pixels array with padding: " << (m_width * 3 + m_widthPaddingBytes) * m_height << endl;

        m_pPixels = unique_ptr<uint8_t[]>(new uint8_t[m_width * m_height * 3]{});
        int byteCounter = 0;

        char* pNewBitmapArray = (char*)m_pPixels.get();
        for (int y = 0; y < m_height; ++y)
        {
            for (int x = 0; x < m_width * 3; ++x)
            {
                *pNewBitmapArray = inFile.get();
                pNewBitmapArray++;
                byteCounter++;
            }
            if (m_widthPaddingBytes > 0)
            {
                for (int i = 0; i < m_widthPaddingBytes; ++i)
                {
                    inFile.ignore();
                    byteCounter++;
                }
            }
        }
        //        cout << "bytes processed: " << byteCounter << endl;
    }
    inFile.close();
}

bool Bitmap::writeToFolderWindowsOS(string filename, string directoryName)
{
    CreateDirectory(directoryName.c_str(), NULL);
    bool success = write(directoryName + "\\" + filename);
    if (!success)
        cerr << "failure attempting to create directory \"" << directoryName << "\"" << endl;
    return success;
}

void Bitmap::applyColorToGrayscalePixel(uint8_t* pAddrToColorChannel, MyRGB* colorToApply)
{
    bool inError = false;
    uint8_t* temp = pAddrToColorChannel;
    uint8_t colorVal = *temp;
    temp++;

    if (colorVal != *temp)
    {
        inError = true;
    }
    else
    {
        temp++;
        if (colorVal != *temp)
            inError = true;
    }

    if (inError)
    {
        cerr << "attempting to add color to non grayscale image or bitmap bit depth is not 24 bits per pixel" << endl;
        return;
    }


    int grayKey = *pAddrToColorChannel;
    //    cout << grayKey << endl;
    if (grayKey == 255)
        return;
    if (grayKey == 0)
        return;

    if (grayKey < 0)
        grayKey = 0;
    else if (grayKey > 255)
        grayKey = 255;

    if (grayKey == 128)
    {
        *pAddrToColorChannel = colorToApply->b;
        pAddrToColorChannel++;
        *pAddrToColorChannel = colorToApply->g;
        pAddrToColorChannel++;
        *pAddrToColorChannel = colorToApply->r;
    }
    else if (grayKey < 128)
    {
        double multiplier = grayKey / 127.5;
        *pAddrToColorChannel = colorToApply->b * multiplier;
        pAddrToColorChannel++;
        *pAddrToColorChannel = colorToApply->g * multiplier;
        pAddrToColorChannel++;
        *pAddrToColorChannel = colorToApply->r * multiplier;
    }
    else
    {
        double multiplier = grayKey / 127.5 - 1;
        *pAddrToColorChannel = colorToApply->b + multiplier * (255 - colorToApply->b);
        pAddrToColorChannel++;
        *pAddrToColorChannel = colorToApply->g + multiplier * (255 - colorToApply->g);
        pAddrToColorChannel++;
        *pAddrToColorChannel = colorToApply->r + multiplier * (255 - colorToApply->r);
    }
}

void Bitmap::applyColorToGrayscale(MyRGB* colorToApply)
{
    uint8_t* pPixels = m_pPixels.get();
    for (int i = 0; i < m_width * m_height; ++i)
    {
        applyColorToGrayscalePixel(pPixels, colorToApply);
        pPixels += 3;
    }
}

void Bitmap::scale(int numerator, int denominator)
{

}