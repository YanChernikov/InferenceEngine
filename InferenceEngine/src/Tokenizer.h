#pragma once

#include <Common/Common.h>
#include <unordered_set>

struct Token
{
	enum class Type
	{
		UNKNOWN = 0, IDENTIFIER, OPERATOR, WHITESPACE, END_STATEMENT
	};

	Type type;
	String token;

public:
	static String TokenTypeToString(Type type);
};

class Tokenizer
{
private:
	enum class State
	{
		NONE = 0, ID, OP, WS, END, ERR
	};
private:
	std::unordered_set<char> m_OperatorChars;
	std::unordered_set<char> m_WhiteSpaceChars;
	std::unordered_set<char> m_EndChars;

	String m_CurrentString;
	uint m_StringPos;
public:
	Tokenizer();
	~Tokenizer();

	void AddOperatorChars(const String& op);
	void AddWhitespaceChars(const String& ws);
	void AddEndChars(const String& end);

	void SetString(const String& string);

	bool Next(Token& token);
private:
	State DetermineState(char c);
	Token::Type TokenizerStateToTokenType(State state);
};