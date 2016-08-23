#ifndef MAINCALCULATORWINDOW_H
#define MAINCALCULATORWINDOW_H

#include "QMainWindow"

class Calculator;

class MainCalculatorWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainCalculatorWindow(QWidget *parent = 0);
    ~MainCalculatorWindow();

signals:

public slots:

private:
    void closeEvent(QCloseEvent *event);

    Calculator *calc;

    QMenu *OptionsMenu;
};

#endif // MAINCALCULATORWINDOW_H
