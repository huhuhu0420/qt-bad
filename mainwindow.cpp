#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(bulr()));
  ui->speedSlider->setRange(0, 100);

  QPixmap dark = QPixmap("../micro/bulb_dark.png");
  ui->label_1->setPixmap(dark);
  ui->label_2->setPixmap(dark);
  ui->label_3->setPixmap(dark);
  ui->label_4->setPixmap(dark);

  //   ui->speedSlider->installEventFilter(this);
}
bool MainWindow::event(QEvent *event) {
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = (QKeyEvent *)event;
    if (keyEvent->key() == 'S' && ui->speedSlider->sliderPosition() <= 100) {
      ui->speedSlider->setSliderPosition(ui->speedSlider->sliderPosition() + 1);
    } else if (keyEvent->key() == 'A' &&
               ui->speedSlider->sliderPosition() >= 0) {
      ui->speedSlider->setSliderPosition(ui->speedSlider->sliderPosition() - 1);
    }
  }
  return QMainWindow::event(event);
}
// bool MainWindow::eventFilter(QObject* watched, QEvent* event)
//{
//     if (event->type() == QEvent::KeyPress )
//     {
//         QKeyEvent* keyEvent = (QKeyEvent*)event;
//         if (keyEvent->key()=='S'&&ui->speedSlider->sliderPosition()<=100)
//         {
//             ui->speedSlider->setSliderPosition(ui->speedSlider->sliderPosition()+1);
//         }
//         else if (keyEvent->key()=='A'&&ui->speedSlider->sliderPosition()>=0)
//         {
//             ui->speedSlider->setSliderPosition(ui->speedSlider->sliderPosition()-1);
//         }

//    }
//    return QMainWindow::eventFilter(watched, event);
//}

MainWindow::~MainWindow() { delete ui; }

int gpio_export(unsigned int gpio) {
  int fd, len;
  char buf[64];

  fd = open("/sys/class/gpio/export", O_WRONLY);
  if (fd < 0) {
    perror("gpio/export");
    return fd;
  }
  len = snprintf(buf, sizeof(buf), "%d", gpio);
  write(fd, buf, len);
  close(fd);

  return 0;
}

int gpio_unexport(unsigned int gpio) {
  int fd, len;
  char buf[64];

  fd = open("/sys/class/gpio/unexprot", O_WRONLY);
  if (fd < 0) {
    perror("gpio/export");
    return fd;
  }

  len = snprintf(buf, sizeof(buf), "%d", gpio);
  write(fd, buf, len);
  close(fd);
  return 0;
}

int gpio_set_dir(unsigned int gpio, std::string dirStatus) {
  int fd;
  char buf[64];
  printf("%d\n", gpio);

  snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", gpio);

  fd = open(buf, O_WRONLY);
  if (fd < 0) {
    perror("gpio/direction");
    return fd;
  }
  if (dirStatus == "out") {
    write(fd, "out", 4);
  } else {
    write(fd, "in", 3);
  }
  close(fd);
  return 0;
}

int gpio_set_value(unsigned int gpio, int value) {
  int fd;
  char buf[64];

  snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);

  fd = open(buf, O_WRONLY);
  if (fd < 0) {
    perror("gpio/set-value");
    return fd;
  }
  if (value == 0) {
    write(fd, "0", 2);
  } else {
    write(fd, "1", 2);
  }
  close(fd);
  return 0;
}
void MainWindow::changeBulb(QLabel *label, int arg1) {

  QPixmap pixmap = QPixmap("../micro/bulb_light.png");

  QPixmap dark = QPixmap("../micro/bulb_dark.png");
  if (arg1 != 0) {
    // ui->label_1->setPixmap(pixmap);
    label->setPixmap(pixmap);
  } else {
    label->setPixmap(dark);
  }
  std::map<QLabel *, int> led = {
      {ui->label_1, 255},
      {ui->label_2, 396},
      {ui->label_3, 429},
      {ui->label_4, 398},
  };
  std::map<std::string, int> power = {{"on", 1}, {"off", 0}};

  std::string power_index = "on";

  if (arg1 != 0) {
    power_index = "on";
  } else {
    power_index = "off";
  }

  int ledCode = led[label];
  int powerCode = power[power_index];
  gpio_export(ledCode);
  gpio_set_dir(ledCode, "out");
  gpio_set_value(ledCode, powerCode);
}
void MainWindow::on_checkBox_stateChanged(int arg1) {
  //    changeBulb(ui->label_1,arg1);
}

