#include <iostream>
#include <cmath>
#include "calculator.h"
#include "QToolButton"
#include "QLineEdit"
#include "QGridLayout"
#include "QLabel"
#include "QAction"
#include "QKeyEvent"
#include "QFile"
#include "QTextEdit"
#include <QTextStream>

Calculator::Calculator(QWidget *parent) :
    QWidget(parent), PI(3.1415926535897932384626433)
{
    DisplayString = "0";
    NotationString = "";
    NewEquation = true;
    GlobalError = NoError;
    GlobalMemory = 0;

    Rank["+"] = 1;
    Rank["-"] = 1;
    Rank["*"] = 2;
    Rank["/"] = 2;
    Rank["^"] = 3;
    Rank["log"] = 4;
    Rank["ln"] = 4;
    Rank["sin"] = 4;
    Rank["cos"] = 4;
    Rank["tg"] = 4;
    Rank["asin"] = 4;
    Rank["acos"] = 4;
    Rank["atg"] = 4;
    Rank["sqrt"] = 4;
    Rank["exp"] = 4;
    Rank["nlog"] = 5;
    Rank["nroot"] = 5;

    iType["+"] = BinaryOperator;
    iType["-"] = BinaryOperator;
    iType["*"] = BinaryOperator;
    iType["/"] = BinaryOperator;
    iType[")"] = UnaryOperator;
    iType["("] = UnaryOperator;
    iType["^"] = BinaryOperator;
    iType["log"] = UnaryOperator;
    iType["ln"] = UnaryOperator;
    iType["sin"] = UnaryOperator;
    iType["cos"] = UnaryOperator;
    iType["tg"] = UnaryOperator;
    iType["asin"] = UnaryOperator;
    iType["acos"] = UnaryOperator;
    iType["atg"] = UnaryOperator;
    iType["sqrt"] = UnaryOperator;
    iType["exp"] = UnaryOperator;
    iType["nlog"] = BinaryOperator;
    iType["nroot"] = BinaryOperator;

    //Text Line
    display = new QLineEdit(DisplayString);
    display->setMaxLength(81);
    display->setAlignment(Qt::AlignRight);
    display->setDragEnabled(false);
    display->setFixedHeight(25);
    QFont font = display->font();
    font.setPointSize(font.pointSize() * 1.5);
    display->setFont(font);
    connect(display, SIGNAL(textEdited(const QString &)), this, SLOT(TextChanged(const QString &)));
    connect(display, SIGNAL(returnPressed()), this, SLOT(EqualsClicked()));
    connect(display,SIGNAL(cursorPositionChanged(int, int)), this, SLOT(ResetCursor()));

    //Label
    displayedResult = new QLabel("");
    displayedResult->setAlignment(Qt::AlignRight);
    displayedResult->setFixedHeight(40);
    displayedResult->setFixedWidth(280);
    font = displayedResult->font();
    font.setPointSize(font.pointSize() * 3);
    displayedResult->setFont(font);
    //Digit buttons
    QToolButton *NumberButton[10];
    NumberButton[0] = CreateButton(QString::number(0), SLOT(NumberClicked()), 0, 0);
    for(int i = 1; i < 10; i++)
    {
        NumberButton[i] = CreateButton(QString::number(i), SLOT(NumberClicked()));
    }

    //Operator buttons
    QToolButton *AddButton = CreateButton("+", SLOT(OperatorClicked()));
    QToolButton *SubtractButton = CreateButton("-", SLOT(OperatorClicked()));
    QToolButton *MultiplyButton = CreateButton("*", SLOT(OperatorClicked()));
    QToolButton *DivideButton = CreateButton("/", SLOT(OperatorClicked()));
    QToolButton *ExponentButton = CreateButton("^", SLOT(OperatorClicked()));
    QToolButton *PointButton = CreateButton(".", SLOT(NumberClicked()));
    QToolButton *LogButton = CreateButton("log", SLOT(OperatorClicked()));
    QToolButton *LnButton = CreateButton("ln", SLOT(OperatorClicked()));
    QToolButton *SqrtButton = CreateButton("sqrt", SLOT(OperatorClicked()));
    QToolButton *SinButton = CreateButton("sin", SLOT(OperatorClicked()));
    QToolButton *CosButton = CreateButton("cos", SLOT(OperatorClicked()));
    QToolButton *TanButton = CreateButton("tg", SLOT(OperatorClicked()));
    QToolButton *BackspaceButton = CreateButton("◄", SLOT(BackspaceClicked()));
    QToolButton *OpenBracketButton = CreateButton("(", SLOT(OperatorClicked()));
    QToolButton *ClosedBracketButton = CreateButton(")", SLOT(OperatorClicked()));
    QToolButton *EqualsButton = CreateButton("=", SLOT(EqualsClicked()));
    QToolButton *AddMemButton = CreateButton("M+", SLOT(AddMemory()));
    QToolButton *SubMemButton = CreateButton("M-", SLOT(SubtractMemory()));
    QToolButton *DisplayMemButton = CreateButton("MR", SLOT(DisplayMemory()));
    QToolButton *ResetMemButton = CreateButton("MC", SLOT(ResetMemory()));
    QToolButton *EButton = CreateButton("e", SLOT(NumberClicked()));
    QToolButton *CleanButton = CreateButton("C", SLOT(Clean()));

    //Main layout
    QGridLayout *MainLayout = new QGridLayout;
    MainLayout->setSizeConstraint(QLayout::SetFixedSize);
    MainLayout->addWidget(display, 0, 0, 1, 8);
    MainLayout->addWidget(CleanButton, 1, 0);
    MainLayout->addWidget(displayedResult, 1, 1, 1, 7);
    for(int i = 0; i < 9; i++)
    {
        MainLayout->addWidget(NumberButton[i + 1], i / 3 + 2, i % 3, 1, 1);
    }
    NumberButton[0]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    MainLayout->addWidget(NumberButton[0], 5, 0, 1, 2);
    MainLayout->addWidget(PointButton, 5, 2);
    MainLayout->addWidget(AddButton, 2, 3);
    MainLayout->addWidget(SubtractButton, 3, 3);
    MainLayout->addWidget(MultiplyButton, 4, 3);
    MainLayout->addWidget(DivideButton, 5, 3);
    MainLayout->addWidget(ExponentButton, 2, 4);
    MainLayout->addWidget(EButton, 3, 4);
    MainLayout->addWidget(LogButton, 4, 4);
    MainLayout->addWidget(LnButton, 5, 4);
    MainLayout->addWidget(SqrtButton, 2, 5);
    MainLayout->addWidget(SinButton, 3, 5);
    MainLayout->addWidget(CosButton, 4, 5);
    MainLayout->addWidget(TanButton, 5, 5);
    MainLayout->addWidget(BackspaceButton, 2, 6);
    MainLayout->addWidget(OpenBracketButton, 3, 6);
    MainLayout->addWidget(ClosedBracketButton, 4, 6);
    MainLayout->addWidget(EqualsButton, 5, 6);
    MainLayout->addWidget(AddMemButton, 2, 7);
    MainLayout->addWidget(SubMemButton, 3, 7);
    MainLayout->addWidget(DisplayMemButton, 4, 7);
    MainLayout->addWidget(ResetMemButton, 5, 7);
    setLayout(MainLayout);

    //Constants menu
    ConstantsWindow = new QWidget;
    ConstantsWindow->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    //Constants menu buttons
    QToolButton *PiButton = CreateButton("π", SLOT(ConstantClicked()));
    QToolButton *EuButton = CreateButton("e", SLOT(ConstantClicked()));
    QToolButton *FiButton = CreateButton("Φ", SLOT(ConstantClicked()));
    QToolButton *GravAccButton = CreateButton("g", SLOT(ConstantClicked()));
    QToolButton *GravConstButton = CreateButton("G", SLOT(ConstantClicked()));
    QToolButton *PlanckButton = CreateButton("h", SLOT(ConstantClicked()));
    QToolButton *EMassButton = CreateButton("Me", SLOT(ConstantClicked()));
    QToolButton *ERadiusButton = CreateButton("Re", SLOT(ConstantClicked()));
    QToolButton *LightButton = CreateButton("c", SLOT(ConstantClicked()));
    QToolButton *AvogardoButton = CreateButton("NA", SLOT(ConstantClicked()));
    QToolButton *UniversalConstButton = CreateButton("R", SLOT(ConstantClicked()));
    QToolButton *BoltzmannButton = CreateButton("kB", SLOT(ConstantClicked()));
    QToolButton *ElecButton = CreateButton("k", SLOT(ConstantClicked()));
    QToolButton *ElementButton = CreateButton("e-", SLOT(ConstantClicked()));
    QToolButton *MagnetButton = CreateButton("μ0", SLOT(ConstantClicked()));
    QToolButton *MassPlusButton = CreateButton("m+", SLOT(ConstantClicked()));
    QToolButton *MassMinusButton = CreateButton("m-", SLOT(ConstantClicked()));
    QToolButton *MassNeutralButton = CreateButton("mN", SLOT(ConstantClicked()));

    //Constants menu layout
    QGridLayout *ConstantsLayout = new QGridLayout;
    ConstantsLayout->addWidget(PiButton, 0, 0);
    ConstantsLayout->addWidget(EuButton, 0, 1);
    ConstantsLayout->addWidget(FiButton, 0, 2);
    ConstantsLayout->addWidget(GravAccButton, 1, 0);
    ConstantsLayout->addWidget(GravConstButton, 1, 1);
    ConstantsLayout->addWidget(PlanckButton, 1, 2);
    ConstantsLayout->addWidget(EMassButton, 2, 0);
    ConstantsLayout->addWidget(ERadiusButton, 2, 1);
    ConstantsLayout->addWidget(LightButton, 2, 2);
    ConstantsLayout->addWidget(AvogardoButton, 3, 0);
    ConstantsLayout->addWidget(UniversalConstButton, 3, 1);
    ConstantsLayout->addWidget(BoltzmannButton, 3, 2);
    ConstantsLayout->addWidget(ElecButton, 4, 0);
    ConstantsLayout->addWidget(ElementButton, 4, 1);
    ConstantsLayout->addWidget(MagnetButton, 4, 2);
    ConstantsLayout->addWidget(MassPlusButton, 5, 0);
    ConstantsLayout->addWidget(MassMinusButton, 5, 1);
    ConstantsLayout->addWidget(MassNeutralButton, 5, 2);
    ConstantsWindow->setLayout(ConstantsLayout);

    ConstantsWindow->adjustSize();
    ConstantsWindow->setFixedSize(ConstantsWindow->size());

    //Functions menu
    FunctionsWindow = new QWidget;
    FunctionsWindow->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    //Functions menu buttons
    const int BSize = 45;
    QToolButton *RootButton = CreateButton("nroot", SLOT(OperatorClicked()), BSize);
    QToolButton *LogarithmButton = CreateButton("nlog", SLOT(OperatorClicked()), BSize);
    QToolButton *FactorialButton = CreateButton("exp", SLOT(OperatorClicked()), BSize);
    QToolButton *ArcSinButton = CreateButton("asin", SLOT(OperatorClicked()), BSize);
    QToolButton *ArcCosButton = CreateButton("acos", SLOT(OperatorClicked()), BSize);
    QToolButton *ArcTanButton = CreateButton("atg", SLOT(OperatorClicked()), BSize);

    //Functions menu layout
    QGridLayout *FunctionsLayout = new QGridLayout;
    FunctionsLayout->addWidget(RootButton, 0, 0);
    FunctionsLayout->addWidget(LogarithmButton, 0, 1);
    FunctionsLayout->addWidget(FactorialButton, 0, 2);
    FunctionsLayout->addWidget(ArcSinButton, 1, 0);
    FunctionsLayout->addWidget(ArcCosButton, 1, 1);
    FunctionsLayout->addWidget(ArcTanButton, 1, 2);
    FunctionsWindow->setLayout(FunctionsLayout);

    FunctionsWindow->adjustSize();
    FunctionsWindow->setFixedSize(FunctionsWindow->size());

    //File reader
    FileWindow = new QWidget;
    FileWindow->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    FileDisplay = new QTextEdit;
    FileDisplay->setReadOnly(true);
    QGridLayout *FileLayout = new QGridLayout;
    FileLayout->addWidget(FileDisplay);
    FileWindow->setLayout(FileLayout);

    FileWindow->adjustSize();
    FileWindow->setFixedSize(FileWindow->size());

    //Actions
    SetupActions();

    setWindowTitle("Kalkulator");
}

