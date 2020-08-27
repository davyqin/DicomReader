#pragma once

#include "DicomUtil.h"

#include <boost/exception/exception.hpp>
#include <exception>

#include <string>
#include <memory>

class Image;

class DicomUtilImplException : public std::exception, public boost::exception
{
public:
  DicomUtilImplException() {};

  ~DicomUtilImplException() {};
};

class DicomUtilImpl : public DicomUtil
{
public:
  DicomUtilImpl();

  DicomUtilImpl(const std::string& fileName);

  ~DicomUtilImpl();

  void setFileName(const std::string& filename);

  std::shared_ptr<unsigned short> pixel();

  std::shared_ptr<Image> fetchImage() const;

  int pixelLength() const;

  int imageHeight() const;

  int imageWidth() const;

  bool hasPixelData() const;

private:

  void readFile();

  class Pimpl;
  std::unique_ptr<Pimpl> _pimpl;
};