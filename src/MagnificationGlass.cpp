/**
 * Project "Magnification Glass Widget for Qt"
 *
 * MagnificationGlass.cpp
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
 
#include "MagnificationGlass.h"

#include <QDebug>
#include <QPixmap>
#include <QTransform>
#include <QtCore/qmath.h>


#define HEXADDR(x)                                      hex << static_cast<const void*>(x)    << dec
#define OBJIDENT(x)  ((x)?x->objectName():"") << "@" << hex << static_cast<const void*>(x)    << dec
#define THISIDENT()  ( this->objectName()   ) << "@" << hex << static_cast<const void*>(this) << dec


MagnificationGlass::MagnificationGlass( QWidget *parent )
  : QWidget(parent)
  , mParent(parent)
  , mMouseXY(0,0)
  , mUpperLeftXY(0,0)
  , mTransformedImagePoint(0,0)
  , mOriginalSize()
  , mWidgetSize()
  , mViewPortSize(1,1)
  , mBigPicture()
  , mMagnification(1.0), mBaseMagnification(mMagnification)
  , mScaling(1.0)
  , mGlassSize(1,1)
  , mPickerScopeRect(0,0,1,1)
  , mLabel( parent )
  , mIsActive(false)
  {
      // Todo: make mMagnification a QSettings parameter
      mMagnification = 5.0;
      mBaseMagnification = mMagnification;
  }


void MagnificationGlass::setup( const QImage bigpicture, const QSize& OriginalSize, const QSize &WidgetSize, const QPoint& MouseXY )
{
    mBigPicture = bigpicture;
    mOriginalSize = OriginalSize;
    mWidgetSize = WidgetSize;
    mMouseXY = MouseXY;

    //qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "---------------------------------";
    //qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "mOriginalSize=" << mOriginalSize << "mWidgetSize=" << mWidgetSize << "@ pos" << mWhere;
    // qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "Point in Widg:" << MagnificationGlass::getPointRelationInRect( mWhere, mWidgetSize );
    // qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "parent->geometry()=" << parent->geometry() << "parent->frameGeometry()=" << parent->frameGeometry();

    double WidgetRatio = MagnificationGlass::getAspectRatio( mWidgetSize );
    double PictureRatio= MagnificationGlass::getAspectRatio( mOriginalSize );
    //qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "PictureRatio:" << PictureRatio << ", WidgetRatio:" << WidgetRatio;
    mViewPortSize= getViewPortSizeOfWidget( mWidgetSize, WidgetRatio, PictureRatio );
    mScaling = MagnificationGlass::getSquareScale( mOriginalSize, mViewPortSize );
    // qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "ViewPort:" << mViewPortSize << ", Magnification=" << mScaling;

    // Todo: make 1/9.0 a QSettings parameter
    double GlassSizeRelativeToViewport = 1/9.0;
    mGlassSize = MagnificationGlass::ScaleBySqare( mViewPortSize, GlassSizeRelativeToViewport );

    // normalize: if the shown picture is 1/2.0 the size of the real picture, and the magnification is 4.0, the resulting mag has to be 2.0
    QSize PickerScopeSize( MagnificationGlass::ScaleBySqare( mGlassSize, 1/(mMagnification*mScaling) ) );
    // qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "before scaling with magnification mGlassSize:" << mGlassSize << "after, PickerScopeSize:" << PickerScopeSize;

    mTransformedImagePoint = MagnificationGlass::getTransformedCoordinates( mMouseXY, mOriginalSize, mWidgetSize, mViewPortSize, WidgetRatio, PictureRatio );
    mPickerScopeRect = MagnificationGlass::PlaceCenteredSubRectInside( mOriginalSize, PickerScopeSize, mTransformedImagePoint );
    // qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "PickerScopeSize:" << PickerScopeSize << "PickerScopeRect:" << mPickerScopeRect << "after scaling with magnification";

    this->Hide();
}


QSize MagnificationGlass::getViewPortSizeOfWidget( const QSize& WidgetSize, double WidgetRatio, double PictureRatio )
{
  int ViewportWidth=1, ViewportHeight=1;

  if( PictureRatio > WidgetRatio ) // a centered image will be filled up above and below, so the x-axis width is the same for Widget and Picture
  {
    ViewportWidth = WidgetSize.width();
    ViewportHeight= MagnificationGlass::getSizeByWidth( ViewportWidth, PictureRatio ).height();
  }
  else // a centered image will be filled up left and right, so the y-axis height is the same for Widget and Picture
  {
    ViewportHeight= WidgetSize.height();
    ViewportWidth = MagnificationGlass::getSizeByHeight( ViewportHeight, PictureRatio ).width();
  }

  return QSize( ViewportWidth, ViewportHeight ); /* size of the visible Image*/
}