Calculator::~Calculator()
{
}

void Calculator::AddMemory()
{
    QString newText = displayedResult->text();
    if(newText != "" && newText != "Error") GlobalMemory += newText.toDouble();
}

void Calculator::BackspaceClicked()
{
    if(NewEquation)
    {
        Clean();
        return;
    }
    //std::cout << NotationString.toStdString() << std::endl;
    QString s = DisplayString.right(1);
    DisplayString.remove(DisplayString.length() - 1, 1);
    if(s == ")" || s == "(") NotationString.remove(NotationString.length() - 2, 2);
    else if(IsOperator(s)) NotationString.remove(NotationString.length() - 3, 3);
    else if(IsNumerical(s)) NotationString.remove(NotationString.length() - 1, 1);
    if(IsPart(s) == _true)
    {
        NotationString.remove(NotationString.length() - 1, 1);
    }
    else if(IsPart(s) == _last)
    {
        NotationString.remove(NotationString.length() - 2, 2);
    }
    //std::cout << NotationString.toStdString() << std::endl;
    display->setText(DisplayString);
    if(DisplayString == "") NewEquation = true;
}

QString Calculator::CalculateRPN(QString RPN)
{
    std::stack<QString> SignStack;
    if(RPN.length() == 0)
    {
        GlobalError = BadEquation;
        return "";
    }
    if(RPN[RPN.length() - 1] != ' ') RPN += " ";
    for(;;)
    {
        int pos = RPN.indexOf(" ");
        QString s = RPN.left(pos);
        if(pos == -1 || RPN == " ") break;
        if(s == "" || s == " ") GlobalError = BadEquation;
        RPN.remove(0, pos + 1);
        if(IsOperator(s))
        {
            double a, b = 0;
            a = TakeNumber(SignStack);
            if(iType[s] == BinaryOperator) b = TakeNumber(SignStack);
            if(GlobalError != NoError) return "";
            if(s == "+")
            {
                s = QString::number(b + a);
            }
            else if(s == "-")
            {
                s = QString::number(b - a);
            }
            else if(s == "*")
            {
                s = QString::number(b * a);
            }
            else if(s == "/")
            {
                if(a == 0.0) GlobalError = DivZero;
                s = QString::number(b / a);
            }
            else if(s == "^")
            {
                s = QString::number(std::pow(b, a));
            }
            else if(s == "log")
            {
                s = QString::number(std::log10(a));
            }
            else if(s == "nlog")
            {
                if(b == 1 || b == 0 || b < 0) GlobalError = Log1;
                a = std::log(a);
                b = std::log(b);
                s = QString::number(a / b);
            }
            else if(s == "ln")
            {
                s = QString::number(std::log(a));
            }
            else if(s == "sqrt")
            {
                if(a < 0) GlobalError = NegSqrt;
                s = QString::number(std::sqrt(a));
            }
            else if(s == "nroot")
            {
                int c = static_cast<int>(b); //a 4 b 2
                if(c % 2 == 0 && a < 0) GlobalError = NegSqrt;
                s = QString::number(std::pow(a, 1 / b));
            }
            else if(s == "sin")
            {
                a *= PI / 180.0;
                s = QString::number(std::sin(a));
            }
            else if(s == "cos")
            {
                a *= PI / 180.0;
                s = QString::number(std::cos(a));
            }
            else if(s == "tg")
            {
                a *= PI / 180.0;
                s = QString::number(std::tan(a));
            }
            else if(s == "asin")
            {
                s = QString::number(std::asin(a));
            }
            else if(s == "acos")
            {
                s = QString::number(std::acos(a));
            }
            else if(s == "atg")
            {
                s = QString::number(std::atan(a));
            }
            else if(s == "exp")
            {
                s = QString::number(std::exp(a));
            }
        }
        else if(!IsNumerical(s)) GlobalError = BadEquation;
        SignStack.push(s);
    }
    if(SignStack.size() != 1) GlobalError = BadEquation;
    return SignStack.top();

}

