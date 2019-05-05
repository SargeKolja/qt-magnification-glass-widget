/**
 * Project "Magnification Glass Widget for Qt"
 *
 * demo/MainDemo.cpp
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
 
#include "MainDemo.h"

#include <QTimer>
#include <QLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QPicture>
#include <QPainter>






MainDemo::MainDemo(QApplication& Applic, QWidget *parent)
  : QMainWindow(parent)
  , m_Applic(Applic)
  , m_CentralWidget( parent )
  , m_ImageFileNameLeft( ":/pictures/sample_landscape_castle_szuarin.jpeg" )
  , m_ImageFileNameRight(":/pictures/sample_portrait_church_szuarin.jpeg" )
  , m_MyPictureLandscape( QImage(m_ImageFileNameLeft ), 640, 480, parent )
  , m_MyPicturePortrait(  QImage(m_ImageFileNameRight), 480, 640, parent )
{
  /* Niklots Schloss */
  QVBoxLayout* LeftPageLSLayout = new QVBoxLayout();
  {
  m_MyPictureLandscape.setScaledContents(true);
  m_MyPictureLandscape.setAlignment( Qt::AlignCenter | Qt::AlignHCenter );
  m_MyPictureLandscape.setMinimumSize(320,200);
  m_MyPictureLandscape.setMaximumSize(1920,1080);
  /* ------------------ */
  LeftPageLSLayout->addSpacing(1);
  LeftPageLSLayout->addWidget( new QLabel("<hr>", &m_CentralWidget ) );
  LeftPageLSLayout->addSpacing(1);
  /* ------------------ */
  LeftPageLSLayout->addStretch(1);
  LeftPageLSLayout->addWidget( &m_MyPictureLandscape, 98, Qt::AlignCenter | Qt::AlignHCenter );
  LeftPageLSLayout->addStretch(1);
  /* ------------------ */
  LeftPageLSLayout->addSpacing(1);
  LeftPageLSLayout->addWidget( new QLabel("<hr>", &m_CentralWidget ) );
  LeftPageLSLayout->addSpacing(1);
  /* ------------------ */
  }


  /* Dom */
  QVBoxLayout* RightPagePTLayout= new QVBoxLayout();
  {
  // not for Widget: m_MyPicturePortrait.setScaledContents(true);
  // not for Widget: m_MyPicturePortrait.setAlignment( Qt::AlignCenter | Qt::AlignHCenter );
  m_MyPicturePortrait.setMinimumSize(200,320);
  m_MyPicturePortrait.setMaximumSize(1080,1920);
  m_MyPicturePortrait.setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
  //qDebug() << __PRETTY_FUNCTION__ << "pic.sizeHint:" << m_MyPicturePortrait.sizeHint() << "lay.sizeHint:" << RightPagePTLayout->sizeHint();
  /* ------------------ */
  RightPagePTLayout->addSpacing(1);
  RightPagePTLayout->addWidget( new QLabel("<hr>", &m_CentralWidget ) );
  RightPagePTLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
  RightPagePTLayout->addSpacing(1);
  /* ------------------ */
  RightPagePTLayout->addStretch(1);
  RightPagePTLayout->addWidget( &m_MyPicturePortrait, 98, Qt::AlignCenter | Qt::AlignHCenter );
  RightPagePTLayout->addStretch(1);
  /* ------------------ */
  RightPagePTLayout->addSpacing(1);
  RightPagePTLayout->addWidget( new QLabel("<hr>", &m_CentralWidget ) );
  RightPagePTLayout->addSpacing(1);
  /* ------------------ */
  }

  /* Postcard together */
  QHBoxLayout* TwoPicLayout= new QHBoxLayout(&m_CentralWidget);
  {
  /* ------ */
  {QFrame* vLine1 = new QFrame( &m_CentralWidget );
   vLine1->setFrameShape(QFrame::VLine);
   vLine1->setFrameShadow(QFrame::Sunken);
   TwoPicLayout->addSpacing(1);
   TwoPicLayout->addWidget( vLine1 );
   TwoPicLayout->addSpacing(1);}
  /* ------ PICTURE LEFT ----------- */
  TwoPicLayout->addLayout( LeftPageLSLayout, 47 );
  /* ------------------------------- */
  {QFrame* vLine2 = new QFrame( &m_CentralWidget );
   vLine2->setFrameShape(QFrame::VLine);
   vLine2->setFrameShadow(QFrame::Sunken);
   TwoPicLayout->addStretch(1);
   TwoPicLayout->addSpacing(1);
   TwoPicLayout->addWidget( vLine2 );
   TwoPicLayout->addSpacing(1);
   TwoPicLayout->addStretch(1);}
  /* ------ PICTURE RIGHT ----------- */
  TwoPicLayout->addLayout( RightPagePTLayout, 47 );
  /* ------------------------------- */
  {QFrame* vLine4 = new QFrame( &m_CentralWidget );
   vLine4->setFrameShape(QFrame::VLine);
   vLine4->setFrameShadow(QFrame::Sunken);
   TwoPicLayout->addSpacing(1);
   TwoPicLayout->addWidget( vLine4 );
   TwoPicLayout->addSpacing(1);}
  /* ------ */
  }

  m_CentralWidget.setLayout( TwoPicLayout );
  // --- define main windows widget as central one:
  setCentralWidget( &m_CentralWidget );

  double PercentOfScreen = 70.0  / 100.0;
  QDesktopWidget dw;
  QSize newSize( static_cast<int>( dw.availableGeometry(this).size().width() * PercentOfScreen )
               , static_cast<int>( dw.availableGeometry(this).size().height()* PercentOfScreen ) );
  m_CentralWidget.setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, newSize, dw.availableGeometry(this) ) );
  qDebug() << __PRETTY_FUNCTION__ << "Done setting Layout to" << PercentOfScreen * 100 << "% of Screen";

  QTimer::singleShot( 5000 /*ms*/, this, SLOT( on_ready()) );
}


