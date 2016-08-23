#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <map>
#include <stack>

class QToolButton;
class QLineEdit;
class QLabel;
class QFile;
class QTextEdit;
/*
namespace Ui {
class Calculator;
}
*/
class Calculator : public QWidget
{
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = 0);
    ~Calculator();

    enum InputType
    {
        NotOperator,
        UnaryOperator,
        BinaryOperator
    };

    enum Logic
    {
        _false,
        _last,
        _true
    };

    enum Error
    {
        NoError,
        BadEquation,
        DivZero,
        MissingBrackets,
        NegSqrt,
        Log1
    };

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void AddMemory();
    void BackspaceClicked();
    void Clean();
    void ConstantClicked();
    void DisplayMemory();
    void EqualsClicked();
    void NumberClicked();
    void OpenChange();
    void OpenConstants();
    void OpenFunctions();
    void OpenHelp();
    void OperatorClicked();
    void ResetCursor();
    void ResetMemory();
    void SubtractMemory();
    void TextChanged(const QString &text);

private:
    friend class MainCalculatorWindow;

    QString CalculateRPN(QString RPN);
    QToolButton *CreateButton(QString text, const char *ButtonSlot, int w = 35, int h = 30);
    void ErrorOccured();
    int FindBracketPos(QString s);
    bool IsNumerical(QString s);
    bool IsOperator(QString s);
    Logic IsPart(QString s);
    QString MakeRPN(QString given);
    void OpenFile(QString s);
    void SetupActions();
    QString StackToString(std::stack<QString> &container);
    double TakeNumber(std::stack<QString> &stack);
    void TextUpdate(QString newText);

    QAction *ChangeAct;
    QAction *ConstantsAct;
    QWidget *ConstantsWindow;
    QLineEdit *display;
    QLabel *displayedResult;
    QString DisplayString;
    QAction *HelpAct;
    QTextEdit *FileDisplay;
    QWidget *FileWindow;
    QAction *FunctionsAct;
    QWidget *FunctionsWindow;
    Error GlobalError;
    double GlobalMemory;
    std::map<QString, InputType> iType;
    bool NewEquation;
    QString NotationString;
    std::map<QString, int> Rank;

    const double PI;
};

#endif // CALCULATOR_H
