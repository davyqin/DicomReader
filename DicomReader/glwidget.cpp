#include <GL/glew.h>
#include "glwidget.h"
#include "wltool.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace {
    GLdouble zoomFactor = 1.0;
    GLint screeny = 0, screenx = 0;
    GLint alignment = 4;
    unsigned char* pData = 0;
    unsigned char* pDataOriginal = 0;
} //namespace

GLWidget::GLWidget( QWidget *parent)
  : QGLWidget(parent), pixelCurve(false)
{
}

GLWidget::~GLWidget() {
  clean();
}

void GLWidget::clean()
{
  delete[] pData;
  delete[] pDataOriginal;
  pData = 0;
  pDataOriginal = 0;
}

void GLWidget::loadFile(const QString fileName)
{
  clean();
  image.setFileName(fileName.toStdString());
  pData = image.pixel();
  pDataOriginal = image.pixel();
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
  if (pixelCurve == flag)
    return;

  pixelCurve = flag;
  updateGL();
}

void GLWidget::initializeGL(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
}

void GLWidget::paintGL(void)
{
#if 1
   glClear(GL_COLOR_BUFFER_BIT);
   if (pData == 0) return;

   glRasterPos2i (screenx, screeny);
   glPixelZoom (zoomFactor, zoomFactor);
   glDrawPixels(image.imageWidth(),  image.imageHeight(), GL_LUMINANCE, GL_UNSIGNED_BYTE, pData);

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
    memcpy(pData, pDataOriginal, image.pixelLength());
    wlTool WLTool(window,level);
    WLTool.convert(pData, image.pixelLength());
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
  glColor3f(255.0, 0.0, 0.0);
  GLint xPos = 1;
  const GLint yPos = 10;
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
    xPos += 2;
  }

  glEnd();
}

void GLWidget::drawCurve()
{
  glColor3f(255.0, 255.0, 0.0);
  GLfloat xPos = 10.0;
  const std::vector<int> pixelCount = image.imagePixelCount();
  glBegin(GL_LINE_STRIP);
  for (int i = 5; i < 256; ++i)
  {
    glVertex2f(xPos, pixelCount.at(i) / 25.0 + 20.0);
    xPos += 2.0;
  }
  glEnd();
}