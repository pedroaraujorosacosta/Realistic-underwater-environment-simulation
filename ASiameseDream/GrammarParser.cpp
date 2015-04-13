#include <list>
#include <locale>
#include <iostream>
#include "GrammarParser.h"
#include "StringTokenizer.h"
#include "AllNodes.h"

using namespace GeneratorNodes;

// SyntaxException class, so we can communicate what kind of syntax error we found.
class SyntaxException
{
	std::string errorMsg;

public:
	SyntaxException(const std::string& errorMsg)
	{
		this->errorMsg = errorMsg;
	}

	std::string getErrorMsg()
	{
		return errorMsg;
	}

	static const std::string SYN_E_ANTECEDENT;
	static const std::string SYN_E_ASSOC_SYMB;
	static const std::string SYN_E_CON_NO_VAR;
	static const std::string SYN_E_CON_INVALID_SYMBOL;
};

const std::string SyntaxException::SYN_E_ANTECEDENT = "Antecendent must be a valid variable name";
const std::string SyntaxException::SYN_E_ASSOC_SYMB = "No association symbols after antecedent";
const std::string SyntaxException::SYN_E_CON_NO_VAR = "Consequent must have at least one variable";
const std::string SyntaxException::SYN_E_CON_INVALID_SYMBOL = "Consequent has an invalid symbol";

GrammarParser::GrammarParser()
{

}

GrammarParser::~GrammarParser()
{

}

bool GrammarParser::parseGrammar(const std::vector<std::string>& grammar, TokenInfo& parsedTree)
{
	std::vector<Node*> lines;
	int lineNum = 1;
	std::vector<std::string>::const_iterator it = grammar.begin();

	while (it != grammar.end())
	{
		try 
		{
			parseLine(*it++, parsedTree);
			if (parsedTree.tokenType == TokenInfo::TokenType::RULE_TOKEN)
				lines.push_back(parsedTree.tokenNode);
		}
		catch (SyntaxException e)
		{
			std::cout << "Invalid syntax on line " << lineNum << ": " << e.getErrorMsg() << "." << std::endl;
			return false;
		}

		lineNum++;
	}

	parsedTree.tokenNode = new ProgramNode(lines);

	return true;
}

void GrammarParser::parseLine(const std::string& line, TokenInfo& parsedTree)
{
	// strip whitespaces & tokenize line
	std::vector<std::string> tokenStrings;
	StringTokenizer::tokenize(line, tokenStrings, StringTokenizer::WHITESPACES);

	if (tokenStrings.size() > 0)
		parseRule(tokenStrings, parsedTree);
	else
		parsedTree.tokenType = TokenInfo::TokenType::EMPTY_TOKEN;
}

bool GrammarParser::parseRule(const std::vector<std::string>& tokenStrings, TokenInfo& parsedTree)
{
	IdentifierNode* antecedent;
	unsigned int i = 0;

	// check if the antecedent is a valid variable name
	if (!parseVar(tokenStrings[0]))
		throw SyntaxException(SyntaxException::SYN_E_ANTECEDENT);
	antecedent = new IdentifierNode(tokenStrings[0]);
		
	// rules have the form A := A B, so check if the second token string corresponds to the ':=' association symbols
	if (tokenStrings.size() < 2 || !parseAssociationSymbols(tokenStrings[1])) 
		throw SyntaxException(SyntaxException::SYN_E_ASSOC_SYMB);

	// a consequent has at least one variable and no invalid tokens
	bool foundInvalidToken = false;
	bool foundVariable = false;
	if (tokenStrings.size() < 3)
		throw SyntaxException(SyntaxException::SYN_E_CON_NO_VAR);

	std::vector<Node*> consequentNodes;
	for (std::vector<std::string>::const_iterator it = tokenStrings.begin()+2; it != tokenStrings.end(); it++)
	{
		Node* node = 0;
		if (parseVar(*it))
		{
			node = new IdentifierNode(*it);
			foundVariable = true;
		}
		else if (parseLeftBracket(*it))
			node = new LeftBracketNode();
		else if (parseRightBracket(*it))
			node = new RightBracketNode();
		else if (parsePlusSymbol(*it))
			node = new PlusNode();
		else if (parseMinusSymbol(*it))
			node = new MinusNode();
		else 
			foundInvalidToken = true;

		if (node)
			consequentNodes.push_back(node);
	}
	TokenSequenceNode* consequent = new TokenSequenceNode(consequentNodes);

	if (!foundVariable)
		throw SyntaxException(SyntaxException::SYN_E_CON_NO_VAR);

	if (foundInvalidToken)
		throw SyntaxException(SyntaxException::SYN_E_CON_INVALID_SYMBOL);

	parsedTree.tokenNode = new RuleNode(antecedent, consequent);
	parsedTree.tokenType = TokenInfo::RULE_TOKEN;

	return true;
}

// Lexer for variables
bool GrammarParser::parseVar(const std::string& str)
{
	std::locale loc;
	bool ret = true;

	if (!std::isalpha(str[0], loc))
		ret = false;

	for (unsigned int i = 1; i < str.length(); i++)
	{
		if (!std::isalnum(str[i], loc))
			ret = false;
	}

	return ret;
}

// Lexer for the two ':=' association symbols of our rules
bool GrammarParser::parseAssociationSymbols(const std::string& str)
{
	if (str.length() == 2 && str[0] == ':' || str[1] == '=')
		return true;
	else
		return false;
}

// Lexer for left bracket
bool GrammarParser::parseLeftBracket(const std::string& str)
{
	if (str.length() == 1 && str[0] == '[')
		return true;
	else
		return false;
}

// Lexer for right bracket
bool GrammarParser::parseRightBracket(const std::string& str)
{
	if (str.length() == 1 && str[0] == ']')
		return true;
	else
		return false;
}

// Lexer for plus symbol
bool GrammarParser::parsePlusSymbol(const std::string& str)
{
	if (str.length() == 1 && str[0] == '+')
		return true;
	else
		return false;
}

// Lexer for minus symbol
bool GrammarParser::parseMinusSymbol(const std::string& str)
{
	if (str.length() == 1 && str[0] == '-')
		return true;
	else
		return false;
}

int GrammarParser::createIndex(std::vector<std::string>& references, const std::string& newValue)
{
	int results = std::find(references.begin(), references.end(), newValue)
		- references.begin();
	if (results == references.size()) {
		references.push_back(newValue);
	}
	return results;
}