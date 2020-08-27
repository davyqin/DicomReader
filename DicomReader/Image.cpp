#include "Image.h"

#include "math.h"
#include <limits>

#include <boost/assign/list_of.hpp>

using namespace std;

namespace {
bool doublesAreEqual(double d1, double d2) {
  return std::fabs(d1 - d2) < std::numeric_limits<double>::epsilon();
}

const std::vector<double> transOri = boost::assign::list_of(1.0)(0.0)(0.0)(0.0)(1.0)(0.0);
const std::vector<double> coroOri = boost::assign::list_of(1.0)(0.0)(0.0)(0.0)(0.0)(1.0);
const std::vector<double> sagiOri = boost::assign::list_of(0.0)(1.0)(0.0)(0.0)(0.0)(1.0);
}

class Image::Pimpl
{
public:
  Pimpl(std::shared_ptr<unsigned short> pixelData,
        std::shared_ptr<unsigned char> pixelData8bit,
  	    const int pixelLength)
  : pixelData(pixelData)
  , pixelData8bit(pixelData8bit)
  , pixelLength(pixelLength)
  , window(65535)
  , level(32768)
  , sampleStep(4)
  , dataType(Image::SHORTBIT)
  , xPos(std::shared_ptr<float>())
  , yPos(std::shared_ptr<float>())
  {
//    computerMinAndMax();
  }

  /* data */
  std::shared_ptr<unsigned short> pixelData;
  std::shared_ptr<unsigned char> pixelData8bit;
  const unsigned long pixelLength;
  std::vector<double> position;
  Image::Orientation orientation;
  int width;
  int height;
  unsigned short minValue;
  unsigned short maxValue;
  int window;
  int level;
  int sampleStep;
  std::vector<double> pixelSpacing;
  std::shared_ptr<unsigned short> outputPixel;
  std::shared_ptr<unsigned char> outputPixel8bit;
  Image::DataType dataType;
  std::shared_ptr<float> xPos;
  std::shared_ptr<float> yPos;

  

#if 0
  void computerMinAndMax() {
    minValue = std::numeric_limits<unsigned short>::max();
    maxValue = std::numeric_limits<unsigned short>::min();
    unsigned short* p = pixelData.get();

    for (unsigned long i = 0; i < pixelLength; ++i, ++p)
    {
      if (*p < minValue) minValue = *p;
      if (*p > maxValue) maxValue = *p;
    }
  }
#endif
};

Image::Image(shared_ptr<unsigned short> pixelData,
             shared_ptr<unsigned char> pixelData8bit,
	         const int pixelLength)
:_pimpl(new Pimpl(pixelData, pixelData8bit, pixelLength)) {}

Image::~Image() {}

void Image::setPosition(const std::vector<double>& pos) {
  _pimpl->position = pos;
}

std::vector<double> Image::position() const {
  return _pimpl->position;
}

void Image::setSize(const int width, const int height) {
  _pimpl->width = width;
  _pimpl->height = height;
}

int Image::height() const {
  return _pimpl->height;
}

int Image::width() const {
  return _pimpl->width;
}

int Image::pixelLength() const {
  return _pimpl->pixelLength;
}

shared_ptr<const unsigned short> Image::rawPixelData() const {
  return _pimpl->pixelData;
}

shared_ptr<const unsigned char> Image::rawPixelData8bit() const {
  return _pimpl->pixelData8bit;
}

shared_ptr<unsigned short> Image::pixelData() const {
  if (!_pimpl->outputPixel) {
    int nCount = _pimpl->pixelLength;
    _pimpl->outputPixel.reset(new unsigned short[_pimpl->pixelLength + 16]);
    unsigned short* np = _pimpl->outputPixel.get();
    unsigned short* pp = _pimpl->pixelData.get();

    const double dSlope = 65535.0 / (double)_pimpl->window;
    const double dShift = (double)_pimpl->level - (double)_pimpl->window / 2.0;

    while (nCount-- > 0) {
      int value = ((int)*pp - dShift) * dSlope;
      if (value <= 0) {
        value = 0;
      }
      else if (value >= 65535) {
        value = 65535;
      }
      
      *np = static_cast<unsigned short>(value);
      ++np; ++pp;      
    }
  }

  return _pimpl->outputPixel;
}

shared_ptr<unsigned char> Image::pixelData8bit() const {
  if (!_pimpl->outputPixel8bit) {
    int nCount = _pimpl->pixelLength;
    _pimpl->outputPixel8bit.reset(new unsigned char[_pimpl->pixelLength + 16]);
    unsigned char* np = _pimpl->outputPixel8bit.get();
    const unsigned char* pp = _pimpl->pixelData8bit.get();
    
    const double halfWindow = (double)_pimpl->window / 2.0;
    while (nCount-- > 0)
    {
      if (*pp < ((double)_pimpl->level - halfWindow)) {
            *np = 0;
        }
      else if (*pp > ((double)_pimpl->level + halfWindow)) {
            *np = 0;
        }
        else {
          if (_pimpl->window == 0) 
            *np = 255;
          else
            *np = (*pp - _pimpl->level) * 255.0 /(double)_pimpl->window;
        }
        np++;
        pp++;
    }
  }

  return _pimpl->outputPixel8bit;
}

void Image::setOrientation(const std::vector<double>& ori) {
  bool tran = true;
  bool coro = true;
  bool sagi = true;
  for (unsigned int i = 0; i < 6; ++i) {
    if (tran && !doublesAreEqual(ori.at(i), transOri.at(i))) {
      tran = false;
    }

    if (coro && !doublesAreEqual(ori.at(i), coroOri.at(i))) {
      coro = false;
    }

    if (sagi && !doublesAreEqual(ori.at(i), sagiOri.at(i))) {
      sagi = false;
    }
  }

  if (tran) _pimpl->orientation = TRAN;
  if (coro) _pimpl->orientation = CORO;
  if (sagi) _pimpl->orientation = SAGI;
}

Image::Orientation Image::orientation() const {
  return _pimpl->orientation;
}

unsigned short Image::maxValue() const {
  return _pimpl->maxValue;
}

unsigned short Image::minValue() const {
  return _pimpl->minValue;
}

unsigned short Image::pixelValue(unsigned int index) const {
  return *(_pimpl->pixelData.get() + index);
}

void Image::updateWL(int window, int level) {
  bool wlChanged = false;
  if (_pimpl->window != window) {
    _pimpl->window = window;
    wlChanged = true;
  }

  if (_pimpl->level != level) {
    _pimpl->level = level;
    wlChanged = true;
  }

  if (wlChanged) {
    _pimpl->outputPixel.reset();
    _pimpl->outputPixel8bit.reset();
  }
}

void Image::setPixelSpacing(const std::vector<double>& value) {
  _pimpl->pixelSpacing = value;
}

std::vector<double> Image::pixelSpacing() const {
  return _pimpl->pixelSpacing;
}

int Image::sampleStep() const {
  return _pimpl->sampleStep;
}

void Image::setSampleStep(const int step) {
  if (_pimpl->sampleStep != step) {
    _pimpl->sampleStep = step;
  }
}

Image::DataType Image::dataType() const {
  return _pimpl->dataType;
}

void Image::setDataType(const Image::DataType dataType) {
  _pimpl->dataType = dataType;
}
