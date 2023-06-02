#include <iostream>
#include <cassert>    
#include <string> 
using namespace std;

struct Number;
struct BinaryOperation;
struct FunctionCall;
struct Variable;

struct Transformer;

struct Expression //базовая абстрактная структура
{
	virtual ~Expression() { }; //виртуальный деструктор
	virtual double evaluate() const = 0; //абстрактный метод «вычислить»
	virtual std::string print() const = 0;//абстрактный метод печать
	virtual Expression * transform(Transformer* tr) const = 0;
};
struct Transformer
{
	virtual ~Transformer() {}
	virtual Expression *transformNumber(Number const*) = 0;
	virtual Expression *transformBinaryOperation(BinaryOperation const*) = 0;
	virtual Expression *transformFunctionCall(FunctionCall const*) = 0;
	virtual Expression *transformVariable(Variable const*) = 0;
};


struct Number : Expression // стуктура «Число»
{
	Number(double value) : value_(value) {} //конструктор
	double value() const {return value_;} // метод чтения значения числа
	double evaluate() const {return value_;} // реализация виртуального метода «вычислить»
	~Number() {}//деструктор, тоже виртуальный
	//метод print() возвращает строковое представление числового значения объекта класса Number
	std::string print() const {return std::to_string(this->value_);}
	Expression * transform(Transformer *tr) const
	{
		return tr->transformNumber(this);
	}

private:
	double value_; // само вещественное число
};
struct BinaryOperation : Expression // «Бинарная операция»
{
	enum { // перечислим константы, которыми зашифруем символы операций
		PLUS = '+',
		MINUS = '-',
		DIV = '/',
		MUL = '*'
	};
	// в конструкторе надо указать 2 операнда — левый и правый, а также сам символ операции
		BinaryOperation(Expression const *left, int op, Expression const *right): left_(left), op_(op), right_(right)
	{
		assert(left_ && right_);
	}
	~BinaryOperation() //в деструкторе освободим занятую память
	{
		delete left_;
		delete right_;
	}
	Expression const *left() const { return left_; } // чтение левого операнда
	Expression const *right() const { return right_; } // чтение правого операнда
		int operation() const { return op_; } // чтение символа операции
	double evaluate() const // реализация виртуального метода «вычислить»
	{
		double left = left_->evaluate(); // вычисляем левую часть
		double right = right_->evaluate(); // вычисляем правую часть
		switch (op_) // в зависимости от вида операции, складываем, вычитаем, или делим левую и правую части
		{
		case PLUS: return left + right;
		case MINUS: return left - right;
		case DIV: {
			if (right == 0.0) {
				throw std::invalid_argument("Division by zero");
			}
			else {
				return left / right;
			}
		}
		case MUL: return left * right;
		}
	}
	Expression * transform(Transformer *tr) const
	{
		return tr->transformBinaryOperation(this);
	}
	std::string print() const {
		return this->left_->print() + std::string(1, this->op_) + this->right_->print();
	}
private:
	Expression const *left_; // указатель на левый операнд
	Expression const *right_; // указатель на правый операнд
	int op_; // символ операции
};
struct FunctionCall : Expression // структура «Вызов функции»
{
	// в конструкторе надо учесть имя функции и ее аогумент
	FunctionCall(std::string const &name, Expression const *arg) : name_(name),
		arg_(arg)
	{
		assert(arg_);
		assert(name_ == "sqrt" || name_ == "abs");
	} // разрешены только вызов sqrt и abs
	std::string const & name() const
	{
		return name_;
	}
	Expression const *arg() const // чтение аргумента функции
	{
		return arg_;
	}
	~FunctionCall() { delete arg_; } // освобождаем память в деструкторе
	virtual double evaluate() const { // реализация виртуального метода
	//«вычислить»
		if (name_ == "sqrt")
			if (arg_->evaluate() < 0.0) {
				throw std::invalid_argument("Square root of negative number");
			}
			else {
				return sqrt(arg_->evaluate()); // либо вычисляем корень квадратный}
			}
		else return fabs(arg_->evaluate());
	} // либо модуль — остальные функции запрещены
	std::string print() const {
		return this->name_ + "(" + this->arg_->print() + ")";
	}
	Expression * transform(Transformer *tr) const
	{
		return tr->transformFunctionCall(this);
	}
private:
	std::string const name_; // имя функции
	Expression const *arg_; // указатель на ее аргумент
};
struct Variable : Expression // структура «Переменная»
{
	Variable(std::string const &name) : name_(name) { } //в конструкторе надо
	//указать ее имя
	std::string const &name() const { return name_; } // чтение имени переменной
	double evaluate() const // реализация виртуального метода «вычислить»
	{
		return 0.0;
	}
	std::string print() const {
		return this->name_;
	}
	Expression * transform(Transformer *tr) const
	{
		return tr->transformVariable(this);
	}
private:
	std::string const name_; // имя переменной
};


