#include "MainDemo.h"

#include <QTimer>
#include <QLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QPicture>


MainDemo::MainDemo(QApplication& Applic, QWidget *parent)
  : QMainWindow(parent)
  , m_Applic(Applic)
  , m_CentralWidget( parent )
  , m_MyPictureLandscape( new QLabel )
  , m_MyPicturePortrait( new QLabel )
{

  /* Niklots Schloss */
  QVBoxLayout* LeftPageLSLayout = new QVBoxLayout();
  {
  m_MyPictureLandscape.setScaledContents(true);
  m_MyPictureLandscape.setPixmap( QPixmap( ":/pictures/sample_landscape_castle_szuarin.jpeg" ).scaled(640,480,Qt::KeepAspectRatio, Qt::SmoothTransformation) );
  m_MyPictureLandscape.setAlignment( Qt::AlignCenter | Qt::AlignHCenter );
  m_MyPictureLandscape.setMinimumSize(320,200);
  m_MyPictureLandscape.setMaximumSize(1920,1080);
  /* ------------------ */
  LeftPageLSLayout->addSpacing(1);
  LeftPageLSLayout->addWidget( new QLabel("<hr>", &m_CentralWidget ) );
  LeftPageLSLayout->addSpacing(1);
  /* ------------------ */
  LeftPageLSLayout->addWidget( &m_MyPictureLandscape, 40, Qt::AlignCenter | Qt::AlignHCenter );
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
  m_MyPicturePortrait.setScaledContents(true);
  m_MyPicturePortrait.setPixmap(  QPixmap( ":/pictures/sample_portrait_church_szuarin.jpeg"  ).scaled(480,640,Qt::KeepAspectRatio, Qt::SmoothTransformation) );
  m_MyPicturePortrait.setAlignment( Qt::AlignCenter | Qt::AlignHCenter );
  m_MyPicturePortrait.setMinimumSize(200,320);
  m_MyPicturePortrait.setMaximumSize(1080,1920);
  /* ------------------ */
  RightPagePTLayout->addSpacing(1);
  RightPagePTLayout->addWidget( new QLabel("<hr>", &m_CentralWidget ) );
  RightPagePTLayout->addSpacing(1);
  /* ------------------ */
  RightPagePTLayout->addWidget( &m_MyPicturePortrait, 40, Qt::AlignCenter | Qt::AlignHCenter );
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
  QFrame* vLine1 = new QFrame( &m_CentralWidget );
  vLine1->setFrameShape(QFrame::VLine);
  vLine1->setFrameShadow(QFrame::Sunken);
  TwoPicLayout->addSpacing(1);
  TwoPicLayout->addWidget( vLine1 );
  TwoPicLayout->addSpacing(1);
  /* ------ */
  TwoPicLayout->addLayout( LeftPageLSLayout, 40 );
  /* ------ */
  QFrame* vLine2 = new QFrame( &m_CentralWidget );
  vLine2->setFrameShape(QFrame::VLine);
  vLine2->setFrameShadow(QFrame::Sunken);
  TwoPicLayout->addSpacing(1);
  TwoPicLayout->addWidget( vLine2 );
  TwoPicLayout->addSpacing(1);
  /* ------ */
  TwoPicLayout->addLayout( RightPagePTLayout, 40 );
  /* ------ */
  QFrame* vLine3 = new QFrame( &m_CentralWidget );
  vLine3->setFrameShape(QFrame::VLine);
  vLine3->setFrameShadow(QFrame::Sunken);
  TwoPicLayout->addSpacing(1);
  TwoPicLayout->addWidget( vLine3 );
  TwoPicLayout->addSpacing(1);
  /* ------ */
  }

  m_CentralWidget.setLayout( TwoPicLayout );
  // --- define main windows widget as central one:
  setCentralWidget( &m_CentralWidget );

  double PercentOfScreen = 75.0  / 100.0;
  QDesktopWidget dw;
  QSize newSize( static_cast<int>( dw.availableGeometry(this).size().width() * PercentOfScreen )
               , static_cast<int>( dw.availableGeometry(this).size().height()* PercentOfScreen ) );
  m_CentralWidget.setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, newSize, dw.availableGeometry(this) ) );
  qDebug() << __PRETTY_FUNCTION__ << "Done setting Layout to" << PercentOfScreen * 100 << "% of Screen";

  QTimer::singleShot( 5000 /*ms*/, this, SLOT( on_ready()) );
}


MainDemo::~MainDemo()
{

}

void MainDemo::on_ready()
{
  double PercentOfScreen = 90.0  / 100.0;
  QDesktopWidget dw;
  QSize newSize( static_cast<int>( dw.availableGeometry(this).size().width() * PercentOfScreen )
               , static_cast<int>( dw.availableGeometry(this).size().height()* PercentOfScreen ) );
  this->resize( newSize );
  this->move( static_cast<int>( (dw.availableGeometry(this).size().width() - this->width() )/2.0 )
            , static_cast<int>( (dw.availableGeometry(this).size().height()- this->height())/2.0 ) );
  qDebug() << __PRETTY_FUNCTION__ << "Done aligning App to middle " << PercentOfScreen * 100 << "% of Screen";

}
