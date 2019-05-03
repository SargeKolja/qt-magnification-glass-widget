#ifndef MAGNIFICATIONGLASS_H
#define MAGNIFICATIONGLASS_H

/**
 * Project "Magnification Glass Widget for Qt"
 *
 * MagnificationGlass.h
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


#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QSize>
#include <QLabel>

class MagnificationGlass : public QWidget
{
    Q_OBJECT
public:
    explicit MagnificationGlass( QWidget *parent = nullptr );

public:
    void setup(const QImage bigpicture,
               const QSize& OriginalSize,
               const QSize& WidgetSize,
               const QPoint& Where = QPoint());

    bool isActive(void) const {return mIsActive; }
    void activate(void) {mIsActive=true;}
    void deactivate(void) {mIsActive=false;}
    void Move(const QPoint& MouseXY);
    void Show(const QPoint& MouseXY);
    void Hide();
    void resetZoom();
    void reportZoom( int Step );

    static QSize  getViewPortSizeOfWidget(const QSize& WidgetSize, double WidgetRatio, double PictureRatio);
    static QPoint getTransformedCoordinates(const QPoint& Where,
                                            const QSize& OriginalSize,
                                            const QSize& WidgetSize,
                                            const QSize& ViewportSize,
                                            double WidgetRatio, double PictureRatio);

  /* NOTE: Scale is alwas a one-dimensional factor, so the scale
   * between picture 200x100 and picture 400x200 is NOT 4.0 (square of pic2 / square of pic1)
   * but it is 2.0 ( width of pic2 / width of pic1)
   *   and vice versa:
   * scaling up a 640x480 image by 2.0 is NOT 905x678 ( 2.0 * square )
   * but 1280x960 ( width * 2.0 and height * 2.0 )
   *   or with Pytagoras:
   * To scale up the image's sqare by 2.0, scale its sides by sqrt(2.0)
   * To scale down the image's sqare by 2.0, scale its sides by (1/sqrt(2.0))
   */

  /* get the x:y ratio of a rectangle / picture */
  static double getAspectRatio( const QSize& size );
  static double getAspectRatio( const QRect& rect );
  static double getAspectRatio( int x, int y );

  /* get the ratio between two pictures, will be bit wrong if both are not at same x:y ratio */
  static double getXScale( const QSize& size1, const QSize& size2 );
  static double getXScale( const QRect& rect1, const QRect& rect2 );
  static double getYScale( const QSize& size1, const QSize& size2 );
  static double getYScale( const QRect& rect1, const QRect& rect2 );
  static double getSquareScale( const QSize& size1, const QSize& size2 );
  static double getSquareScale( const QRect& size1, const QRect& size2 );

  /* make a new size/rect based on a given and a scale factor (1:1 is same, 0.xxx is smaller, > 1.xxx is bigger)
   * scaling by 2.0 makes a 4.0 times larger rect
   */
  static QSize ScaleByX( const QSize& sizeIn, double SideScaleFactor );
  static QRect ScaleByX( const QRect& rectIn, double SideScaleFactor );
  /* and here the square base function: scaling by 2.0 makes a 2.0 times larger rect with 1.41 times larger sides: */
  static QSize ScaleBySqare( const QSize& sizeIn, double SqareScaleFactor );
  static QRect ScaleBySqare( const QRect& rectIn, double SqareScaleFactor );

  /* get the y part of a rectangle / picture, by giving x part and x:y ratio */
  static QSize getSizeByWidth( int width, double aspectRatio );
  static QRect getRectByWidth( const QPoint& atopleft, int width, double aspectRatio );

  /* get the x part of a rectangle / picture, by giving y part and x:y ratio */
  static QSize getSizeByHeight( int height, double aspectRatio );
  static QRect getRectByHeight( const QPoint& atopleft, int height, double aspectRatio );

  /* get the percentual information, where a point inside a rectangle points,
   * like: @(10%,30%)
   */
  static QString getPointRelationInRect( const QPoint& point, const QSize& inSize );
  static QString getPointRelationInRect( const QPoint& point, const QRect& inRect );

  static QRect  PlaceCenteredSubRectInside( const QSize& BigRect, const QSize& SubRect, const QPoint& CenterCoord );
  static QPoint getTopLeft_and_AdjustCenterOfSubRectInside(const QSize& BigRect, const QSize& SubRect, const QPoint& CenterCoord );

signals:
  //
public slots:
  //
private:
    MagnificationGlass();
    Q_DISABLE_COPY(MagnificationGlass)
    // ------ construction time --------
    QWidget*      mParent;
    QPoint        mMouseXY;
    QPoint        mUpperLeftXY;
    QPoint        mTransformedImagePoint; // Widget Point (mouse) transformed to point in the underlying picture
    QSize         mOriginalSize;
    QSize         mWidgetSize;
    QSize         mViewPortSize;
    QImage        mBigPicture;
    // ------ runtime --------
    double        mMagnification, mBaseMagnification;
    double        mScaling;
    QSize         mGlassSize;
    QRect         mPickerScopeRect;
    QLabel        mLabel;
    bool          mIsActive;
};

#endif // MAGNIFICATIONGLASS_H
