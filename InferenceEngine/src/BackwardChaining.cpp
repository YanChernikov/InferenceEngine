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
	// vector of strings to store the result
	std::vector<String> chain;
	// pushes back initial goal state
	chain.push_back(goal);
//	const String* current = &goal;
	uint count = 0;
	
	// checks to see if one of the identifiers are the goal
	for (String& id : m_Identifiers)
	{
		if (id == goal)
			return chain;
	}

	// stack to keep track of id's that need to be visited.
	std::stack<String> ids;
	ids.push(goal);
	String current;

	// loops through all the statements. 
 	for (int i = 0; i < m_Statements.size(); i++)
 	{
		if (ids.empty())
			break;
		// sets the current statement/id to top of stack.
		String current = ids.top();
		ids.pop();

 		for (Statement* s : m_Statements)
 		{	
 			// checks to see if the current identifier on the right hand side is
 			// equal to the current identifier being processed
			if (s->identifiers.back() == current)
 			{
 				// loops through the identifiers not taking into account
 				// the right most side of the statement. 
				for (int j = 0; j < s->identifiers.size() - 1; j++)
				{
					// sets id as as the identifiers at index J, checks to see if it contained
					// within the result, pushes back id into stack and result. 
					const String& id = s->identifiers[j];
					if (VectorContains(chain, id))
						continue;

 					chain.push_back(id);
					ids.push(id);
				}
 			}
 		}
 	}

 	// if the chain size is greater than one
 	// meaning some result has been found, it will reverse the order of results
 	// to print in the desired format. 
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