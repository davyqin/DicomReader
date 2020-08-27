#pragma once

#include <memory>
#include <vector>

class Image
{
public:
  enum Orientation {
    TRAN,
    CORO,
    SAGI
  };

  enum DataType {
    SHORTBIT = 0,
    CHARBIT
  };

  Image(std::shared_ptr<unsigned short> pixelData,
        std::shared_ptr<unsigned char> pixelData8bit,
        const int pixelLength);

  ~Image();

  void setPosition(const std::vector<double>& pos);

  void setOrientation(const std::vector<double>& ori);

  void setSize(const int width, const int height);

  void setPixelSpacing(const std::vector<double>& value);

  std::vector<double> pixelSpacing() const;

  int height() const;

  int width() const;

  int pixelLength() const;

  std::shared_ptr<const unsigned short> rawPixelData() const;

  std::shared_ptr<const unsigned char> rawPixelData8bit() const;
  
  std::shared_ptr<unsigned short> pixelData() const;

  std::shared_ptr<unsigned char> pixelData8bit() const;

  Orientation orientation() const;

  std::vector<double> position() const;

  unsigned short maxValue() const;

  unsigned short minValue() const;

  unsigned short pixelValue(unsigned int index) const;

  void updateWL(int window, int level);

  int sampleStep() const;

  void setSampleStep(const int step);

  DataType dataType() const;

  void setDataType(const DataType dataType);

private:
  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};