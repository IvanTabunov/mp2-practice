#include "TArithmeticExpression.h"
#include <iostream>
#include <map>

TArithmeticExpression::TArithmeticExpression(string infx) : infix(infx)
{
	priority = { {"(",1},{")",1},{"+",2},{"-",2}, {"*",3},{"/",3}};
	ToPostfix();

}

void TArithmeticExpression::Parse()
{
	string currentElement;
	for (char c : infix) {
		if (c == '+' || c == '-' || c == '*' || c == '/' || c == '('||c==')') {
			// ���� ������� ������ - ��������, ��������� ������� ������� � ������ � ����� ��������� ��������
			if (!currentElement.empty()) {
				lexems.push_back(currentElement);
				currentElement = "";
			}
			lexems.push_back(string(1, c));
		}
		else if (isdigit(c) || c == '.') {
			// ���� ������� ������ - ����� ��� �����, ��������� ��� � �������� ��������
			currentElement += c;
		}
		else if (isalpha(c)) {
			// ���� ������� ������ - �����, ��������� ��� � �������� ��������
			currentElement += c;
		}
	}
	// ��������� ��������� ������� � ������
	if (!currentElement.empty()) {
		lexems.push_back(currentElement);
	}	
}

vector<string> TArithmeticExpression::GetOperands() const
{
	vector<string> op;
	for (const auto& item : operands)
		if (!IsConst(item.first))
		{
			op.push_back(item.first);
		}
	return op;
}

bool TArithmeticExpression::IsOperator(const string& lecsem) const
{
	bool flag = false;
	for (const auto& c : priority)
	{
		if (lecsem == c.first)
		{
			flag = true;
		}	
	}
	return flag;

}

void TArithmeticExpression::SetValues()
{
	double value;
	for (auto& op : operands)
	{
		if (!IsConst(op.first))
		{
			cout << "Enter value of " << op.first << ":";
			cin >> value;
			operands[op.first] = value;
		}
	}
}

bool TArithmeticExpression::IsConst(const string& lecsem) const
{
	bool hasDot = false;
	bool flag = true;
	for (char c : lecsem) {
		if (!std::isdigit(c)) {
			if (c == '.' && !hasDot) {
				hasDot = true;
			}
			else {
				flag = false;
				break;
			}
		}
	}
	return flag;

}

void TArithmeticExpression::ToPostfix() {
	Parse();
	TStack<string> st;
	string stackItem;
	for (string item : lexems) {
		if (item == "(") {
			st.Push(item);
		}
		else if (item == ")") {
			stackItem = st.Top();
			st.Pop();
			while (stackItem != "(") {
				postfix.push_back(stackItem);
				stackItem = st.Top();
				st.Pop();
			}
		}
		else if (item == "+" || item == "-" || item == "*" || item == "/") {
			while (!st.IsEmpty()) {
				stackItem = st.Top();
				st.Pop();
				if (priority[item] <= priority[stackItem])
					postfix.push_back(stackItem);
				else {
					st.Push(stackItem);
					break;
				}
			}
			st.Push(item);
		}
		else {
			double value = 0.0;
			if (IsConst(item)) {
				value = stod(item);
			}
			operands.insert({ item, value });
			postfix.push_back(item);
		}
	} 
	while (!st.IsEmpty()) {
		stackItem = st.Top();
		st.Pop();
		postfix.push_back(stackItem);
	}
}

double TArithmeticExpression::Calculate(const map<string, double>& values)
{
	for (auto& val : values) {
		try {
			operands.at(val.first) = val.second;
		}
		catch (out_of_range& e) {}
	}

	TStack<double> st;
	double leftOperand, rightOperand;
	for (string lexem : postfix) {
		if (lexem == "+") {
			rightOperand = st.Top();
			st.Pop();
			leftOperand = st.Top();
			st.Pop();
			st.Push(leftOperand + rightOperand);
		}
		else if (lexem == "-") {
			rightOperand = st.Top();
			st.Pop();
			leftOperand = st.Top();
			st.Pop();
			st.Push(leftOperand - rightOperand);
		}
		else if (lexem == "*") {
			rightOperand = st.Top();
			st.Pop();
			leftOperand = st.Top();
			st.Pop();
			st.Push(leftOperand * rightOperand);
		}
		else if (lexem == "/") {
			rightOperand = st.Top();
			st.Pop();
			leftOperand = st.Top();
			st.Pop();
			if (rightOperand == 0) {
				throw"Error";
			}
			st.Push(leftOperand / rightOperand);
		}
		else {
			st.Push(operands[lexem]);
		}
	}
	return st.Top();
}

double TArithmeticExpression::Calculate()
{
	return Calculate(operands);
}

bool TArithmeticExpression::isCorrectInfixExpression()
{
	int openParentheses = 0;
	int closeParentheses = 0;

	for (char c : infix)
		if (c == '(')
			openParentheses++;
		else if (c == ')')
			closeParentheses++;



	return openParentheses == closeParentheses;

}

void TArithmeticExpression::ShowPostfix()
{
	for (int i = 0; i < postfix.size(); i++)
	{
		cout << postfix[i] << " ";
	}
	cout << endl;
}