QPoint MagnificationGlass::getTransformedCoordinates( const QPoint& Where, const QSize& OriginalSize, const QSize& WidgetSize, const QSize& ViewportSize, double WidgetRatio, double PictureRatio )
{
    int Xoffset=0, Yoffset=0; // Mouse correction Coordinates
    if( PictureRatio > WidgetRatio ) // a centered image will be filled up above and below, so the x-axis width is the same for Widget and Picture
    {
      Yoffset = (WidgetSize.height() - ViewportSize.height()) / 2;  // if centered image was filled up above and below, we need to subtract the half fillup from Mouse-Y
    }
    else // a centered image will be filled up left and right, so the y-axis height is the same for Widget and Picture
    {
      Xoffset = (WidgetSize.width() - ViewportSize.width()) / 2;  // if centered image was filled up left and right, we need to subtract the half fillup from Mouse-X
    }


    QPoint Coord(0,0);
    Coord.setX( qMin( ViewportSize.width(),  qMax( 0, Where.x() - Xoffset ) ) );  // if centered image was filled up somewhere, we need to subtract the half fillup-offset
    Coord.setY( qMin( ViewportSize.height(), qMax( 0, Where.y() - Yoffset ) ) );  // ...
    // qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "Point in Widget:" << mWhere << "== Point in View:" << Coord;
    // return Coord;

    QTransform tr;
    tr.scale( static_cast<qreal>(OriginalSize.width() ) / static_cast<qreal>(ViewportSize.width() ), \
              static_cast<qreal>(OriginalSize.height()) / static_cast<qreal>(ViewportSize.height()) );
    QPoint imagePoint = tr.map( Coord );
    // qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "Point in Widget:" << mWhere << "== Point in View:" << Coord << "Point in Original Image:" << imagePoint;
    // qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "in Orig:" << MagnificationGlass::getPointRelationInRect(imagePoint,mOriginalSize) << "in View:" << MagnificationGlass::getPointRelationInRect(mMouseInView,mViewPortSize);
    return imagePoint;
}




void MagnificationGlass::Move(const QPoint& MouseXY )
{
  //qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "Mouse Moved from" << mMouseXY << "to" << MouseXY;
  mMouseXY = MouseXY;
  //mLabel.move( getTopLeft_and_AdjustCenterOfSubRectInside( mWidgetSize, mGlassSize, MouseXY ) );
  this->Show( mMouseXY );
}

void MagnificationGlass::Hide(void)
{
    mLabel.hide();
}



void MagnificationGlass::Show( const QPoint& MouseXY )
{
  mUpperLeftXY = getTopLeft_and_AdjustCenterOfSubRectInside( mWidgetSize, mGlassSize, MouseXY );
  double WidgetRatio = MagnificationGlass::getAspectRatio( mWidgetSize );
  double PictureRatio= MagnificationGlass::getAspectRatio( mOriginalSize );
  QSize PickerScopeSize = mPickerScopeRect.size();
  mTransformedImagePoint = MagnificationGlass::getTransformedCoordinates( MouseXY, mOriginalSize, mWidgetSize, mViewPortSize, WidgetRatio, PictureRatio );
  mPickerScopeRect = MagnificationGlass::PlaceCenteredSubRectInside( mOriginalSize, PickerScopeSize, mTransformedImagePoint );
  mLabel.setPixmap( QPixmap::fromImage( mBigPicture ).copy( mPickerScopeRect ) );
  // qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "setPixmap from" << mPickerScopeRect << "@" << MouseXY << "UpperLeftXY:" << mUpperLeftXY /*<< "Pic" << HEXADDR(&mBigPicture)*/;
  mLabel.setMinimumSize( mGlassSize );
  mLabel.setMaximumSize( mGlassSize );
  mLabel.setScaledContents(true);
  mLabel.adjustSize();
  mLabel.move( mUpperLeftXY );
  mLabel.show();
}