void Calculator::Clean()
{
    NotationString = "";
    DisplayString = "";
    GlobalError = NoError;
    NewEquation = true;
    display->setText(DisplayString);
    displayedResult->setText(NotationString);
}

void Calculator::ConstantClicked()
{
    QToolButton *ClickedButton = qobject_cast<QToolButton *>(sender());
    QString name = ClickedButton->text();
    QString value = "";
    if(name == "π") value = "3.1415926535897932384626433";
    else if(name == "e") value = "2.7182818";
    else if(name == "Φ") value = "1.618033988749894848204586834";
    else if(name == "g") value = "9.80665";
    else if(name == "G") value = "6.6740831e-11";
    else if(name == "h") value = "6.62607004081e-34";
    else if(name == "Me") value = "5.97237e24";
    else if(name == "Re") value = "6378.1";
    else if(name == "c") value = "299792458";
    else if(name == "NA") value = "6.02214085774e23";
    else if(name == "R") value = "8.314459848";
    else if(name == "kB") value = "1.3806485279e-23";
    else if(name == "k") value = "8.9875517873681764e9";
    else if(name == "e-") value = "1.602176620898e-19";
    else if(name == "μ0") value = "1.2566370614e-6";
    else if(name == "m+") value = "1.67262177774e-27";
    else if(name == "m-") value = "9.1093835611e-31";
    else if(name == "mN") value = "1.67492747121e-27";
    TextUpdate(value);
}

