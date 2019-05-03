#ifndef MAINDEMO_H
#define MAINDEMO_H

#include <QMainWindow>
#include <QApplication>
#include <QLabel>
#include <QStyle>

class MainDemo : public QMainWindow
{
  Q_OBJECT

public:
  MainDemo( QApplication& Applic, QWidget *parent = nullptr);
  ~MainDemo();

public slots:
  void on_ready(void);

private:
  QApplication&    m_Applic;
  QWidget          m_CentralWidget;
  QLabel           m_MyPictureLandscape;
  QLabel           m_MyPicturePortrait;
};

#endif // MAINDEMO_H
