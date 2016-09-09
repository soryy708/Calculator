#ifndef EQUATION_H_INCLUDED
#define EQUATION_H_INCLUDED

#include <string>
#include <exception>

class Equation
{
public:
	class SyntaxError : public std::exception
	{
	};

private:
	class Pimpl;
	Pimpl* pimpl;

public:
	const std::string toString() const;
	const double compute() const;

	Equation(std::string str);
	Equation(Equation& other);
	Equation(Equation&& other);
	~Equation();
};

#endif
