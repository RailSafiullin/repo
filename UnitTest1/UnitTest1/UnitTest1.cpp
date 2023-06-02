#include "pch.h"
#include "CppUnitTest.h"
#include <string> 
#include "lab6_TRPO.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Tests
{
	TEST_CLASS(ExpressionTests)
	{
	public:

		TEST_METHOD(TestNumberEvaluate)
		{
			Number* n = new Number(7.5);
			Assert::AreEqual(7.5, n->evaluate());
		}

		TEST_METHOD(TestNumberPrint)
		{
			Number* n = new Number(7.5);
			Assert::AreEqual(to_string(7.5), n->print());
		}

		TEST_METHOD(TestBinaryOperationEvaluatePlus)
		{
			Number* n1 = new Number(7.5);
			Number* n2 = new Number(5.2);
			BinaryOperation* op = new BinaryOperation(n1, BinaryOperation::PLUS, n2);
			Assert::AreEqual(12.7, op->evaluate());
		}

		TEST_METHOD(TestBinaryOperationEvaluateMinus)
		{
			Number* n1 = new Number(7.5);
			Number* n2 = new Number(5.2);
			BinaryOperation* op = new BinaryOperation(n1, BinaryOperation::MINUS, n2);
			Assert::AreEqual(2.3, op->evaluate());
		}

		TEST_METHOD(TestBinaryOperationEvaluateDIV)
		{
			Number* n1 = new Number(7.5);
			Number* n2 = new Number(2.5);
			BinaryOperation* op = new BinaryOperation(n1, BinaryOperation::DIV, n2);
			Assert::AreEqual(3.0, op->evaluate());
		}
		TEST_METHOD(TestBinaryOperationEvaluateMul)
		{
			Number* n1 = new Number(7.0);
			Number* n2 = new Number(3.0);
			BinaryOperation* op = new BinaryOperation(n1, BinaryOperation::MUL, n2);
			Assert::AreEqual(21.0, op->evaluate());
		}


		TEST_METHOD(TestBinaryOperationEvaluateDivByZero)
		{
			Number* n1 = new Number(7.5);
			Number* n2 = new Number(0.0);
			BinaryOperation* op = new BinaryOperation(n1, BinaryOperation::DIV, n2);
			try {
				op->evaluate();
				Assert::Fail(L"Expected exception not thrown");
			}
			catch (std::invalid_argument const& err) {
				Assert::AreEqual("Division by zero", err.what());
			}
			catch (...) {
				Assert::Fail(L"Unexpected exception type thrown");
			}
		}

		TEST_METHOD(TestBinaryOperationPrint)
		{
			Number* n1 = new Number(7.5);
			Number* n2 = new Number(5.2);
			BinaryOperation* op = new BinaryOperation(n1, BinaryOperation::PLUS, n2);
			Assert::AreEqual(to_string(7.5) + "+" + to_string(5.2), op->print());
		}

		TEST_METHOD(TestFunctionCallEvaluateSqrt)
		{
			Number* n = new Number(25.0);
			FunctionCall f("sqrt", n);
			Assert::AreEqual(5.0, f.evaluate());
		}

		TEST_METHOD(TestFunctionCallPrint)
		{
			Number* n = new Number(9.0);
			FunctionCall f("sqrt", n);
			Assert::IsTrue("sqrt(9.000000)" == f.print());
		}

		TEST_METHOD(TestVariableEvaluate)
		{
			Variable* var = new Variable("x");
			Assert::AreEqual(0.0, var->evaluate());
		}

		TEST_METHOD(TestVariablePrint)
		{
			Variable* var = new Variable("x");
			Assert::IsTrue("x" == var->print());
		}

		TEST_METHOD(TestCopySyntaxTreeTransformNumber)
		{
			Number* n = new Number(10.5);
			CopySyntaxTree cst;
			Expression* newExpr = n->transform(&cst);
			Assert::IsTrue(dynamic_cast<Number*>(newExpr) != nullptr);
			Assert::AreEqual(10.5, dynamic_cast<Number*>(newExpr)->value());
			delete newExpr;
		}

		TEST_METHOD(TestCopySyntaxTreeTransformBinaryOperation)
		{
			Number* n1 = new Number(10.5);
			Number* n2 = new Number(5.2);
			BinaryOperation* op = new BinaryOperation(n1, BinaryOperation::PLUS, n2);
			CopySyntaxTree cst;
			Expression* newExpr = op->transform(&cst);
			Assert::IsTrue(dynamic_cast<BinaryOperation*>(newExpr) != nullptr);
			Assert::AreEqual(to_string(10.5) + "+" + to_string(5.2), dynamic_cast<BinaryOperation*>(newExpr)->print());
			delete newExpr;
		}

		TEST_METHOD(TestCopySyntaxTreeTransformFunctionCall)
		{
			Number* n1 = new Number(10.5);
			FunctionCall f("sqrt", n1);
			CopySyntaxTree cst;
			Expression* newExpr = f.transform(&cst);
			Assert::IsTrue(dynamic_cast<FunctionCall*>(newExpr) != nullptr);
			FunctionCall* newFunc = dynamic_cast<FunctionCall*>(newExpr);
			Assert::IsTrue("sqrt" == newFunc->name());
			delete newExpr;
		}

		TEST_METHOD(TestCopySyntaxTreeTransformVariable)
		{
			Variable var("x");
			CopySyntaxTree cst;
			Expression* newExpr = var.transform(&cst);
			Assert::IsTrue(dynamic_cast<Variable*>(newExpr) != nullptr);
			Assert::IsTrue("x" == dynamic_cast<Variable*>(newExpr)->name());
			delete newExpr;
		}
	};
}