MainDemo::~MainDemo()
{ // Nothing to destroy, it's all QObject managed
}


void MainDemo::on_ready()
{
  double PercentOfScreen = 80.0  / 100.0;
  QDesktopWidget dw;
  QSize newSize( static_cast<int>( dw.availableGeometry(this).size().width() * PercentOfScreen )
               , static_cast<int>( dw.availableGeometry(this).size().height()* PercentOfScreen ) );
  this->resize( newSize );
  this->move( static_cast<int>( (dw.availableGeometry(this).size().width() - this->width() )/2.0 )
            , static_cast<int>( (dw.availableGeometry(this).size().height()- this->height())/2.0 ) );
  qDebug() << __PRETTY_FUNCTION__ << "Done aligning app to screen middle " << PercentOfScreen * 100 << "% of Screen";
}









// ==============================================================================
// ==============================================================================

LabelWithImage::LabelWithImage(QWidget* parent, Qt::WindowFlags f)
  : QLabel(parent,f)
  , m_MouseButtonTime()
  , m_pixmapWidth(0)
  , m_pixmapHeight(0)
  , m_MagGlass( this )
{
}


LabelWithImage::LabelWithImage(const QString& text, QWidget* parent, Qt::WindowFlags f)
  : QLabel(text, parent,f)
  , m_MouseButtonTime()
  , m_pixmapWidth(0)
  , m_pixmapHeight(0)
  , m_MagGlass( this )
{
}


LabelWithImage::LabelWithImage(const QImage& image, int width, int height, QWidget* parent, Qt::WindowFlags f)
  : QLabel(parent,f)
  , m_MouseButtonTime()
  , m_pixmapWidth(0)
  , m_pixmapHeight(0)
  , m_MagGlass( this )
{
  this->setImage( image, width, height );
}


void LabelWithImage::setImage(const QImage& image,
                              int width, int height,
                              Qt::AspectRatioMode aspectMode,
                              Qt::TransformationMode mode )
{
  this->m_Image = image;
  this->setPixmap_internal( QPixmap::fromImage( m_Image ).scaled(width,height,aspectMode,mode) );
}

void LabelWithImage::setPixmap(const QPixmap& pixmap)
{
  this->m_Image = pixmap.toImage();
  this->setPixmap_internal( pixmap );
}


void LabelWithImage::setPixmap_internal(const QPixmap& pixmap)
{
    m_pixmapWidth = pixmap.width();
    m_pixmapHeight = pixmap.height();

    updateMargins_internal();
    QLabel::setPixmap(pixmap);
}