QToolButton *Calculator::CreateButton(QString text, const char *ButtonSlot, int w, int h)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QFont font = button->font();
    font.setPointSize(font.pointSize() * 1.5);
    button->setFont(font);
    connect(button, SIGNAL(clicked()), this, ButtonSlot);
    if(w != 0 && h != 0) button->setFixedSize(QSize(w, h));
    return button;
}

void Calculator::DisplayMemory()
{
    TextUpdate(QString::number(GlobalMemory));
}

void Calculator::EqualsClicked()
{
    if(NotationString == "") return;
    std::cout << NotationString.toStdString() << std::endl;
    NotationString = MakeRPN(NotationString);
    if(GlobalError != NoError)
    {
        ErrorOccured();
        return;
    }
    std::cout << NotationString.toStdString() << std::endl;
    NotationString = CalculateRPN(NotationString);
    std::cout << NotationString.toStdString() << std::endl;
    if(GlobalError != NoError)
    {
        ErrorOccured();
        return;
    }
    displayedResult->setText(NotationString);
    NotationString = "";
    NewEquation = true;
}

void Calculator::ErrorOccured()
{
    QString ErrorMessage = "";
    switch (GlobalError) {
    case BadEquation:
        ErrorMessage += "Wrong formula!";
        break;
    case DivZero:
        ErrorMessage += "Division by 0!";
        break;
    case MissingBrackets:
        ErrorMessage += "Missing brackets!";
        break;
    case NegSqrt:
        ErrorMessage += "Negative even root!";
        break;   
    case Log1:
        ErrorMessage += "Wrong log base!";
        break;
    default:
        break;
    }
    GlobalError = NoError;
    displayedResult->setText(ErrorMessage);
    NewEquation = true;
    NotationString = "";
}