void MagnificationGlass::reportZoom(int Step)
{
    mMagnification += (Step/120)/1.0;  // one mouse wheel step is 120
    QSize PickerScopeSize( MagnificationGlass::ScaleBySqare( mGlassSize, 1/(mMagnification*mScaling) ) );
    mPickerScopeRect = MagnificationGlass::PlaceCenteredSubRectInside( mOriginalSize, PickerScopeSize, mTransformedImagePoint );
    // qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "Zoom" << mMagnification << "PickerScopeSize" << PickerScopeSize << "mPickerScopeRect" << mPickerScopeRect;
    this->Show( mMouseXY );
}

void MagnificationGlass::resetZoom()
{
    mMagnification = mBaseMagnification;
    QSize PickerScopeSize( MagnificationGlass::ScaleBySqare( mGlassSize, 1/(mMagnification*mScaling) ) );
    mPickerScopeRect = MagnificationGlass::PlaceCenteredSubRectInside( mOriginalSize, PickerScopeSize, mTransformedImagePoint );
    this->update();
    this->Show( mMouseXY );
}


/* get the x:y ratio of a rectangle / picture */
/* rough: 1.0 is sqare, 0.xxx is portrait, >1.xxx is landscape */
double MagnificationGlass::getAspectRatio( const QSize& size )
{   return MagnificationGlass::getAspectRatio( size.width(), size.height() );
}

double MagnificationGlass::getAspectRatio( const QRect& rect )
{   return MagnificationGlass::getAspectRatio( rect.width(), rect.height() );
}

double MagnificationGlass::getAspectRatio(int x, int y)
{   return static_cast<double>(x) / static_cast<double>(y);
}


/* get the ratio between two pictures, will be bit wrong if both are not at same x:y ratio */
/* if image 2 width is double the size of image 1 width, the return value is 2.0 */
double MagnificationGlass::getXScale(const QSize& size1, const QSize& size2)
{   return MagnificationGlass::getAspectRatio( size2.width(), size1.width() );
}

double MagnificationGlass::getXScale(const QRect& rect1, const QRect& rect2)
{   return MagnificationGlass::getAspectRatio( rect2.width(), rect1.width() );
}

/* if image 2 heigth is double the size of image 1 heigth, the return value is 2.0 */
double MagnificationGlass::getYScale(const QSize& size1, const QSize& size2)
{   return MagnificationGlass::getAspectRatio( size2.height(), size1.height() );
}

double MagnificationGlass::getYScale(const QRect& rect1, const QRect& rect2)
{   return MagnificationGlass::getAspectRatio( rect2.height(), rect1.height() );
}

/* if image 2 sqare is double the size of image 1 sqare, the return value is 2.0
 * if image 2 with and heigt is double the size of image 1 with and heigt, the return value is 4.0
 */
double MagnificationGlass::getSquareScale(const QSize& size1, const QSize& size2)
{ return MagnificationGlass::getXScale(size1,size2) * \
         MagnificationGlass::getYScale(size1,size2);
}

double MagnificationGlass::getSquareScale(const QRect& rect1, const QRect& rect2)
{ return MagnificationGlass::getXScale(rect1,rect2) * \
         MagnificationGlass::getYScale(rect1,rect2);
}


/* make a new size/rect based on a given and a scale factor (1:1 is same, 0.xxx is smaller, > 1.xxx is bigger)
 * scaling by 2.0 makes a 4.0 times larger rect
 */
QSize MagnificationGlass::ScaleByX(const QSize& sizeIn, double SideScaleFactor)
{   return QSize( static_cast<int>( sizeIn.width() * SideScaleFactor ),
                  static_cast<int>( sizeIn.height() * SideScaleFactor ) );
}

QRect MagnificationGlass::ScaleByX( const QRect& rectIn, double SideScaleFactor )
{   return QRect( rectIn.topLeft(), MagnificationGlass::ScaleByX( rectIn.size(), SideScaleFactor) );
}

