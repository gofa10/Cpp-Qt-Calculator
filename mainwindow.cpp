#include "mainwindow.h"

#include <QObject>
#include <QtMath>

#include <QDebug>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)

  , precision{12}

  , num_first{0.0}
  , num_second{0.0}
  , num_third{1.0}

  , isDarkTheme{false}
  , isTrigChecked{false}
  , isAdvancedChecked{false}
  , isRad{false}
  , isMathOperation{false}

      , isEvalMode{false}, isPriorityMode{false}

  , isNumberPanelEnabled{true}

  , lastPressedArthOps{nullptr}

  , appHeight{491}
  , appWidth{303}
  , appWidthOneEnabled{372}
  , appWidthAllEnabled{441}
{
  ui->setupUi(this);
  ui->alg->hide();
  ui->trigonometry->hide();

  setColor();

  this->setFixedSize(appWidth, appHeight);

  this->setWindowTitle("Калькулятор");
  this->setStyleSheet("QLineEdit { padding-right: 12px; padding-left: 12px ; border: 0px ; background: " + displayBackColor + " ; }"
                      "QFrame { border: 0px; }"
                      "QFrame#display { background-color: "
                      + displayBackColor
                      + "; }"
                        "QFrame#keyboard { background-color: "
                      + keyboardBackColor
                      + "; }"
                        "QPushButton { border-radius: 30px; }"
                        "QPushButton:pressed { border-radius: 18px; }");
  // Раскрашиваю кнопки, отобр. арифм операции ---
  arthOpsStyle = "QPushButton { background-color: " + arthOpsBackColor + "; color: " + arthOpsTextColor +
  "; }"
                 "QPushButton:hover { background-color: " + pressedArthOpsBackColor + "; color: " + arthOpsBackColor + " ; }";

  pressedArthOpsStyle = "QPushButton { background-color: " + pressedArthOpsBackColor + "; color: " + arthOpsBackColor +
                        "; }"
                 "QPushButton:hover { background-color: " + pressedArthOpsBackColor + "; color: " + arthOpsBackColor + " ; border: none }"
      ;


  for (i = 0; i < ui->mainOperations->layout()->count() - 1; ++i) {
    it = qobject_cast<QPushButton *>(ui->mainOperations->layout()->itemAt(i)->widget());
    connect(it, &QPushButton::clicked, this, &MainWindow::mathOperationsPanel);
    it->setStyleSheet(arthOpsStyle);
  }
  for (i = 0; i < ui->controls->count(); ++i) {
      it = qobject_cast<QPushButton *>(ui->controls->itemAt(i)->widget());
      connect(it, &QPushButton::clicked, this, &MainWindow::operations);
      it->setStyleSheet(arthOpsStyle);
  }
  ui->pow->setStyleSheet(arthOpsStyle);
  // Кнопка = (eval)
  ui->eval->setStyleSheet(
      "QPushButton { background-color: " + evalButtonBackColor + "; color:  "  + evalButtonTextColor + "; }"
      "QPushButton:hover { background-color: "
      + hoveredEvalBackColor + "; color: " + evalButtonTextColor + "}");
  connect(ui->eval, &QPushButton::clicked, this, &MainWindow::mathOperationsPanel);
  // Возведение в степень
  connect(ui->pow, &QPushButton::clicked, this, &MainWindow::mathOperationsPanel);
  // Раскрашиваю кнопки, отобр цифры и изв мат. постоянные
  numsStyle = "QPushButton { background-color: " + displayBackColor
              + " ; color: " + displayTextColor
              + " ; }"
                "QPushButton:hover { background-color: "
              + onDisplayBackColor + " ; }";
  for (i = 0; i < ui->nums->count(); ++i) {
    it = qobject_cast<QPushButton *>(ui->nums->itemAt(i)->widget());
    connect(it, &QPushButton::clicked, this, &MainWindow::numberPanel);
    it->setStyleSheet(numsStyle);
  }
  ui->pi->setStyleSheet(numsStyle);
  connect(ui->pi, &QPushButton::clicked, this, &MainWindow::numberPanel);
  ui->e->setStyleSheet(numsStyle);
  connect(ui->e, &QPushButton::clicked, this, &MainWindow::numberPanel);
  // Алгебра и тригонометрия
  advancedMathStyle = "QPushButton { background-color: rgb(199, 236, 203); color: rgb(1, 33, "
                      "13); }"
                      "QPushButton:hover { background-color: rgb(171, 207, 176); }";
  for (i = 0; i < ui->trigonometry->layout()->count() - 1; ++i) {
      it = qobject_cast<QPushButton *>(ui->trigonometry->layout()->itemAt(i)->widget());
      connect(it, &QPushButton::clicked, this, &MainWindow::trigPanel);
      it->setStyleSheet(advancedMathStyle);
  }
  for (i = 1; i < ui->alg->layout()->count() - 1; ++i) {
      it = qobject_cast<QPushButton *>(ui->alg->layout()->itemAt(i)->widget());
      it->setStyleSheet(advancedMathStyle);
  }

  // Настраиваю шрифт
  primaryTextFont = QFont("Inter", 36);
  ui->primaryText->setFont(primaryTextFont);
  // ---

  connect(ui->log, &QPushButton::clicked, this, &MainWindow::advancedPanel);
  connect(ui->fact, &QPushButton::clicked, this, &MainWindow::advancedPanel);
  connect(ui->sqrt, &QPushButton::clicked, this, &MainWindow::advancedPanel);

  ui->secondaryText->setText("");

  ui->indicator->setText("ГРАД");

  // ---
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setColor()
{
    if (isDarkTheme) {
        // Темная тема
        displayBackColor = "rgb(61, 57, 47)";
        displayTextColor = "rgb(233,226,212)";
        keyboardBackColor = "rgb(22 , 19 , 11 )";
        controlsBackColor = "rgb(205, 232, 224)";
        controlsTextColor = "rgb(6, 32, 27)";
    } else {
        // Светлая тема
        displayBackColor = "rgb(225, 217, 204)";
        onDisplayBackColor = "rgb(195, 185, 162)";
        displayTextColor = "rgb(30, 27, 19)";
        keyboardBackColor = "rgb(255, 248, 240)";
        controlsBackColor = "rgb(177, 204, 196)";
        controlsTextColor = "rgb(51, 75, 70)";

        evalButtonTextColor = "rgb(113, 92, 13)";
        evalButtonBackColor = "rgb(253, 225, 134)";
        hoveredEvalBackColor = "rgb(224, 197, 109)";

        arthOpsBackColor = "rgb(255, 217, 226)";
        arthOpsTextColor = "rgb(49, 19, 28)";
        pressedArthOpsBackColor = "rgb(150, 103, 117)";
        pressedArthOpsTextColor = "rgb(224, 197, 109)";
        hoveredArthOpsTextColor = "rgb(245, 249, 237)";
    }
}

void MainWindow::on_showAgvancedAction_toggled(bool arg1)
{
    if (arg1) {
        isAdvancedChecked = true;
        ui->alg->show();
        if (isTrigChecked) {
            this->setFixedSize(appWidthAllEnabled, appHeight);
        } else {
            this->setFixedSize(appWidthOneEnabled, appHeight);
        }
    } else {
        isAdvancedChecked = false;
        ui->alg->hide();
        if (!isTrigChecked) {
            this->setFixedSize(appWidth, appHeight);
        } else {
            this->setFixedSize(appWidthOneEnabled, appHeight);
        }
    }
}

void MainWindow::on_showTrigAction_toggled(bool arg1)
{
    if (arg1) {
        isTrigChecked = true;
        ui->trigonometry->show();
        if (isAdvancedChecked) {
            this->setFixedSize(appWidthAllEnabled, appHeight);
        } else {
            this->setFixedSize(appWidthOneEnabled, appHeight);
        }
    } else {
        isTrigChecked = false;
        ui->trigonometry->hide();
        if (!isAdvancedChecked) {
            this->setFixedSize(appWidth, appHeight);
        } else {
            this->setFixedSize(appWidthOneEnabled, appHeight);
        }
    }
}

void MainWindow::changeNumberPanelAvailability() {
  for (int i = 1; i < ui->nums->count(); ++i) {
    qobject_cast<QPushButton*>(ui->nums->itemAt(i)->widget())->setEnabled(isNumberPanelEnabled);
  }
}

void MainWindow::numberPanel()
{
  // all_numbers :: введенные числа
  // new_label :: строка, которая отображается на основном дисплее
  QPushButton *button = (QPushButton *)sender();
  double all_numbers;
  QString new_label = ui->primaryText->text();
  if (button == ui->del) {
    if (isMathOperation) {

      if (lastPressedArthOps) {
        lastPressedArthOps->setStyleSheet(arthOpsStyle);
        lastPressedArthOps = nullptr;
      }

      new_label = QString::number(num_first, 'g', precision);

      ui->del->setText("AC");
      isMathOperation = false;

    } else {
      ui->del->setText("AC");
      ui->dot->setEnabled(true);

      primaryTextFont.setPointSize(32);
      ui->primaryText->setFont(primaryTextFont);

      new_label = "0";

      if (!isNumberPanelEnabled) {
        isNumberPanelEnabled = true;
        changeNumberPanelAvailability();
      }

    }
    isEvalMode = false;
    ui->secondaryText->setText("");
  } else if (button == ui->pi || button == ui->e) {
    new_label = (button->text() == "e" ? QString::number(M_E, 'g', precision) : QString::number(M_PI, 'g', precision));

    primaryTextFont.setPointSize(28);
    ui->primaryText->setFont(primaryTextFont);

    if (!isNumberPanelEnabled) {
      isNumberPanelEnabled = true;
      changeNumberPanelAvailability();
    }

  } else {
    if (button->text() == ".") {
      new_label = (ui->primaryText->text().contains(".") ? ui->primaryText->text() : ui->primaryText->text() + button->text());
      ui->dot->setEnabled(false);
    } else {
      if (ui->primaryText->text().length() < ( ui->dot->isEnabled() ? 12 : 13 )) {
        new_label = (ui->primaryText->text() == "0" || ui->primaryText->text() == "π" || ui->primaryText->text() == "e" ? button->text() : ui->primaryText->text() + button->text());
        if (ui->primaryText->text().length() == 10) {
          // уменьшаем размер шрифта
          primaryTextFont.setPointSize(28);
          ui->primaryText->setFont(primaryTextFont);
        }
      } else {
        isNumberPanelEnabled = true;
        changeNumberPanelAvailability();
      }
    }
  }
  ui->primaryText->setText(new_label);
}

void MainWindow::evaluateResult(double a, double b, QString sign) {

  if (sign == "+") {
    mathOperationRes = QString::number(a + b, 'g', precision);
  } else if (sign == "-") {
    mathOperationRes = QString::number(a - b, 'g', precision);
  } else if (sign == "×") {
    mathOperationRes = QString::number(a * b, 'g', precision);
  } else if (sign == "/") {
    if (b) {
      mathOperationRes = QString::number(a / b, 'g', precision);
    } else {
      mathOperationRes = "nan";
    }
  } else if (sign == "^") {
    mathOperationRes = QString::number(qPow(a, b), 'g', precision);
  }


}

void MainWindow::evaluatePriorityMode() {

  if (
    (mathOperationSign == "/" && lastPressedArthOps->text() == "×") ||
    (mathOperationSign == "×" && lastPressedArthOps->text() == "/")
  )
  {
    evaluateResult(num_first, num_second, mathOperationSign);
    evaluateResult(mathOperationRes.toDouble(), ui->primaryText->text().toDouble(), lastPressedArthOps->text());

  } else {
    evaluateResult(num_second, ui->primaryText->text().toDouble(), lastPressedArthOps->text());
    evaluateResult(num_first, mathOperationRes.toDouble(), mathOperationSign);
  }


  lastPressedArthOps->setStyleSheet(arthOpsStyle);
  lastPressedArthOps = nullptr;
  isPriorityMode = false;

}

void MainWindow::evalMe() {

  if (isPriorityMode) {
    evaluatePriorityMode();
    num_first = mathOperationRes.toDouble();
    ui->secondaryText->setText(ui->secondaryText->text() + " " + ui->primaryText->text() +  " =");
  } else {
    num_second = ui->primaryText->text().toDouble();
    evaluateResult(num_first, num_second, mathOperationSign);
    ui->secondaryText->setText(QString::number(num_first) + mathOperationSign + ui->primaryText->text() + " =");
    num_first = mathOperationRes.toDouble();
  }

  ui->primaryText->setText(mathOperationRes);
  isMathOperation = false;

}

void MainWindow::mathOperationsPanel()
{
  QPushButton* button = (QPushButton *)sender();

  // ---
  // Если у нас выбрана хотябы одна из операций, то мы при нажатии на eval ее выполняем, иначе не делаем ничего.
  // При повторном нажатии на знак операции он должен вести себя как "="
  // При повторном нажатии на знак "=" в левым операндом должен стать результат операции.
  // Мат Операция должна заканчиваться только в случае если либо нажата "DEL", либо введено обычное число.
  if (isMathOperation) {
    // Повторное нажатие на знак математической операции
    // Низкий приоритет
    isEvalMode = (button->text() == mathOperationSign ? true : false);

    if (button == ui->plus || button == ui->minus ) {

      if (isEvalMode) {

        evalMe();
      // Нажата эта кнопка и до этого было введено выражение с приоритетом мат операций.
      } else if (isPriorityMode) {

        //num_second = ui->primaryText->text().toDouble();
        evaluatePriorityMode();
        num_first = mathOperationRes.toDouble();
        mathOperationSign = button->text();

        ui->secondaryText->setText(mathOperationRes + " " + mathOperationSign + " ");
        ui->primaryText->setText("0");
      // Поменяли минус на плюс например.
      } else {
        mathOperationSign = button->text();
        ui->primaryText->setText("0");
        ui->secondaryText->setText(mathOperationRes + " " + mathOperationSign + " ");

        num_first = mathOperationRes.toDouble();

      }

    // Высокий приоритет
    } else if (button == ui->mult || button == ui->div || button == ui->pow) {

      if (isEvalMode) {

        evalMe();

      // Если выбрал деление и до этого было умножение
      } else if (isPriorityMode) {

        evaluatePriorityMode();
        mathOperationSign = button->text();

        ui->secondaryText->setText(mathOperationRes + " " + mathOperationSign + " ");
        ui->primaryText->setText("0");
      // До этого было сложение или вычитание
      } else {

        button->setStyleSheet(pressedArthOpsStyle);
        lastPressedArthOps = button;

        num_second = ui->primaryText->text().toDouble();
        ui->secondaryText->setText(QString::number(num_first, 'g', precision) + " " + mathOperationSign + " " + ui->primaryText->text() + " " + button->text());
        ui->primaryText->setText("0");

        isPriorityMode = true;
      }

    } else if (button == ui->eval) {

      evalMe();

    }
    // Первое нажатие на знак математической операции
  } else {

    if (ui->primaryText->text() == "π") {
      num_first = M_PI;
    } else if (ui->primaryText->text() == "e") {
      num_first = M_E;
    } else {
      num_first = ui->primaryText->text().toDouble();
    }

    if (button == ui->plus || button == ui->minus || button == ui->mult || button == ui->div || button == ui->pow) {
      // Прописать логику для повторного нажатия на ту же операцию

      mathOperationSign = button->text();

      ui->primaryText->setText("0");

      ui->secondaryText->setText(QString::number(num_first, 'g', precision) + " " + mathOperationSign + " ");

      ui->del->setText("C");
      isMathOperation = true;

    } else if (button == ui->eval) { // Оставляет тоже число на дисплее.
      ui->primaryText->setText(QString::number(num_first, 'g', precision));
    }

    if (!isNumberPanelEnabled) {
        isNumberPanelEnabled = true;
        changeNumberPanelAvailability();
    }

  }
}

int MainWindow::fact(int n) {
  if ((n==0)||(n==1))
    return 1;
  else
    return n*fact(n-1);
}

void MainWindow::advancedPanel() {
  QPushButton *button = (QPushButton *)sender();

  if (isMathOperation) {
    evalMe();
  } else {
    mathOperationRes = ui->primaryText->text();
    num_first = mathOperationRes.toDouble();
  }

  double res;

  if (button == ui->sqrt) {

    res = qSqrt(num_first);
    ui->secondaryText->setText("sqrt(" + mathOperationRes + ") =");

  } else if (button == ui->fact) {

    res = fact(num_first);
    ui->secondaryText->setText(mathOperationRes + "! =");

  } else if (button == ui->log) {

    res = qLn(num_first);
    ui->secondaryText->setText("ln("+ ui->primaryText->text() + ") =");

  }

  ui->primaryText->setText(QString::number(res, 'g', precision));

}

void MainWindow::operations()
{
  QPushButton* button = (QPushButton*)sender();

  double all_numbers;

  if(button == ui->changeSign) {
    all_numbers = (ui->primaryText->text().toDouble());
    all_numbers = all_numbers * -1;
    QString new_label = QString::number(all_numbers, 'g', 12);
    ui->primaryText->setText(new_label);
  } else if(button == ui->percent) {
    all_numbers = (ui->primaryText->text().toDouble());
    all_numbers = all_numbers * 0.01;
    QString new_label = QString::number(all_numbers, 'g', 12);
    ui->primaryText->setText(new_label);
  } else if (button == ui->toRad) {
    if (!isRad) {
      isRad = true;
      ui->toRad->setText("ГРАД");
      ui->indicator->setText("РАД");

    } else {
      isRad = false;
      ui->toRad->setText("РАД");
      ui->indicator->setText("ГРАД");
    }
  }
}

double MainWindow::degToRad() {
  return (isRad ? num_first : qDegreesToRadians(num_first));
}

void MainWindow::trigPanel() {
  QPushButton* button = (QPushButton*)sender();

  if (isMathOperation) {
    evalMe();
  } else {
    mathOperationRes = ui->primaryText->text();
    num_first = ui->primaryText->text().toDouble();
  }
  double res = 0.0; QString funcSign;

  if (button == ui->sin) {

    res = qSin(degToRad());
    funcSign = "sin(";

  } else if (button == ui->cos) {

    res = qCos(degToRad());
    funcSign = "cos(";

  } else if (button == ui->tg) {

    res = qTan(degToRad());
    funcSign = "tg(";

  } else if (button == ui->ctg) {

    res = 1.0 / qTan(degToRad());
    funcSign = "ctg(";

  }

  num_first = res;
  ui->secondaryText->setText(funcSign + mathOperationRes + ") =");
  ui->primaryText->setText(
    (res > qPow(10, -10) ? QString::number(res, 'g', precision) : "0")
  );
}


