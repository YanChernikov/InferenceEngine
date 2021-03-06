#pragma once

#include <Common/Common.h>
#include <vector>

// All supported operators
enum class Operator
{
	NONE = 0, AND, OR, IMPLICATION, NEGATION, DISJUNCTION, BICONDITIONAL
};

struct Statement
{
	std::vector<String> identifiers;
	std::vector<Operator> operators;
};


static Operator ParseOperator(const String& op)
{
	if (op == "&")		return Operator::AND;
	if (op == "|")		return Operator::OR;
	if (op == "=>")		return Operator::IMPLICATION;
	if (op == "~")		return Operator::NEGATION;
	if (op == "\\/")	return Operator::DISJUNCTION;
	if (op == "<=>")	return Operator::BICONDITIONAL;
	return Operator::NONE;
}

static Operator ParseEnglishOperator(const String& op)
{
	if (op == "and")	return Operator::AND;
	if (op == "or")		return Operator::OR;
	if (op == "then")	return Operator::IMPLICATION;
	if (op == "not")	return Operator::NEGATION;
	return Operator::NONE;
}

static String OperatorToString(Operator op)
{
	switch (op)
	{
		case Operator::AND:				return "&";
		case Operator::OR:				return "|";
		case Operator::IMPLICATION:		return "=>";
		case Operator::NEGATION:		return "~";
		case Operator::DISJUNCTION:		return "\\/";
		case Operator::BICONDITIONAL:	return "<=>";
	}
	return "None";
}