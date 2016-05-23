#include "BackwardChaining.h"

BackwardChaining::BackwardChaining()
{

}

BackwardChaining::~BackwardChaining()
{

}

void BackwardChaining::AddIdentifier(const String& id)
{
	m_Identifiers.push_back(id);
}

void BackwardChaining::AddStatement(Statement* statement)
{
	m_Statements.push_back(statement);
}

std::vector<String> BackwardChaining::Solve(const String& goal)
{
	std::vector<String> chain;
	String current = goal;
	uint count = 0;
	
	// checks to see if one of the identifiers are the goal
	for (String& id : m_Identifiers)
	{
		if (id == goal)
		{
			m_VisitedIdentifiers.push_back(id);
			chain.push_back(id);
			return chain;
		}
	}
	
	while (!(count == m_Statements.size()))
	{
		for (Statement* s : m_Statements)
		{
				m_VisitedStatements.push_back(s);
				if (s->identifiers.back() == current)
				{
					chain.push_back(s->identifiers.front());
					current = s->identifiers.front();
				}
		}
		count++;
	}
	if (chain.size() > 1)
	{
		std::reverse(chain.begin(), chain.end());
		chain.push_back(goal);
	}
	return chain;
}


bool BackwardChaining::VectorContains(const std::vector<String>& vector, const String& string)
{
	for (const String& s : vector)
	{
		if (s == string)
			return true;
	}
	return false;
}

bool BackwardChaining::VectorContains(const std::vector<Statement*>& vector, const Statement* statement)
{
	for (const Statement* s : vector)
	{
		if (s == statement)
			return true;
	}
	return false;
}