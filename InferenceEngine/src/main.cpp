#include <Common/Common.h>
#include "Tokenizer.h"
#include "TruthTable.h"
#include "Language.h"

int main(int argc, char** argv)
{
	String input = "res/input.txt";
	std::vector<String> lines = ReadLinesFromFile(input);

	Tokenizer tokenizer;
	tokenizer.AddEndChars(";");
	tokenizer.AddOperatorChars("=>&");
	tokenizer.AddWhitespaceChars(" \n\r\t");
	tokenizer.SetString(lines[1]);

	Token* lastToken = nullptr;
	Token token;
	Statement* statement = nullptr;
	std::vector<Statement*> statements;
	while (tokenizer.Next(token))
	{
		if (lastToken == nullptr || lastToken->type == Token::Type::END_STATEMENT)
		{
			statement = new Statement();
			statements.push_back(statement);
		}
		lastToken = &token;
		switch (token.type)
		{
			case Token::Type::IDENTIFIER:
				statement->identifiers.push_back(token.token);
				break;
			case Token::Type::OPERATOR:
				statement->operators.push_back(ParseOperator(token.token));
				break;
		}
	}

	std::vector<TruthTable*> tts;
	for (Statement* statement : statements)
	{
		TruthTable* tt = new TruthTable();
		tt->SetStatement(statement);
		tts.push_back(tt);
	}

	for (TruthTable* truthTable : tts)
	{
		truthTable->GenerateTable();
		truthTable->PrintTable();
		std::cout << std::endl << std::endl;
	}

#if 0
	TruthTable tt;
	tt.PushToken(Token{ Token::Type::IDENTIFIER,  "a" });
	tt.PushToken(Token{ Token::Type::IDENTIFIER,  "b" });
	tt.PushToken(Token{ Token::Type::IDENTIFIER,  "c" });
	tt.PushToken(Token{ Token::Type::IDENTIFIER,  "d" });
	tt.PushToken(Token{ Token::Type::IDENTIFIER,  "e" });
	tt.PushToken(Token{ Token::Type::IDENTIFIER,  "f" });
	tt.PushToken(Token{ Token::Type::IDENTIFIER,  "g" });
	tt.PushToken(Token{ Token::Type::IDENTIFIER,  "h" });
	tt.PushToken(Token{ Token::Type::IDENTIFIER,  "i" });
	tt.GenerateTable();
	tt.PrintTable();
#endif

	system("PAUSE");
	return 0;
} 
