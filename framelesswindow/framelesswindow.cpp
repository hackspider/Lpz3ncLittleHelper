/*
###############################################################################
#                                                                             #
# The MIT License                                                             #
#                                                                             #
# Copyright (C) 2017 by Juergen Skrotzky (JorgenVikingGod@gmail.com)          #
#               >> https://github.com/Jorgen-VikingGod                        #
#                                                                             #
# Sources: https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle  #
#                                                                             #
###############################################################################
*/

#include <QGraphicsDropShadowEffect>
#include "framelesswindow.h"
#include "ui_framelesswindow.h"

FramelessWindow::FramelessWindow(QWidget *parent): QDialog(parent), ui(new Ui::FramelessWindow)
{
  ui->setupUi(this);

  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
  // append minimize button flag in case of windows,
  // for correct windows native handling of minimize function
#if defined(Q_OS_WIN)
  setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
#endif
  setAttribute(Qt::WA_TranslucentBackground);

  // add window shadow
  if (   QSysInfo::productType().toLower() != "windows"
     || (QSysInfo::productType().toLower() == "windows" && QSysInfo::productVersion().toInt() > 7) ) {

     //shadow under window title text
     QGraphicsDropShadowEffect *textShadow = new QGraphicsDropShadowEffect;
     textShadow->setBlurRadius(4.0);
     textShadow->setColor(QColor(0,0,0));
     textShadow->setOffset(0.0);
     ui->titleText->setGraphicsEffect(textShadow);

     //window shadow
     QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
     windowShadow->setBlurRadius(9.0);
     windowShadow->setColor(palette().color(QPalette::Highlight));
     windowShadow->setOffset(0.0);
     ui->windowFrame->setGraphicsEffect(windowShadow);
  }

  // watch mouse clicks on icon label and fire own signals
  MouseButtonSignaler signaler;
  signaler.installOn(ui->icon);
  QObject::connect(&signaler, &MouseButtonSignaler::mouseButtonEvent, [this](QWidget*, QMouseEvent * event) {
    if (event->type() == QEvent::MouseButtonPress) {
      QMouseEvent *mouse = static_cast<QMouseEvent*>(event);
      if (mouse->button() == Qt::LeftButton) {
        emit windowIconLeftClicked();
      } else if (mouse->button() == Qt::RightButton) {
        emit windowIconRightClicked();
      }
    } else if (event->type() == QEvent::MouseButtonDblClick) {
      emit windowIconDblClick();
    }
  });

  QObject::connect(qApp, &QGuiApplication::applicationStateChanged, this, &FramelessWindow::on_applicationStateChanged);
}

void FramelessWindow::changeEvent(QEvent *event)
{

    event->accept();
}

void FramelessWindow::setContent(QWidget *w)
{
  contentLayout.setMargin(0);
  contentLayout.addWidget(w);
  ui->windowContent->setLayout(&contentLayout);
}

void FramelessWindow::setContent(QDialog *w)
{
  contentLayout.setMargin(0);
  contentLayout.addWidget(w);
  ui->windowContent->setLayout(&contentLayout);
  connect(w,SIGNAL(finished(int)),this,SLOT(contentClosed(int)));
}

void FramelessWindow::setWindowTitle(const QString &text)
{
  ui->titleText->setText(text);
}

void FramelessWindow::setWindowIcon(const QIcon &ico)
{
  ui->icon->setPixmap(ico.pixmap(16,16));
}

void FramelessWindow::styleWindow(bool bActive, bool bNoState)
{
  if (bActive) {
    if (bNoState) {
      layout()->setMargin(15);
      ui->windowTitlebar->setStyleSheet("#windowTitlebar{border: 0px none palette(shadow); border-top-left-radius:5px; border-top-right-radius:5px; background-color:palette(shadow); height:20px;}");
      ui->windowFrame->setStyleSheet("#windowFrame{border:1px solid palette(highlight); border-radius:5px 5px 5px 5px; background-color:palette(Window);}");
      QGraphicsEffect *oldShadow = ui->windowFrame->graphicsEffect();
      if (oldShadow)
        delete oldShadow;
      QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
      windowShadow->setBlurRadius(9.0);
      windowShadow->setColor(palette().color(QPalette::Highlight));
      windowShadow->setOffset(0.0);
      ui->windowFrame->setGraphicsEffect(windowShadow);
    } else {
      layout()->setMargin(0);
      ui->windowTitlebar->setStyleSheet("#windowTitlebar{border: 0px none palette(shadow); border-top-left-radius:0px; border-top-right-radius:0px; background-color:palette(shadow); height:20px;}");
      ui->windowFrame->setStyleSheet("#windowFrame{border:1px solid palette(dark); border-radius:0px 0px 0px 0px; background-color:palette(Window);}");
      QGraphicsEffect *oldShadow = ui->windowFrame->graphicsEffect();
      if (oldShadow)
        delete oldShadow;
      ui->windowFrame->setGraphicsEffect(nullptr);
    } // if (bNoState) else maximize
  } else {
    if (bNoState) {
      layout()->setMargin(15);
      ui->windowTitlebar->setStyleSheet("#windowTitlebar{border: 0px none palette(shadow); border-top-left-radius:5px; border-top-right-radius:5px; background-color:palette(dark); height:20px;}");
      ui->windowFrame->setStyleSheet("#windowFrame{border:1px solid #000000; border-radius:5px 5px 5px 5px; background-color:palette(Window);}");
      QGraphicsEffect *oldShadow = ui->windowFrame->graphicsEffect();
      if (oldShadow)
        delete oldShadow;
      QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
      windowShadow->setBlurRadius(9.0);
      windowShadow->setColor(palette().color(QPalette::Shadow));
      windowShadow->setOffset(0.0);
      ui->windowFrame->setGraphicsEffect(windowShadow);
    } else {
      layout()->setMargin(0);
      ui->windowTitlebar->setStyleSheet("#titlebarWidget{border: 0px none palette(shadow); border-top-left-radius:0px; border-top-right-radius:0px; background-color:palette(dark); height:20px;}");
      ui->windowFrame->setStyleSheet("#windowFrame{border:1px solid palette(shadow); border-radius:0px 0px 0px 0px; background-color:palette(Window);}");
      QGraphicsEffect *oldShadow = ui->windowFrame->graphicsEffect();
      if (oldShadow)
        delete oldShadow;
      ui->windowFrame->setGraphicsEffect(nullptr);
    } // if (bNoState) { else maximize
  } // if (bActive) { else no focus
}

void FramelessWindow::on_applicationStateChanged(Qt::ApplicationState state)
{
  if (windowState().testFlag(Qt::WindowNoState)) {
    if (state == Qt::ApplicationActive) {
      styleWindow(true, true);
    } else {
      styleWindow(false, true);
    }
  }
}

void FramelessWindow::on_minimizeButton_clicked()
{
  setWindowState(Qt::WindowMinimized);
}

void FramelessWindow::on_closeButton_clicked()
{
  close();
}

void FramelessWindow::on_windowTitlebar_doubleClicked()
{

}

void FramelessWindow::contentClosed(int result)
{
    setResult(result);
    close();
}