int Calculator::FindBracketPos(QString s)
{
    int open = 0;
    for(int i = 0; i < s.length(); i++)
    {
        if(s[i] == '(') open++;
        else if (s[i] == ')')
        {
            if(open) open--;
            else return i;
        }
    }
    return -1;
}

bool Calculator::IsNumerical(QString s)
{
    bool NoDots = false;
    bool NoE = false;
    for(int i = 0; i < s.length(); i++)
    {
        if(s[i] >= '0' && s[i] <= '9') continue;
        else if(s[i] == '.' && i != 0 && i != s.length() - 1 && !NoDots)
        {
            NoDots = true;
            continue;
        }
        else if(s[i] == 'e' && i != 0 && i != s.length() - 1 && !NoE)
        {
            NoE = true;
            NoDots = true;
            continue;
        }
        else if((i == 0 || (i != 0 && s[i - 1] == 'e')) && s[i] == '-' && s.length() > 1) continue;
        else return false;
    }
    return true;
}

bool Calculator::IsOperator(QString s)
{
    if(iType[s] == UnaryOperator || iType[s] == BinaryOperator) return true;
    else return false;
}

Calculator::Logic Calculator::IsPart(QString s)
{
    QString prev = DisplayString.right(1);
    if(prev == "l" && s == "o") return _true;
    else if(prev == "o" && s == "g") return _last;
    else if(prev == "l" && s == "n") return _last;
    else if(prev == "c" && s == "o") return _true;
    else if(prev == "o" && s == "s") return _last;
    else if(prev == "s" && s == "i") return _true;
    else if(prev == "i" && s == "n") return _last;
    else if(prev == "s" && s == "q") return _true;
    else if(prev == "q" && s == "r") return _true;
    else if(prev == "r" && s == "t") return _last;
    else if(prev == "n" && s == "r") return _true;
    else if(prev == "r" && s == "o") return _true;
    else if(prev == "o" && s == "o") return _true;
    else if(prev == "o" && s == "t") return _last;
    else if(prev == "n" && s == "l") return _true;
    else if(prev == "t" && s == "g") return _last;
    else if(prev == "a" && s == "s") return _true;
    else if(prev == "a" && s == "c") return _true;
    else if(prev == "a" && s == "t") return _true;
    else if(prev == "e" && s == "x") return _true;
    else if(prev == "x" && s == "p") return _last;
    else if(s == "l") return _true;
    else if(s == "c") return _true;
    else if(s == "s") return _true;
    else if(s == "n") return _true;
    else if(s == "t") return _true;
    else if(s == "a") return _true;
    else if(s == "e") return _true;
    else return _false;
}

