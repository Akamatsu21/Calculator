#include "maincalculatorwindow.h"
#include "calculator.h"
#include "QMenuBar"
#include "QCloseEvent"

MainCalculatorWindow::MainCalculatorWindow(QWidget *parent) : QMainWindow(parent)
{
    calc = new Calculator;

    OptionsMenu = this->menuBar()->addMenu("Advanced options");
    OptionsMenu->addAction(calc->ConstantsAct);
    OptionsMenu->addAction(calc->FunctionsAct);
    OptionsMenu = this->menuBar()->addMenu("Help");
    OptionsMenu->addAction(calc->HelpAct);
    OptionsMenu->addAction(calc->ChangeAct);

    setCentralWidget(calc);
    adjustSize();
    setFixedSize(size());
}

MainCalculatorWindow::~MainCalculatorWindow()
{
}

void MainCalculatorWindow::closeEvent(QCloseEvent *event)
{
    if(calc->ConstantsWindow->isVisible()) calc->ConstantsWindow->close();
    if(calc->FunctionsWindow->isVisible()) calc->FunctionsWindow->close();
    if(calc->FileWindow->isVisible()) calc->FileWindow->close();
    event->accept();
}
