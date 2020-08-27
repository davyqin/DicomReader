#pragma once

#include <memory>

class Image;

class DicomUtil
{
public:

    virtual ~DicomUtil() {}

    virtual std::shared_ptr<unsigned short> pixel() = 0;

    virtual std::shared_ptr<Image> fetchImage() const = 0;

    virtual int pixelLength() const = 0;

    virtual int imageHeight() const = 0;

    virtual int imageWidth() const = 0;

    virtual bool hasPixelData() const = 0;
};