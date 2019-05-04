#ifndef MAINDEMO_H
#define MAINDEMO_H

/**
 * Project "Magnification Glass Widget for Qt"
 *
 * demo/MainDemo.h
 * 
 * Just want to zoom into your QWidget/QLabel Windows like having a mag glas? 
 *
 * You are welcome, here we go!
 *
 * This is mostly a simplified implementation of sub window which
 * can take a rectangle of your currently viewed image (taking from the underlaying full res image)
 * and opens a floating, frameless window around the mouse cursor wit a magnified view of what is 
 * around the mouse pointer.
 * Zoom further in/out via wheel is possible
 * calling as function or via signal/slot
 *
 *
 *
 * Plans:
 * add SIGNAL/SLOT very soon
 * add QSettings store/restore
 * add subclassed QWidget which already owns all we need for graphic with Mag Glass, so one can derive from this
 *
 * 
 * copyright 2019 Modemman, GERMANY
 * 
 * distributed under the terms of the 2-clause license also known as "Simplified BSD License" or "FreeBSD License"
 * License is compatible with GPL and LGPL
 */ 
 
#include "../src/MagnificationGlass.h"

#include <QMainWindow>
#include <QApplication>
#include <QLabel>
#include <QStyle>
#include <QString>
#include <QImage>

#include <QMouseEvent>
#include <QTime>

class LabelWithImage : public QLabel
{
  Q_OBJECT

public:
  explicit LabelWithImage( QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags() );
  explicit LabelWithImage( const QString &text, QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags() );
  explicit LabelWithImage( const QImage &image, int width, int height, QWidget* parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags() );
  virtual ~LabelWithImage() {}

  void setImage( const QImage& image, int width, int height, Qt::AspectRatioMode aspectMode = Qt::KeepAspectRatio, Qt::TransformationMode mode = Qt::SmoothTransformation );
  void setPixmap(const QPixmap& pixmap );

protected:
  void resizeEvent(       QResizeEvent *event );
  void mousePressEvent(   QMouseEvent  *event );
  void mouseMoveEvent(    QMouseEvent  *event );
  void mouseReleaseEvent( QMouseEvent  *event );
  void wheelEvent(        QWheelEvent  *event );

private:
  void setPixmap_internal( const QPixmap& pixmap );
  void updateMargins_internal(void);

private:
  QTime               m_MouseButtonTime;
  QImage              m_Image;
  int                 m_pixmapWidth;
  int                 m_pixmapHeight;
  /* ------ This is what the demo is about: ------*/
  MagnificationGlass  m_MagGlass;
};




class MainDemo : public QMainWindow
{
  Q_OBJECT

public:
  MainDemo( QApplication& Applic, QWidget *parent = nullptr);
  ~MainDemo();

public slots:
  void on_ready(void);

private:
  QApplication&       m_Applic;
  QWidget             m_CentralWidget;
  QString             m_ImageFileNameLeft, m_ImageFileNameRight;
  LabelWithImage      m_MyPictureLandscape, m_MyPicturePortrait;
};

#endif // MAINDEMO_H
