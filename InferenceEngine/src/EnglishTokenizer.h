#pragma once

#include <Common/Common.h>
#include <unordered_set>

struct EnglishToken
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

// A specialised version of the Tokenizer which supports keywords, ignored words,
// and word sets (i.e. nouns and adjectives)
class EnglishTokenizer
{
private:
	enum class State
	{
		NONE = 0, WORD, OP, WS, END, ERR
	};
private:
	std::unordered_set<char> m_OperatorChars;
	std::unordered_set<char> m_WhiteSpaceChars;
	std::unordered_set<char> m_EndChars;
	std::unordered_set<String> m_Keywords;
	std::unordered_set<String> m_IgnoredWords;

	std::unordered_set<String> m_Nouns;
	std::unordered_set<String> m_Adjectives;

	String m_CurrentString;
	uint m_StringPos;
public:
	EnglishTokenizer();
	~EnglishTokenizer();

	void AddOperatorChars(const String& op);
	void AddWhitespaceChars(const String& ws);
	void AddEndChars(const String& end);
	void AddKeyword(const String& keyword);
	void AddIgnoredWord(const String& word);

	void LoadNouns(const String& path);
	void LoadAdjectives(const String& path);

	void SetString(const String& string);

	bool Next(EnglishToken& token);
private:
	State DetermineState(char c);
	EnglishToken::Type TokenizerStateToTokenType(State state);
};