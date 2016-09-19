#ifndef DICOMIMAGE_H
#define DICOMIMAGE_H

#include <string>
#include <vector>

class dicomImage
{
public:
    dicomImage();

    dicomImage(const std::string& filename);

    ~dicomImage();

    void setFileName(const std::string& filename);

    unsigned char* pixel();

    unsigned short* shortPixel();

    int pixelLength() const;

    int imageHeight() const;

    int imageWidth() const;

    const std::vector<int> imagePixelCount() const;

    const std::vector<int> imageShortPixelCount() const;

private:

    void readImage(void);

    void countPixels();

    unsigned char* convertTo8Bit(unsigned char* _pData, unsigned long nNumPixels,
                                 bool bIsSigned, short nHighBit,
                                 float fRescaleSlope, float fRescaleIntercept,
                                 float fWindowCenter, float fWindowWidth);

    std::string _fileName;
    unsigned char* _pData;
    unsigned char* _pDataOld;
    unsigned short* _pShortData;
    std::vector<int> pixelCount;
    std::vector<int> shortPixelCount;

};

#endif // DICOMIMAGE_H