void Calculator::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        Clean();
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}

QString Calculator::MakeRPN(QString given)
{
    std::stack<QString> SignStack;
    if(given[given.length() - 1] != ' ') given += " ";
    QString result("");
    for(;;)
    {
        int pos = given.indexOf(" ");
        if(pos == -1 || given == " ") break;
        QString s = given.left(pos);
        given.remove(0, pos + 1);
        //cerr << s << "*" << given << "*" << endl;
        if(IsNumerical(s))
        {
            if(s.indexOf(".") != -1 && s.indexOf("e") == -1)
            {
                int ss = s.length() - 1;
                while(s[ss] == '0' || s[ss] == '.')
                {
                    s.remove(ss, 1);
                    ss = s.length() - 1;
                }
            }
            else if(s.indexOf("e") != -1)
            {
                int epos = s.indexOf("e");
                QString zeros = s.right(s.length() - epos - 1);
                if(zeros.toInt() < 5)
                {
                    QString ss = s.left(epos);
                    double base = ss.toDouble();
                    base *= pow(10, zeros.toInt());
                    s = QString::number(base);
                }
            }
            while(s[0] == '0' && s[1] != '.' && s.length() != 1)
            {
                s.remove(0, 1);
            }
            result += s + " ";
        }
        else if(IsOperator(s))
        {
            if(s == "(")
            {
                pos = FindBracketPos(given);
                if(pos == -1)
                {
                    GlobalError = MissingBrackets;
                    return "";
                }
                s = given.left(pos - 1);
                given.remove(0, pos + 2);
                result += MakeRPN(s);
            }
            else if(s == ")")
            {
                GlobalError = MissingBrackets;
                return "";
            }
            else
            {
                if(!SignStack.empty())
                {
                    QString incoming = s;
                    QString ontop = SignStack.top();
                    while(Rank[incoming] != 4 && Rank[incoming] <= Rank[ontop])
                    {
                        result += SignStack.top() + " ";
                        SignStack.pop();
                        if(SignStack.empty()) break;
                        ontop = SignStack.top();
                    }
                }
                SignStack.push(s);
            }
        }
        else
        {
            GlobalError = BadEquation;
            return "";
        }
        //std::cout << "Result = " << result.toStdString() << std::endl;
    }
    result += StackToString(SignStack);
    return result;
}

void Calculator::NumberClicked()
{
    QToolButton *ClickedButton = qobject_cast<QToolButton *>(sender());
    TextUpdate(ClickedButton->text());
    //std::string current_locale_text = NotationString.toLocal8Bit().constData();
    //std::cout << current_locale_text << std::endl;
}

