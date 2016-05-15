#pragma once

#include "Tokenizer.h"
#include "Language.h"

class TruthTable
{
private:
	bool* m_Table;
	uint m_Width, m_Height;
	Statement* m_Statement;
public:
	TruthTable();
	~TruthTable();

	void GenerateTable();
	void PrintTable();

	inline void SetStatement(Statement* statement) { m_Statement = statement; }
	inline bool* GetRow(uint row) { return &m_Table[row * m_Width]; }
};
