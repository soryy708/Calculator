#include "equation.h"
#include <vector>
#include <regex>
#include <sstream>

class Token
{
public:
	enum class Type
	{
		VALUE,
		OPERATION
	};
	union Value
	{
		double value;
		char operation;
	};

private:
	const Type type;
	Value value;

public:
	const Type getType() const { return type; }
	const Value getValue() const { return value; }
	const std::string toString() const
	{
		std::string str;
		std::stringstream ss;
		switch (type)
		{
		case(Type::VALUE) :
			ss << value.value;
			ss >> str;
			break;
		case(Type::OPERATION) :
			str = value.operation;
			break;
		}
		return str;
	}

	Token(const Type type, const Value value) :
		type(type),
		value(value)
	{
	}
	Token(const char operation) :
		type(Type::OPERATION)
	{
		value.operation = operation;
	}
	Token(const double value) :
		type(Type::VALUE)
	{
		this->value.value = value;
	}
};

std::vector<Token> tokenize(std::string str)
{
	std::vector<Token> tokens;
	const std::regex re{ "(\\d+(\\.\\d*)?)|([\\+\\-\\*\\/])" };
	std::for_each(std::sregex_iterator(cbegin(str), cend(str), re), std::sregex_iterator(), [&](const auto& i)
	{
		if (i[1].length() > 0U) // Value
		{
			std::stringstream ss(i[1]);
			double value;
			ss >> value;
			tokens.emplace_back(value);
		}
		else if (i[0].length() > 0U) // Operation
		{
			std::stringstream ss(i[0]);
			char value;
			ss >> value;
			tokens.emplace_back(value);
		}
	});

	return tokens;
}

std::string sanitize(std::string str)
{
	std::string sanitized;

	for (unsigned int i = 0; i < str.length(); ++i)
	{
		if (str[i] == '0' || str[i] == '1' || str[i] == '2' || str[i] == '3' || str[i] == '4' || str[i] == '5' ||
			str[i] == '6' || str[i] == '7' || str[i] == '8' || str[i] == '9' || str[i] == '+' || str[i] == '-' ||
			str[i] == '*' || str[i] == '/' || str[i] == '.')
		{
			sanitized.push_back(str[i]);
		}
	}

	return sanitized;
}

bool isValid(const std::vector<Token> tokens)
{
	for (unsigned int i = 0; i < tokens.size(); ++i)
	{
		if (i % 2 == 0) // Should be a value
		{
			if (tokens[i].getType() != Token::Type::VALUE)
				return false;
		}
		else if (i % 2 == 1) // Should be an operation
		{
			if (tokens[i].getType() != Token::Type::OPERATION)
				return false;
		}
	}
	return true;
}

class Equation::Pimpl
{
private:
	std::vector<Token> tokens;

public:
	friend Equation;

	Pimpl(const std::vector<Token> tokens) :
		tokens(tokens)
	{
	}
};

const std::string Equation::toString() const
{
	std::string str;
	for (auto it : pimpl->tokens)
	{
		str += it.toString();
	}
	return str;
}

const double Equation::computeInOrder() const
{
	double answer = 0.0;
	if (isValid(pimpl->tokens))
	{
		if (pimpl->tokens[0].getType() == Token::Type::VALUE)
		{
			answer = pimpl->tokens[0].getValue().value;
		}
		else
		{
			throw(SyntaxError());
		}
		for (unsigned int i = 1; i < pimpl->tokens.size(); i+=2)
		{
			if (pimpl->tokens[i].getType() == Token::Type::OPERATION)
			{
				if (pimpl->tokens.size() > i + 1 && pimpl->tokens[i + 1].getType() == Token::Type::VALUE)
				{
					switch (pimpl->tokens[i].getValue().operation)
					{
					case('+') :
						answer += pimpl->tokens[i + 1].getValue().value;
						break;
					case('-') :
						answer -= pimpl->tokens[i + 1].getValue().value;
						break;
					case('*') :
						answer *= pimpl->tokens[i + 1].getValue().value;
						break;
					case('/') :
						answer /= pimpl->tokens[i + 1].getValue().value;
						break;
					}
				}
			}
			else
			{
				throw(SyntaxError());
			}
		}
	}
	else
	{
		throw(SyntaxError());
	}
	return answer;
}

Equation::Equation(std::string str) :
	pimpl(new Pimpl(tokenize(sanitize(str))))
{
}

Equation::Equation(Equation& other) :
	pimpl(new Pimpl(*other.pimpl))
{
}

Equation::Equation(Equation&& other) :
	pimpl(other.pimpl)
{
	other.pimpl = nullptr;
}

Equation::~Equation()
{
	if (pimpl)
	{
		delete pimpl;
	}
}
