#include <GL/glew.h>
#include "glwidget.h"
#include "wltool.h"

using namespace std;

namespace {
    GLdouble zoomFactor = 1.0;
    GLint screeny = 0, screenx = 0;
} //namespace

GLWidget::GLWidget(QWidget *parent)
  : QGLWidget(parent), pixelCurve(false), imageWindow(255), imageLevel(0), pixelType(BytePixel),
  pData(shared_ptr<unsigned char>()), pDataOriginal(shared_ptr<unsigned char>()),
  pShortData(shared_ptr<unsigned short>()), pShortOriginalData(shared_ptr<unsigned short>())
{
}

GLWidget::~GLWidget() {
  clean();
}

void GLWidget::clean()
{
  pData.reset();
  pDataOriginal.reset();
  pShortData.reset();
  pShortOriginalData.reset();
}

void GLWidget::loadFile(const QString fileName)
{
  clean();
  image.setFileName(fileName.toStdString());
  pData = image.bytePixel();
  pDataOriginal = image.bytePixel();
  pShortData = image.shortPixel();
  pShortOriginalData = image.shortPixel();
  updateGL();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(image.imageWidth(), image.imageHeight());
}

QSize GLWidget::sizeHint() const
{
    return QSize(image.imageWidth(), image.imageHeight());
}

void GLWidget::setZoomFactor(int zoom) {
    zoomFactor = 0.1 * zoom;
    updateGL();
}

void GLWidget::showPixelCurve(bool flag) {
  pixelType = type;

  if (pixelType == BytePixel)
    memcpy(pData.get(), pDataOriginal.get(), image.pixelLength());
  else
    memcpy(pShortData.get(), pShortOriginalData.get(), image.pixelLength());

  updateGL();
}

void GLWidget::setPixelType(PixelType type) {
  if (pixelType == type)
    return;

  pixelType = type;
  updateGL();
}

void GLWidget::initializeGL(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
}

void GLWidget::paintGL(void)
{
#if 1
   glClear(GL_COLOR_BUFFER_BIT);
   if (pData == 0 && pShortData == 0) return;

   glRasterPos2i (screenx, screeny);
   glPixelZoom (zoomFactor, zoomFactor);

   if (pixelType == BytePixel)
     glDrawPixels(image.imageWidth(),  image.imageHeight(), GL_LUMINANCE, GL_UNSIGNED_BYTE, pData.get());
   else
     glDrawPixels(image.imageWidth(), image.imageHeight(), GL_LUMINANCE, GL_UNSIGNED_SHORT, pShortData.get());

   drawPixelCurve();

   glFlush();
#endif
}

void GLWidget::resizeGL(int width, int height)
{
   glViewport(0, 0, (GLsizei)width, (GLsizei)height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void GLWidget::setWindowLevel(int window, int level) {
    imageWindow = window;
    imageLevel = level;
    
    wlTool WLTool(window,level);

    if (pixelType == BytePixel) {
      memcpy(pData.get(), pDataOriginal.get(), image.pixelLength());
      WLTool.convert(pData.get(), image.pixelLength());
    }
    else {
      memcpy(pShortData.get(), pShortOriginalData.get(), image.pixelLength());
      WLTool.convertShortPixel(pShortData.get(), image.pixelLength());
    }

    updateGL();
}

void GLWidget::drawPixelCurve()
{
  if (pixelCurve) {
    drawRuler();
    drawCurve();
  }
}

void GLWidget::drawRuler()
{
  if (pixelType == ShortPixel)
    return;

  GLint xPos = 1;
  const GLint yPos = 10;
  const int pixelSize = image.imageWidth() * image.imageHeight();
  const std::vector<int> pixelCount = image.imagePixelCount();
  glColor3f(255.0, 0.0, 0.0);

  glBegin(GL_LINES);
  // draw baseline
  glVertex2i(xPos, yPos);
  glVertex2i(512, yPos);

  // draw teeth
  for (int i = 0; i < 256; ++i)
  {
    GLint yHight = 5;
    if ((int)xPos % 20 == 1)
      yHight = 10;
    glVertex2i(xPos, yPos);
    glVertex2i(xPos, yPos + yHight);

    if (i == imageLevel)
    {
      glColor3f(255.0, 0.0, 0.0);
      glVertex2i(xPos, yPos);
      glVertex2f(xPos, (float)pixelCount.at(i) / (float)pixelSize * (float)height() * 10.0 + 30.0);
    }
    xPos += 2;
  }

  glEnd();
}

void GLWidget::drawCurve()
{
  const int pixelSize = image.imageWidth() * image.imageHeight();
  GLfloat xPos = 11.0;
  const double halfWindow = imageWindow / 2.0;
  if (pixelType == BytePixel) {
    const std::vector<int> pixelCount = image.imagePixelCount();
    glBegin(GL_LINE_STRIP);
    for (int i = 5; i < 256; ++i)
    {
      if (i >= (imageLevel - halfWindow) && i < (imageLevel + halfWindow))
        glColor3f(255.0, 255.0, 0.0);
      else
        glColor3f(0.0, 255.0, 0.0);

      glVertex2f(xPos, (float)pixelCount.at(i) / (float)pixelSize * (float)height() * 10.0 + 30.0);
      xPos += 2.0;
    }
    glEnd();
  }
}
