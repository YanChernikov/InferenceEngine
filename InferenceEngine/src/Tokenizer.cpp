#include "Tokenizer.h"


String Token::TokenTypeToString(Type type)
{
	switch (type)
	{
		case Token::Type::IDENTIFIER:		return "Identifier";
		case Token::Type::OPERATOR:			return "Operator";
		case Token::Type::WHITESPACE:		return "Whitespace";
		case Token::Type::END_STATEMENT:	return "End Statement";
	}
	return "Unknown";
}

Tokenizer::Tokenizer()
{
}

Tokenizer::~Tokenizer()
{
}

void Tokenizer::AddOperatorChars(const String& op)
{
	for (uint i = 0; i < op.size(); i++)
		m_OperatorChars.insert(op[i]);
}

void Tokenizer::AddWhitespaceChars(const String& ws)
{
	for (uint i = 0; i < ws.size(); i++)
		m_WhiteSpaceChars.insert(ws[i]);
}

void Tokenizer::AddEndChars(const String& end)
{
	for (uint i = 0; i < end.size(); i++)
		m_EndChars.insert(end[i]);
}

void Tokenizer::SetString(const String& string)
{
	m_CurrentString = string;
	m_StringPos = 0;
}

Tokenizer::State Tokenizer::DetermineState(char c)
{
	if (m_OperatorChars.find(c) != m_OperatorChars.end())		return State::OP;
	if (m_WhiteSpaceChars.find(c) != m_WhiteSpaceChars.end())	return State::WS;
	if (m_EndChars.find(c) != m_EndChars.end())					return State::END;
	return State::ID;
}

Token::Type Tokenizer::TokenizerStateToTokenType(Tokenizer::State state)
{
	switch (state)
	{
		case Tokenizer::State::OP:		return Token::Type::OPERATOR;
		case Tokenizer::State::ID:		return Token::Type::IDENTIFIER;
		case Tokenizer::State::WS:		return Token::Type::WHITESPACE;
		case Tokenizer::State::END:		return Token::Type::END_STATEMENT;
	}
	return Token::Type::UNKNOWN;
}

bool Tokenizer::Next(Token& token)
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