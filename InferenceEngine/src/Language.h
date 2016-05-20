#pragma once

#include <Common/Common.h>
#include <vector>

enum class Operator
{
	NONE = 0, AND, OR, IMPLICATION
};

struct Statement
{
	std::vector<String> identifiers;
	std::vector<Operator> operators;
};


static Operator ParseOperator(const String& op)
{
	if (op == "&")	return Operator::AND;
	if (op == "|")	return Operator::OR;
	if (op == "=>")	return Operator::IMPLICATION;
	return Operator::NONE;
}

static String OperatorToString(Operator op)
{
	switch (op)
	{
		case Operator::AND:				return "&";
		case Operator::OR:				return "|";
		case Operator::IMPLICATION:		return "=>";
	}
	return "None";
}