void LabelWithImage::resizeEvent(QResizeEvent* event)
{
    // qDebug() << __PRETTY_FUNCTION__ << "ResizeEvent=" << event;
    if( m_MagGlass.isActive() )
        m_MagGlass.setup( m_Image, event->size(), this->size() );

    this->updateMargins_internal();
    QLabel::resizeEvent(event);
}


void LabelWithImage::mousePressEvent(QMouseEvent* event)
{
  Qt::MouseButton Button = event->button();
//Qt::MouseButtons Buttons = event->buttons();
  QPoint MouseXY = event->pos();

  int TimeBetween = m_MouseButtonTime.restart();
  bool DoubleClicked = (TimeBetween<500/*ms*/) ? true:false;

  // qDebug() << __PRETTY_FUNCTION__ << "Button" << Button << "/" << Buttons << "at" << MouseXY << ((DoubleClicked)? "double":"single") << "clicked within" << TimeBetween << "ms";
  if( Button == Qt::RightButton )
  {
      m_MagGlass.activate();
      m_MagGlass.setup( m_Image, m_Image.size(), this->size(), MouseXY );
      m_MagGlass.Show( event->pos() );

      if( DoubleClicked )
         m_MagGlass.resetZoom();
      return;
  }
}


void LabelWithImage::mouseMoveEvent(QMouseEvent* event)
{
  // event->pos() reports the position of the mouse cursor, relative to this widget.
  // If you want to show a tooltip immediately, while the mouse is moving (e.g., to get the mouse coordinates with QMouseEvent::pos() and show them as a tooltip),
  // you must first enable mouse tracking as described above. Then, to ensure that the tooltip is updated immediately, you must call QToolTip::showText()
  // instead of setToolTip() in your implementation of mouseMoveEvent().
  //QPoint Where = event->pos();
  //Qt::MouseButton Button = event->button();
  // qDebug() << __PRETTY_FUNCTION__ << "(" << objectName() << ") Mouse with Button" << Button << "at" << Where << "moved";
  if( m_MagGlass.isActive() )
  {
      m_MagGlass.Move( event->pos() );
  }
}


void LabelWithImage::mouseReleaseEvent(QMouseEvent* event)
{
    if( event->button() == Qt::RightButton )
    {
        m_MagGlass.deactivate();
        m_MagGlass.Hide();
    }
}


void LabelWithImage::wheelEvent(QWheelEvent* event)
{
    m_MagGlass.reportZoom( event->delta() );
}


void LabelWithImage::updateMargins_internal(void)
{
    if( (m_pixmapWidth>0) && (m_pixmapHeight>0) )
    {
        int current_w = this->width();
        int current_h = this->height();

        if( (current_w>0) && (current_h>0) )
        {
            if( (current_w * m_pixmapHeight) > (current_h * m_pixmapWidth) )
            {
                int mix = (current_w - (m_pixmapWidth * current_h / m_pixmapHeight)) / 2;
                setContentsMargins( mix, 0, mix, 0 );
            }
            else
            {
                int miy = (current_h - (m_pixmapHeight * current_w / m_pixmapWidth)) / 2;
                setContentsMargins( 0, miy, 0, miy );
            }
        } // if (current_w > 0 && current_h > 0)
    } // if( m_pixmapWidth > 0 && m_pixmapHeight <= 0 )


}

// ==============================================================================
// ==============================================================================

WidgetWithImage::WidgetWithImage(QWidget* parent, Qt::WindowFlags f)
  : QWidget( parent, f )
  , m_MouseButtonTime()
  , m_pixmapWidth(0)
  , m_pixmapHeight(0)
  , m_aspectRatioMode(Qt::KeepAspectRatio)
  , m_transformationMode(Qt::SmoothTransformation)
  , m_MagGlass( this )
{
}


WidgetWithImage::WidgetWithImage(const QImage& image, int width, int height, QWidget* parent, Qt::WindowFlags f)
  : QWidget( parent,f )
  , m_MouseButtonTime()
  , m_pixmapWidth(0)
  , m_pixmapHeight(0)
  , m_aspectRatioMode(Qt::KeepAspectRatio)
  , m_transformationMode(Qt::SmoothTransformation)
  , m_MagGlass( this )
{
    this->setImage( image, width, height );
}