void MainWindow::on_checkBox_2_stateChanged(int arg1) {
  // changeBulb(ui->label_2,arg1);
}

void MainWindow::on_checkBox_3_stateChanged(int arg1) {
  // changeBulb(ui->label_3,arg1);
}

void MainWindow::on_checkBox_4_stateChanged(int arg1) {
  // changeBulb(ui->label_4,arg1);
}

void MainWindow::bulr() {
  counter = counter - 1;
  if (counter <= 0) {
    timer->stop();
  } else {
    // timer->start(1000);
  }
  Qt::CheckState t = Qt::CheckState(1);
  Qt::CheckState f = Qt::CheckState(0);

  if (counter % 2 == 0) {

    changeBulb(ui->label_1, 1);
    changeBulb(ui->label_2, 1);
    changeBulb(ui->label_3, 0);
    changeBulb(ui->label_4, 0);

    ui->checkBox->setCheckState(t);
    ui->checkBox_2->setCheckState(t);
    ui->checkBox_3->setCheckState(f);
    ui->checkBox_4->setCheckState(f);

  } else {

    changeBulb(ui->label_1, 0);
    changeBulb(ui->label_2, 0);
    changeBulb(ui->label_3, 1);
    changeBulb(ui->label_4, 1);

    ui->checkBox->setCheckState(f);
    ui->checkBox_2->setCheckState(f);
    ui->checkBox_3->setCheckState(t);
    ui->checkBox_4->setCheckState(t);
  }
}

void MainWindow::on_shining_clicked() {
  int check1 = ui->checkBox->checkState();
  changeBulb(ui->label_1, check1);
  int check2 = ui->checkBox_2->checkState();
  changeBulb(ui->label_2, check2);
  int check3 = ui->checkBox_3->checkState();
  changeBulb(ui->label_3, check3);
  int check4 = ui->checkBox_4->checkState();
  changeBulb(ui->label_4, check4);
}

void MainWindow::on_dump_clicked() {

  //    changeBulb(ui->label_1,1);
  //    changeBulb(ui->label_2,1);
  //    changeBulb(ui->label_3,0);
  //    changeBulb(ui->label_4,0);
  //    Qt::CheckState t = Qt::CheckState(1);
  //    Qt::CheckState f = Qt::CheckState(0);

  //    ui->checkBox->setCheckState(t);
  //    ui->checkBox_2->setCheckState(t);
  //    ui->checkBox_3->setCheckState(f);
  //    ui->checkBox_4->setCheckState(f);

  int target = ui->spinBox->value();
  counter = target * 2;

  timer->start(ShiningSpeed + 1); // avoid 0 case
}

void MainWindow::changeSwitchSpeed(int speed) {
  QString s = ">=100";
  if (speed <= 0) {
    s = "<=0";
  } else if (speed < 50) {
    s = "<50";
  } else if (speed < 100) {
    s = "<100";
  }
  ui->cout->setText(s);
  ui->speedBar->setValue(speed);
  ShiningSpeed = speed * 5;
}

void MainWindow::on_speedBar_valueChanged(int value) {
  changeSwitchSpeed(value);
}

void MainWindow::on_speedSlider_sliderMoved(int position) {
  changeSwitchSpeed(position);
}

void MainWindow::on_pushButton_clicked() { timer->stop(); }

void MainWindow::on_speedSlider_valueChanged(int value) {
  changeSwitchSpeed(value);
}
