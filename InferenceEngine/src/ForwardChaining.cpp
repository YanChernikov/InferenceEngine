#include "ForwardChaining.h"

ForwardChaining::ForwardChaining()
{

}

ForwardChaining::~ForwardChaining()
{
	for (Statement* s : m_VisitedStatements)
		delete s;
}

void ForwardChaining::AddIdentifier(const String& id)
{
	m_Identifiers.push_back(id);
}

void ForwardChaining::AddStatement(Statement* statement)
{
	m_Statements.push_back(statement);
}

std::vector<String> ForwardChaining::Solve(const String& goal)
{
	// Start with identifiers
	std::vector<String> chain;
	while (Try(chain))
	{
statement:
		String& current = chain.back();
		m_VisitedIdentifiers.push_back(current);
		if (current == goal)
			return chain;
		for (Statement* s : m_Statements)
		{
			bool candidate = false;
			for (int i = 0; i < s->identifiers.size() - 1; i++)
			{
				const String& identifier = s->identifiers[i];
				if (identifier == current)
				{
					candidate = true;
					break;
				}
			}
			if (candidate)
			{
				bool canExpand = true;
				for (int i = 0; i < s->identifiers.size() - 1; i++)
				{
					const String& identifier = s->identifiers[i];
					if (!VectorContains(chain, identifier))
					{
						canExpand = false;
						break;
					}
				}
				if (canExpand)
				{
					chain.push_back(s->identifiers.back());
					m_VisitedStatements.push_back(s);
					// Go through the statements again with a new value in the chain
					goto statement;
				}
			}
		}
	}


	return std::vector<String>();
}

// Tries to add an identifier to the chain
bool ForwardChaining::Try(std::vector<String>& chain)
{
	for (String& id : m_Identifiers)
	{
		if (!VectorContains(m_VisitedIdentifiers, id))
		{
			m_VisitedIdentifiers.push_back(id);
			chain.push_back(id);
			return true;
		}
	}

	for (Statement* s : m_Statements)
	{
		if (!VectorContains(m_VisitedStatements, s))
		{
			m_VisitedStatements.push_back(s);
			chain.push_back(s->identifiers.front());
			return true;
		}
	}
	return false;
}

// Check if the given vector contains a string
bool ForwardChaining::VectorContains(const std::vector<String>& vector, const String& string)
{
	for (const String& s : vector)
	{
		if (s == string)
			return true;
	}
	return false;
}

// Check if the given vector contains a statement
bool ForwardChaining::VectorContains(const std::vector<Statement*>& vector, const Statement* statement)
{
	for (const Statement* s : vector)
	{
		if (s == statement)
			return true;
	}
	return false;
}