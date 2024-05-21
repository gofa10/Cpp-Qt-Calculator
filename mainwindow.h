#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include <QFont>

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


 signals:
 private slots:
  void on_showAgvancedAction_toggled(bool arg1);

  void on_showTrigAction_toggled(bool arg1);

  void numberPanel();
  void operations();
  // void on_seven_clicked();
  void mathOperationsPanel();
  void advancedPanel();
  void trigPanel();

private:
  // Методы
  void setColor();
  double degToRad();

  int fact(int n);

  void evaluateResult(double a, double b, QString sign);
  void evaluatePriorityMode();
  void evalMe();

  void changeNumberPanelAvailability();

  // Свойства
  Ui::MainWindow *ui;

  unsigned short i;
  int appHeight, appWidth, appWidthOneEnabled, appWidthAllEnabled;
  int precision;

  double num_first, num_second, num_third;

  QPushButton *it;

  bool isAdvancedChecked;
  bool isTrigChecked;
  bool isDarkTheme;
  bool isRad;
  bool isMathOperation, isEvalMode, isPriorityMode;
  bool isNumberPanelEnabled;

  QFont primaryTextFont;

  QString mathOperationSign, mathOperationRes;

  QString displayBackColor;
  QString onDisplayBackColor;
  QString displayTextColor;
  QString keyboardBackColor;
  QString controlsBackColor;

  QString arthOpsStyle, pressedArthOpsStyle;
  QString arthOpsBackColor, arthOpsTextColor, pressedArthOpsBackColor, pressedArthOpsTextColor, hoveredArthOpsTextColor;
  QPushButton* lastPressedArthOps;

  QString controlsTextColor;

  QString evalButtonBackColor, evalButtonTextColor, hoveredEvalBackColor;

  QString numsStyle;
  QString advancedMathStyle;


};
#endif  // MAINWINDOW_H
