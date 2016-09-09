#include "equation.h"
#include <iostream>

int main(int argc, char* argv[])
{
	std::string input;
	if (argc < 2)
	{
		std::cout << "Please input an equation." << std::endl;
		std::getline(std::cin, input);
	}
	else
	{
		input = argv[1];
	}
	try
	{
		Equation equation(input);
		std::cout << equation.toString() << " = " << equation.compute() << std::endl;
	}
	catch (Equation::SyntaxError& e)
	{
		std::cout << "Syntax error. Try again." << std::endl;
		main(1, argv);
	}
	
	return 0;
}
