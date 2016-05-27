#include <Common/Common.h>
#include "Tokenizer.h"
#include "EnglishTokenizer.h"
#include "TruthTable.h"
#include "Language.h"
#include "ForwardChaining.h"
#include "BackwardChaining.h"

#include <algorithm>

// Used for debugging
#define PRINT_TABLES 0

// Attempt to solve using the truth table solution
static void TruthTableSolution(const String& goal, std::vector<Statement*>& statements, bool allowSpaces = false)
{
	String string = goal;
	// Remove spaces since they should be ignored (i.e. variables should not be split)
	if (allowSpaces)
		string.erase(std::remove(string.begin(), string.end(), ' '), string.end());

	std::vector<TruthTable*> tts;
	for (Statement* statement : statements)
	{
		TruthTable* tt = new TruthTable();
		tt->SetStatement(statement);
		tts.push_back(tt);
	}

	if (allowSpaces)
		std::cout << "Solving..." << std::endl;
	else
		std::cout << "Solving for query '" << goal << "' using Truth Tables." << std::endl;

	uint count = 0;
	for (TruthTable* truthTable : tts)
	{
		truthTable->GenerateTable();
#if PRINT_TABLES
		truthTable->PrintTable();
		std::cout << std::endl << std::endl;
#endif
		if (allowSpaces)
			count += truthTable->QueryLast();
		else
			count += truthTable->Query(string);
	}

	if (count > 0)
		std::cout << "Yes: " << count << std::endl;
	else
		std::cout << "No" << std::endl;
}

// Attempt to solve using the forward chaining solution
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

// Attempt to solve using the backward chaining solution
static void BackwardChainingSolution(const String& goal, std::vector<Statement*>& statements)
{
	BackwardChaining bc;
	for (Statement* statement : statements)
	{
		if (statement->operators.size() == 0)
			bc.AddIdentifier(statement->identifiers.front());
		else
			bc.AddStatement(statement);
	}

	std::cout << "Solving for query '" << goal << "' using Backward Chaining." << std::endl;
	std::vector<String> chain = bc.Solve(goal);
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

static std::vector<Statement*> ParseStatements(const String& input, bool allowSpaces = false)
{
	String string = input;
	// Remove spaces since they should be ignored (i.e. variables should not be split)
	if (allowSpaces)
		string.erase(std::remove(string.begin(), string.end(), ' '), string.end());

	string += ";";

	Tokenizer tokenizer;
	tokenizer.AddEndChars(";");
	tokenizer.AddOperatorChars("<=>&|\\/");
	tokenizer.AddWhitespaceChars(" \n\r\t");
	tokenizer.SetString(string);

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

// Parse English sentence and try and convert it into propositional logic
static String ParseEnglish(const String& input)
{
	std::cout << input << std::endl;

	EnglishTokenizer tokenizer;
	tokenizer.AddEndChars(";");
	tokenizer.AddKeyword("and");
	tokenizer.AddKeyword("or");
	tokenizer.AddKeyword("not");
	tokenizer.AddKeyword("then");
	tokenizer.LoadNouns("res/nounlist.txt");
	tokenizer.LoadAdjectives("res/adjectives.txt");
	tokenizer.AddWhitespaceChars(" \n\r\t");
	tokenizer.SetString(input);

	Statement* statement = new Statement();
	EnglishToken token;
	String string;
	while (tokenizer.Next(token))
	{
		switch (token.type)
		{
			case EnglishToken::Type::IDENTIFIER:
				statement->identifiers.push_back(token.token);
				string += token.token + " ";
				break;
			case EnglishToken::Type::OPERATOR:
				statement->operators.push_back(ParseEnglishOperator(token.token));
				Operator op = ParseEnglishOperator(token.token);
				string += OperatorToString(op);
				if (op != Operator::NEGATION)
					string += " ";
				break;
		}
	}
	std::cout << string << std::endl;
	delete statement;
	return string;
}

// Print program usage
static void PrintUsage()
{
	std::cout << "\tUsage: iengine method(TT|FC|BC) filename" << std::endl;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Error: Incorrect input arguments!" << std::endl;
		PrintUsage();
		return 1;
	}

	String method = argv[1];
	String input = argv[2];
	std::vector<String> lines = ReadLinesFromFile(input);

	const String* knowledgeBase = nullptr;
	const String* query = nullptr;
	for (int i = 0; i < lines.size(); i++)
	{
		const String& line = lines[i];
		if (Contains(line, "TELL"))
			knowledgeBase = &lines[++i];
		else if (Contains(line, "ASK"))
			query = &lines[++i];
	}

	// Apply natural language processing and solve via truth table
	if (method == "ENG")
	{
		String sentence = *knowledgeBase + ";";
		sentence = ParseEnglish(sentence);
		std::vector<Statement*> statements = ParseStatements(sentence, true);
		TruthTableSolution(*query, statements, true);
		return 0;
	}

	if (!knowledgeBase || !query)
	{
		std::cout << "Error: invalid input file format!" << std::endl;
		PrintUsage();
		return 2;
	}

	std::vector<Statement*> statements = ParseStatements(*knowledgeBase);
	if (method == "TT")
		TruthTableSolution(*query, statements);
	else if (method == "FC")
		ForwardChainingSolution(*query, statements);
	else if (method == "BC")
		BackwardChainingSolution(*query, statements); 
	else
	{
		std::cout << "Error: Invalid method!";
		PrintUsage();
		return 3;
	}
	return 0;
} 
