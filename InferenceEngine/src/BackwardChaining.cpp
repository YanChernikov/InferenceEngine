#include "BackwardChaining.h"

#include <stack>

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
	chain.push_back(goal);
//	const String* current = &goal;
	uint count = 0;
	
	// checks to see if one of the identifiers are the goal
	for (String& id : m_Identifiers)
	{
		if (id == goal)
			return chain;
	}

	std::stack<String> ids;
	ids.push(goal);
	String current;
 	for (int i = 0; i < m_Statements.size(); i++)
 	{
		if (ids.empty())
			break;
		String current = ids.top();
		ids.pop();
 		for (Statement* s : m_Statements)
 		{
			if (s->identifiers.back() == current)
 			{
				for (int j = 0; j < s->identifiers.size() - 1; j++)
				{
					const String& id = s->identifiers[j];
					if (VectorContains(chain, id))
						continue;

 					chain.push_back(id);
					ids.push(id);
				}
 			}
 		}
 	}

	if (chain.size() > 1)
		std::reverse(chain.begin(), chain.end());

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