void Calculator::OpenChange()
{
    OpenFile("Changelog.txt");
}

void Calculator::OpenConstants()
{
    ConstantsWindow->show();
}

void Calculator::OpenFile(QString s)
{
    QFile InputFile(s);
    if(!InputFile.open(QIODevice::ReadOnly))
    {
        QString message = "Error. " + s + " could not be opened.";
        FileDisplay->setText(message);
    }
    else
    {
        QTextStream InputStream(&InputFile);
        FileDisplay->setText(InputStream.readAll());
    }

    FileWindow->show();
}

void Calculator::OpenFunctions()
{
    FunctionsWindow->show();
}

void Calculator::OpenHelp()
{
    OpenFile("README.txt");
}

void Calculator::OperatorClicked()
{
    QToolButton *ClickedButton = qobject_cast<QToolButton *>(sender());
    TextUpdate(ClickedButton->text());
}

void Calculator::ResetCursor()
{
    display->setCursorPosition(DisplayString.length());
}

void Calculator::ResetMemory()
{
    GlobalMemory = 0;
}

void Calculator::SetupActions()
{
    ConstantsAct = new QAction("Constants", this);
    connect(ConstantsAct, SIGNAL(triggered()), this, SLOT(OpenConstants()));

    FunctionsAct = new QAction("Functions", this);
    connect(FunctionsAct, SIGNAL(triggered()), this, SLOT(OpenFunctions()));

    HelpAct = new QAction("Help", this);
    connect(HelpAct, SIGNAL(triggered()), this, SLOT(OpenHelp()));

    ChangeAct = new QAction("Changelog", this);
    connect(ChangeAct, SIGNAL(triggered()), this, SLOT(OpenChange()));
}

QString Calculator::StackToString(std::stack<QString> &container)
{
    QString s = "";
    while(!container.empty())
    {
        s += container.top();
        s += " ";
        container.pop();
    }
    return s;
}

void Calculator::SubtractMemory()
{
    QString newText = displayedResult->text();
    if(newText != "" && newText != "Error") GlobalMemory -= newText.toDouble();
}

double Calculator::TakeNumber(std::stack<QString> &stack)
{
    if(stack.empty())
    {
        GlobalError = BadEquation;
        return 0;
    }
    QString temp = stack.top();
    stack.pop();
    bool SpecialCase = false;
    if(temp[0] == '-')
    {
        temp.remove(0, 1);
        SpecialCase = true;
    }
    double x = temp.toDouble();
    if(SpecialCase) x *= (-1);
    return x;
}

void Calculator::TextChanged(const QString &text)
{
    if(DisplayString.length() < text.length())
    {
        int pos = display->cursorPosition() - 1;
        QString newText = text[pos];
        if(!IsOperator(newText)
           && !IsNumerical(newText)
           && !IsPart(newText)
           && newText != "."
           && newText != "e") newText = "";
        TextUpdate(newText);
    }
    else
    {
        BackspaceClicked();
    }
}

void Calculator::TextUpdate(QString newText)
{
    bool SpecialCase = false;
    bool Unary = false;
    bool Part = false;
    if(NewEquation)
    {
        NewEquation = false;
        DisplayString = "";
        if(newText == ".")
        {
            DisplayString += "0";
            NotationString += "0";
        }
        else if(iType[newText] == BinaryOperator
                && displayedResult->text() != ""
                && displayedResult->text() != "Error")
        {
            DisplayString += displayedResult->text();
            NotationString += displayedResult->text();
        }
        if(newText == "-"
           && DisplayString == "") SpecialCase = true;
    }
    else if(DisplayString.right(1) == "(") SpecialCase = true;
    else if(DisplayString.right(1) == "e" && newText == "-") SpecialCase = true;
    if(iType[newText] == UnaryOperator) Unary = true;
    if(IsPart(newText) == _last)
    {
        Part = true;
    }
    DisplayString += newText;
    if(IsOperator(newText))
    {
        if(newText == ")") newText = " " + newText;
        else if(Unary) newText = newText + " ";
        else if(!SpecialCase) newText = " " + newText + " ";
    }
    if(Part) newText = newText + " ";
    if(newText == "n") newText = " " + newText;
    NotationString += newText;
    display->setText(DisplayString);
}