void WidgetWithImage::setImage(const QImage& image, int width, int height,
                               Qt::AspectRatioMode aspectMode, Qt::TransformationMode mode)
{
    // qDebug() << __PRETTY_FUNCTION__ << "set Image=" << image << "h=" << height << "w=" << width ;
    setImage_internal( image, width, height, aspectMode, mode );
}


void WidgetWithImage::setPixmap(const QPixmap& pixmap)
{
    //qDebug() << __PRETTY_FUNCTION__ << "set Pixmap=" << pixmap;
    setImage_internal( pixmap.toImage(), pixmap.width(), pixmap.height() );
}


void WidgetWithImage::setImage_internal( const QImage& image, int width, int height,
                                         Qt::AspectRatioMode aspectMode, Qt::TransformationMode mode )
{
    m_Image = image;
    m_aspectRatioMode = aspectMode;
    m_transformationMode = mode;
    m_pixmapWidth = width;
    m_pixmapHeight = height;

    updateMargins_internal();
}


QSize	WidgetWithImage::sizeHint( void ) const
{
    QSize bestSize( m_pixmapWidth, m_pixmapHeight );
    return bestSize;
}

void WidgetWithImage::paintEvent( QPaintEvent *event )
{
    QSize WidgetSize( this->size() );
    QSize PixmapSize( m_pixmapWidth, m_pixmapHeight );
    // qDebug() << __PRETTY_FUNCTION__ << "paintEvent=" << event << "WidgetSize" << WidgetSize << "newSize" << PixmapSize << "sizeHint:" << sizeHint();
    QPainter wPainter;
    //QImage image( m_Image.scaled( PixmapSize, m_aspectRatioMode, m_transformationMode ) );
    QImage image( m_Image.scaled( WidgetSize, m_aspectRatioMode, m_transformationMode ) );

    /* painting of the picture inside the widget at center */
    wPainter.begin( this );
    int StartX = ( width() - image.width() ) / 2;
    int StartY = ( height() - image.height() ) / 2;
    wPainter.drawImage( StartX, StartY, image ); // a scaled down version, if m_pixmapWidth/Heigt > this->size(), otherwise scaled up

    wPainter.end();
    //QWidget::paintEvent(event);
    event->accept();
}


void WidgetWithImage::resizeEvent(QResizeEvent* event)
{
    //qDebug() << __PRETTY_FUNCTION__ << "ResizeEvent=" << event << "sizeHint:" << sizeHint();
    // m_pixmapWidth, m_pixmapHeight

    if( m_MagGlass.isActive() )
        m_MagGlass.setup( m_Image, event->size(), this->size() );
}


void WidgetWithImage::mousePressEvent(QMouseEvent* event)
{
  Qt::MouseButton Button = event->button();
//Qt::MouseButtons Buttons = event->buttons();
  QPoint MouseXY = event->pos();

  int TimeBetween = m_MouseButtonTime.restart();
  bool DoubleClicked = (TimeBetween<500/*ms*/) ? true:false;

  // qDebug() << __PRETTY_FUNCTION__ << "Button" << Button << "/" << Buttons << "at" << MouseXY << ((DoubleClicked)? "double":"single") << "clicked within" << TimeBetween << "ms";
  if( Button == Qt::RightButton )
  {
      m_MagGlass.activate();
      m_MagGlass.setup( m_Image, m_Image.size(), this->size(), MouseXY );
      m_MagGlass.Show( event->pos() );

      if( DoubleClicked )
         m_MagGlass.resetZoom();
      return;
  }
}


void WidgetWithImage::mouseMoveEvent(QMouseEvent* event)
{
    if( m_MagGlass.isActive() )
    {   m_MagGlass.Move( event->pos() );
    }
}


void WidgetWithImage::mouseReleaseEvent(QMouseEvent* event)
{
    if( event->button() == Qt::RightButton )
    {
        m_MagGlass.deactivate();
        m_MagGlass.Hide();
    }
}


void WidgetWithImage::wheelEvent(QWheelEvent* event)
{
    m_MagGlass.reportZoom( event->delta() );
}


void WidgetWithImage::updateMargins_internal()
{
}

