#include <Common/Common.h>
#include "Tokenizer.h"

int main(int argc, char** argv)
{
	String input = "res/input.txt";
	std::vector<String> lines = ReadLinesFromFile(input);

	Tokenizer tokenizer;
	tokenizer.AddEndChars(";");
	tokenizer.AddOperatorChars("=>&");
	tokenizer.AddWhitespaceChars(" \n\r\t");
	tokenizer.SetString(lines[1]);

	Token token;
	while (tokenizer.Next(token))
		std::cout << Token::TokenTypeToString(token.type) << "\t\t" << token.token << std::endl;

	system("PAUSE");
	return 0;
} 
