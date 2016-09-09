//#define DO_UNIT_TESTS // Uncomment to perform unit tests
#include "equation.h"
#include <iostream>

#ifdef DO_UNIT_TESTS
namespace Tests
{
	bool assert(Equation equation, float answer)
	{
		double result = equation.compute();
		if (result != answer)
		{
			std::cout << "Unit test failed: " << equation.toString() << "=" << result << " != " << answer << std::endl;
			return false;
		}
		return true;
	}
}
#endif

int main(int argc, char* argv[])
{
#ifdef DO_UNIT_TESTS
	if (Tests::assert(Equation("3+3"), 6.0) &&
		Tests::assert(Equation("5/2"), 2.5) &&
		Tests::assert(Equation("3+3*2"), 9.0) &&
		Tests::assert(Equation("(3+3)*2"), 12.0))
	{
		std::cout << "Unit tests suceeded." << std::endl;
	}
#endif

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
