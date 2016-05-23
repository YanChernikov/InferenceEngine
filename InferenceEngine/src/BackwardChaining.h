#pragma once

#include "Tokenizer.h"
#include "Language.h"

class BackwardChaining
{
private:
	std::vector<String> m_Identifiers;
	std::vector<String> m_VisitedIdentifiers;
	std::vector<Statement*> m_Statements;
	std::vector<Statement*> m_VisitedStatements;
public:
	BackwardChaining();
	~BackwardChaining();

	void AddIdentifier(const String& id);
	void AddStatement(Statement* statement);
	std::vector<String> Solve(const String& goal);

	bool Try(std::vector<String>& chain);

	bool VectorContains(const std::vector<String>& vector, const String& string);
	bool VectorContains(const std::vector<Statement*>& vector, const Statement* statement);
};