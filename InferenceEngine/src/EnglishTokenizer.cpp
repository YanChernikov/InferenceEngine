#include "EnglishTokenizer.h"
#include <algorithm>

String EnglishToken::TokenTypeToString(Type type)
{
	switch (type)
	{
		case EnglishToken::Type::IDENTIFIER:		return "Identifier";
		case EnglishToken::Type::OPERATOR:			return "Operator";
		case EnglishToken::Type::WHITESPACE:		return "Whitespace";
		case EnglishToken::Type::END_STATEMENT:		return "End Statement";
	}
	return "Unknown";
}

EnglishTokenizer::EnglishTokenizer()
{
}

EnglishTokenizer::~EnglishTokenizer()
{
}

void EnglishTokenizer::AddOperatorChars(const String& op)
{
	for (uint i = 0; i < op.size(); i++)
		m_OperatorChars.insert(op[i]);
}

void EnglishTokenizer::AddWhitespaceChars(const String& ws)
{
	for (uint i = 0; i < ws.size(); i++)
		m_WhiteSpaceChars.insert(ws[i]);
}

void EnglishTokenizer::AddEndChars(const String& end)
{
	for (uint i = 0; i < end.size(); i++)
		m_EndChars.insert(end[i]);
}

void EnglishTokenizer::AddKeyword(const String& keyword)
{
	m_Keywords.insert(keyword);
}

void EnglishTokenizer::AddIgnoredWord(const String& word)
{
	m_IgnoredWords.insert(word);
}

void EnglishTokenizer::SetString(const String& string)
{
	m_CurrentString = string;
	m_StringPos = 0;
}

EnglishTokenizer::State EnglishTokenizer::DetermineState(char c)
{
	if (m_WhiteSpaceChars.find(c) != m_WhiteSpaceChars.end())	return State::WS;
	if (m_EndChars.find(c) != m_EndChars.end())					return State::END;
	return State::WORD;
}

EnglishToken::Type EnglishTokenizer::TokenizerStateToTokenType(EnglishTokenizer::State state)
{
	switch (state)
	{
		case EnglishTokenizer::State::OP:		return EnglishToken::Type::OPERATOR;
		case EnglishTokenizer::State::WORD:		return EnglishToken::Type::IDENTIFIER;
		case EnglishTokenizer::State::WS:		return EnglishToken::Type::WHITESPACE;
		case EnglishTokenizer::State::END:		return EnglishToken::Type::END_STATEMENT;
	}
	return EnglishToken::Type::UNKNOWN;
}

void EnglishTokenizer::LoadNouns(const String& path)
{
	std::vector<String> n = ReadLinesFromFile(path);
	for (const String& s : n)
		m_Nouns.insert(s);
}
void EnglishTokenizer::LoadAdjectives(const String& path)
{
	std::vector<String> n = ReadLinesFromFile(path);
	for (const String& s : n)
		m_Adjectives.insert(s);
}

void StripPunctuation(String& string)
{
	uint pos = string.find("ing");
	if (pos != String::npos)
		string.erase(pos, 3);
	string.erase(std::remove(string.begin(), string.end(), ','), string.end());
	string.erase(std::remove(string.begin(), string.end(), '.'), string.end());
}

bool EnglishTokenizer::Next(EnglishToken& token)
{
	if (m_StringPos >= m_CurrentString.size())
		return false;

	State prevState = State::NONE;
	String buffer;
	for ( ; m_StringPos < m_CurrentString.size(); m_StringPos++)
	{
		char c = m_CurrentString[m_StringPos];
		State state = DetermineState(c);
		if (prevState == State::NONE)
			prevState = state;
		else if (state != prevState)
		{
			if (prevState == State::WS)
			{
				buffer.clear();
			}
			else
			{
				if (prevState == State::WORD)
				{
					if (m_Keywords.find(buffer) != m_Keywords.end())
						token.type = EnglishToken::Type::OPERATOR;
					else if (m_Nouns.find(buffer) != m_Nouns.end())
						token.type = EnglishToken::Type::IDENTIFIER;
					else if (m_Adjectives.find(buffer) != m_Adjectives.end())
						token.type = EnglishToken::Type::IDENTIFIER;
					else
					{
						StripPunctuation(buffer);
						if (m_Nouns.find(buffer) != m_Nouns.end())
							token.type = EnglishToken::Type::IDENTIFIER;
						else
						{
							buffer.clear();
							continue;
						}
					}
				}
				else
					token.type = TokenizerStateToTokenType(prevState);
				token.token = buffer;
				return true;
			}
			prevState = state;
		}
		buffer += c;
	}
	return false;
}