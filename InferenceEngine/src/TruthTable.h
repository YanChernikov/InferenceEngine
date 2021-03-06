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

	bool GetValue(const String& id, uint row);
	bool PerformOperation(bool l, bool r, Operator op);
	uint GetColumn(const String& id);

	uint Query(const String& id) const;
	uint QueryLast() const;
	uint Count(uint column) const;

	inline void SetStatement(Statement* statement) { m_Statement = statement; }
	// Retrieves a pointer to the beginning of a row
	inline bool* GetRow(uint row) { return &m_Table[row * m_Width]; }
	// Returns the value of a cell in the truth table
	inline bool GetValue(uint column, uint row) const { return m_Table[column + row * m_Width]; }
};
