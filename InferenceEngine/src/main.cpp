#include <Common/Common.h>
#include "Tokenizer.h"
#include "TruthTable.h"
#include "Language.h"
#include "ForwardChaining.h"

static void TruthTableSolution(const String& goal, std::vector<Statement*>& statements)
{
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
}

static void ForwardChainingSolution(const String& goal, std::vector<Statement*>& statements)
{
	ForwardChaining fc;
	for (Statement* statement : statements)
	{
		if (statement->operators.size() == 0)
			fc.AddIdentifier(statement->identifiers.front());
		else
			fc.AddStatement(statement);
	}
	
	std::cout << "Solving for query '" << goal << "' using Forward Chaining." << std::endl;
	std::vector<String> chain = fc.Solve(goal);
	if (chain.size())
	{
		std::cout << "Solution found:" << std::endl;
		std::cout << "\t";
		for (uint i = 0; i < chain.size(); i++)
		{
			std::cout << chain[i];
			if (i < chain.size() - 1)
				std::cout << ", ";
		}
		std::cout << std::endl;
	}
	else
	{
		std::cout << "No solution found." << std::endl;
	}
}

static std::vector<Statement*> ParseStatements(const String& input)
{
	Tokenizer tokenizer;
	tokenizer.AddEndChars(";");
	tokenizer.AddOperatorChars("=>&|");
	tokenizer.AddWhitespaceChars(" \n\r\t");
	tokenizer.SetString(input);

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
	return statements;
}

int main(int argc, char** argv)
{
	String input = "res/input.txt";
	std::vector<String> lines = ReadLinesFromFile(input);
	std::vector<Statement*> statements = ParseStatements(lines[1]);
	
	String goal = "f";
	ForwardChainingSolution(goal, statements);
	// TruthTableSolution(goal, statements);

	system("PAUSE");
	return 0;
} 