struct CopySyntaxTree : Transformer
{
	Expression *transformNumber(Number const *number)
	{
		return new Number(number->value());
	}
	Expression *transformBinaryOperation(BinaryOperation const *binop)
	{
		return new BinaryOperation(
			binop->left()->transform(this),
			binop->operation(),
		    binop->right()->transform(this)
		);
	}
	Expression *transformFunctionCall(FunctionCall const *fcall)
	{
		return new FunctionCall(
			fcall->name(),
			fcall->arg()->transform(this)
		);
	}
	Expression *transformVariable(Variable const *var)
	{
		return  new Variable(var->name());;
	}
	~CopySyntaxTree() { };
};
struct FoldConstants : Transformer
{
	Expression *transformNumber(Number const *number)
	{
		return new Number(number->value());
	}
	Expression *transformBinaryOperation(BinaryOperation const *binop)
	{
		// Создаем указатели на левое и правое выражение
		Expression * nleft = (binop->left())->transform(this); // рекурсивно уходим в левый операнд, чтобы свернуть
		Expression * nright = (binop->right())->transform(this); // рекурсивно уходим в правый операнд, чтобы свернуть
		int noperation = binop->operation();

		// Создаем новый объект типа BinaryOperation с новыми указателями
		BinaryOperation * nbinop = new BinaryOperation(nleft, noperation, nright);
		//Проверяем на приводимость указателей к типу Number
		Number * nleft_is_number = dynamic_cast<Number *>(nleft);
		Number * nright_is_number = dynamic_cast<Number *>(nright);
		if (nleft_is_number && nright_is_number) {
			delete nbinop;
			//Возвращаем вычисленное значение выражения
			return new Number(binop->evaluate());
		}
		else {
			return nbinop;
		}

	}
	Expression *transformFunctionCall(FunctionCall const *fcall)
	{
		// Создаем указатель на аргумент
		Expression * arg = (fcall->arg())->transform(this);// рекурсивно сворачиваем аргумент
		std::string const & nname = fcall->name();

		// Создаем новый объект типа FunctionCall с новым указателем
		FunctionCall * nfcall = new FunctionCall(nname, arg);

		// Проверяем на приводимость указателя к типу Number
		Number * arg_is_number = dynamic_cast<Number *>(arg);
		if (arg_is_number) { // если аргумент — число
			delete nfcall;
			//Возвращаем вычисленное значение выражения
			return new Number(fcall->evaluate());
		}
		else return nfcall;
	}
	Expression *transformVariable(Variable const *var)
	{
		return new Variable(var->name());
	}
	~FoldConstants() { };
};

int main()
{
	Number* n32 = new Number(32.0);
	Number* n16 = new Number(16.0);
	BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
	FunctionCall* callSqrt = new FunctionCall("sqrt", minus);
	Variable* var = new Variable("var");
	BinaryOperation* mult = new BinaryOperation(var, BinaryOperation::MUL, callSqrt);
	FunctionCall* callAbs = new FunctionCall("abs", mult);

	CopySyntaxTree CST;
	Expression* newExpr = callAbs->transform(&CST);
	std::cout << newExpr->print() << std::endl;
	
	FoldConstants FC;
	newExpr = callAbs->transform(&FC);
	std::cout << newExpr->print() << std::endl;
}
