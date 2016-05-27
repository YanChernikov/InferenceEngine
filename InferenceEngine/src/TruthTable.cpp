#include "TruthTable.h"

TruthTable::TruthTable()
{

}

TruthTable::~TruthTable()
{
	delete m_Table;
}

void TruthTable::GenerateTable()
{
	const Statement& statement = *m_Statement;

	m_Width = statement.identifiers.size() + statement.operators.size();
	m_Height = pow(2, statement.identifiers.size());
	m_Table = new bool[m_Width * m_Height];
	memset(m_Table, 0, m_Width * m_Height);

	uint width = statement.identifiers.size();
	int index = 0;
	for (uint i = 0; i < width * m_Height; i++)
	{
		unsigned short shift = (width - 1) - i % width;
		unsigned short counter = i / width;
		int value = (counter >> shift) & 1;
		if (statement.identifiers[i % statement.identifiers.size()][0] == '~')
			value = 1 - value;
		m_Table[index] = value;
		if ((i + 1) % statement.identifiers.size() == 0)
			index += statement.operators.size();

		index++;
	}

	if (statement.operators.size() == 0)
		return;

	// int op = 0;
	uint lid = GetColumn(statement.identifiers[0]);
	for (uint i = 1, o = 0; i < statement.identifiers.size(); i++, o++)
	{
		uint rid = GetColumn(statement.identifiers[i]);
		Operator op = statement.operators[o];
		uint column = o + statement.identifiers.size();
		for (uint row = 0; row < m_Height; row++)
		{
			bool l = GetValue(lid, row);
			bool r = GetValue(rid, row);
			m_Table[column + row * m_Width] = PerformOperation(l, r, op);
		}
		lid = column;
	}
	
}

void TruthTable::PrintTable()
{
	int space = 0;
	std::vector<uint> spacing;
	for (uint i = 0; i < m_Statement->identifiers.size(); i++)
	{
		spacing.push_back(1);
		std::cout << " " << m_Statement->identifiers[i] << " ";
		space += 2 + m_Statement->identifiers[i].size();
		if (i < m_Statement->identifiers.size() - 1)
		{
			std::cout << "|";
			space++;
		}
	}

	if (m_Statement->operators.size() > 0)
	{
		std::cout << "| ";
		uint id = 0;
		String opString = m_Statement->identifiers[id++] + " ";
		opString += OperatorToString(m_Statement->operators[0]) + " " + m_Statement->identifiers[id++];
		std::cout << opString << " | ";
		spacing.push_back(opString.length());
		for (uint op = 1; op < m_Statement->operators.size(); op++)
		{
			opString += String(" ") + OperatorToString(m_Statement->operators[op]) + " " + m_Statement->identifiers[id++];
			std::cout << " " << opString << " | ";
			spacing.push_back(opString.length());
		}
	}

	std::cout << std::endl;
	for (int i = 0; i < space; i++)
		std::cout << "_";
	std::cout << std::endl;
	std::cout << std::endl;

	for (uint i = 0; i < m_Width * m_Height; i++)
	{
		for (uint s = 0; s < spacing[i % m_Width] / 2; s++)
			std::cout << " ";
		std::cout << " " << m_Table[i] << " ";
		for (uint s = 0; s < spacing[i % m_Width] / 2; s++)
			std::cout << " ";
		if ((i + 1) % m_Width == 0)
		{
			std::cout << "|" << std::endl;
			for (int i = 0; i < space; i++)
				std::cout << "-";
			std::cout << std::endl;
		}
		else
		{
			std::cout << "|";
		}
	}
}

bool TruthTable::GetValue(const String& id, uint row)
{
	uint i;
	for (i = 0; i < m_Statement->identifiers.size(); i++)
	{
		if (m_Statement->identifiers[i] == id)
			break;
	}
	ASSERT(i < m_Statement->identifiers.size());
	return m_Table[i + row * m_Width];
}

bool TruthTable::PerformOperation(bool l, bool r, Operator op)
{
	switch (op)
	{
		case Operator::AND:
			return l && r;
		case Operator::OR:
		case Operator::DISJUNCTION:
			return l || r;
		case Operator::IMPLICATION:
			return !l || r;
		case Operator::NEGATION:
			return !l;
		case Operator::BICONDITIONAL:
			return l == r;
	}
	ASSERT(false);
	return false;
}

uint TruthTable::GetColumn(const String& id)
{
	for (uint i = 0; i < m_Statement->identifiers.size(); i++)
	{
		if (m_Statement->identifiers[i] == id)
			return i;
	}
	return 0;
}

uint TruthTable::Query(const String& id) const
{
	const Statement& statement = *m_Statement;
	for (uint i = 0; i < statement.identifiers.size(); i++)
	{
		const String& s = statement.identifiers[i];
		if (s == id)
		{
			if (statement.operators.size() == 0)
				return Count(i);
			else
				return Count(statement.identifiers.size() + (i > 0 ? i - 1 : 0));
		}
	}
	return 0;
}

uint TruthTable::QueryLast() const
{
	return Count(m_Width - 1);
}

uint TruthTable::Count(uint column) const
{
	uint result = 0;
	for (uint row = 0; row < m_Height; row++)
		result += GetValue(column, row);
	return result;
}