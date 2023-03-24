#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>
#include <QTimer>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <qlabel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:

  void on_checkBox_stateChanged(int arg1);

  bool event(QEvent *event);
  //    bool eventFilter(QObject* watched, QEvent* event);

  void on_checkBox_2_stateChanged(int arg1);
  void on_checkBox_3_stateChanged(int arg1);
  void on_checkBox_4_stateChanged(int arg1);

  void on_shining_clicked();
  void bulr();
  void on_dump_clicked();

  void changeSwitchSpeed(int speed);

  void on_speedBar_valueChanged(int value);

  void on_speedSlider_sliderMoved(int position);

  void on_pushButton_clicked();

  void on_speedSlider_valueChanged(int value);
  void changeBulb(QLabel *label, int arg1);

private:
  Ui::MainWindow *ui;
  QTimer *timer = new QTimer();
  int counter = 0;
  int ShiningSpeed = 1;
};
#endif // MAINWINDOW_H