/* and here the square base functions: scaling by 2.0 makes a 2.0 times larger rect with 1.41 times larger sides: */
QSize MagnificationGlass::ScaleBySqare(const QSize& sizeIn, double SqareScaleFactor)
{   return MagnificationGlass::ScaleByX( sizeIn, qSqrt(SqareScaleFactor) );
}

QRect MagnificationGlass::ScaleBySqare(const QRect& rectIn, double SqareScaleFactor)
{   return MagnificationGlass::ScaleByX( rectIn, qSqrt(SqareScaleFactor) );
}


/* get the y part of a rectangle / picture, by giving x part and x:y ratio */
QSize MagnificationGlass::getSizeByWidth( int width, double aspectRatio )
{   /* h = w / (w/h) */
    return QSize( width, static_cast<int>(width / aspectRatio) );
}

QRect MagnificationGlass::getRectByWidth( const QPoint& atopleft, int width, double aspectRatio )
{   return QRect( atopleft, MagnificationGlass::getSizeByWidth( width, aspectRatio ) );
}

/* get the x part of a rectangle / picture, by giving y part and x:y ratio */
QSize MagnificationGlass::getSizeByHeight(int height, double aspectRatio)
{ /* w = h * (w/h) */
  return QSize( static_cast<int>(height * aspectRatio), height );
}

QRect MagnificationGlass::getRectByHeight(const QPoint& atopleft, int height, double aspectRatio)
{   return QRect( atopleft, MagnificationGlass::getSizeByHeight( height, aspectRatio ) );
}

/* get the percentual information, where a point inside a rectangle points,
 * like: @(10%,30%)
 */
QString MagnificationGlass::getPointRelationInRect( const QPoint& point, const QSize& inSize )
{
    double x = static_cast<double>(100.0f * point.x()) / static_cast<double>( inSize.width() );
    double y = static_cast<double>(100.0f * point.y()) / static_cast<double>( inSize.height());

    return QString( "@(%1%,%2%)" ).arg(x,0,'f',2).arg(y,0,'f',2);
}

QString MagnificationGlass::getPointRelationInRect(const QPoint& point, const QRect& inRect)
{   return MagnificationGlass::getPointRelationInRect( point, inRect.size() );
}

QRect MagnificationGlass::PlaceCenteredSubRectInside(const QSize& BigRect, const QSize& SubRect, const QPoint& CenterCoord)
{
  /* the upper left corner shall never get negative, the lower right corner shall never leave BigPicture to bottom or right */
    int MinLeft = SubRect.width() / 2;
    int MaxLeft = BigRect.width() - SubRect.width() / 2;
    int MinTop  = SubRect.height() / 2;
    int MaxTop  = BigRect.height() - SubRect.height() / 2;
    // qDebug() << __PRETTY_FUNCTION__ << __LINE__ << ": MinLeft" << MinLeft << "MaxLeft" << MaxLeft << "MinTop" << MinTop << "MaxTop" << MaxTop << ", CenterCoord" << CenterCoord;
    /* --- X-axis --- */
    int Left = CenterCoord.x();
    Left = qMax( Left, MinLeft );
    Left = qMin( Left, MaxLeft );
    /* --- Y-axis --- */
    int Top  = CenterCoord.y();
    Top = qMax( Top, MinTop );
    Top = qMin( Top, MaxTop );

    Left -= SubRect.width()/2;
    Top  -= SubRect.height()/2;
    // bring to top-left / bottom-right Rectangle coordinates:
    QRect resultingRect( Left, Top, SubRect.width(), SubRect.height() );
    // qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "resultingRect" << resultingRect;
    return resultingRect;
}

QPoint MagnificationGlass::getTopLeft_and_AdjustCenterOfSubRectInside( const QSize& BigRect, const QSize& SubRect, const QPoint& CenterCoord )
{
  QRect PointOfRect = MagnificationGlass::PlaceCenteredSubRectInside( BigRect, SubRect, CenterCoord );
  // qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "PointOfRect" << PointOfRect;
  return PointOfRect.topLeft();
}
