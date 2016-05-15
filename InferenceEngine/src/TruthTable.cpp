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
	m_Width = m_Statement->identifiers.size() + m_Statement->operators.size();
	m_Height = pow(2, m_Statement->identifiers.size());
	m_Table = new bool[m_Width * m_Height];
	memset(m_Table, 0, m_Width * m_Height);

	for (uint i = 0; i < m_Width * m_Height; i++)
	{
		short shift = (m_Width - 1) - i % m_Width;
		short counter = i / m_Width;
		m_Table[i] = (counter >> shift) & 1;
	}

	
}

void TruthTable::PrintTable()
{
	int space = 0;
	for (int i = 0; i < m_Statement->identifiers.size(); i++)
	{
		std::cout << " " << m_Statement->identifiers[i] << " ";
		space += 2 + m_Statement->identifiers[i].size();
		if (i < m_Statement->identifiers.size() - 1)
		{
			std::cout << "|";
			space++;
		}
	}
	std::cout << std::endl;
	for (int i = 0; i < space; i++)
		std::cout << "_";
	std::cout << std::endl;
	std::cout << std::endl;

	for (int i = 0; i < m_Width * m_Height; i++)
	{
		std::cout << " " << m_Table[i] << " ";
		if ((i + 1) % m_Width == 0)
		{
			std::cout << std::endl;
			for (int i = 0; i < space; i++)
				std::cout << "-";
			std::cout << std::endl;
		}
		else
		{
			if (i < m_Width * m_Height - 1)
				std::cout << "|";
		}
	}
}
