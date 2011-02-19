/*
 *  propertytreeparser.h
 *  config
 *
 *  Created by Peter Backman on 2/10/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef CONFIG_PROPERTYTREEPARSER_H
#define CONFIG_PROPERTYTREEPARSER_H

#include "propertynode.h"
#include "simpletokenizer.h"
#include <stdexcept>

enum PropertyTreeTokens {
	PropertyTokenBlockStart = TokenUser,
	PropertyTokenBlockEnd,
	PropertyTokenEOL,
};

class BadBlock : public std::runtime_error {
public:
	BadBlock(const std::string & message) : std::runtime_error(message) {}
};

class PropertyTreeParser {
public:
	PropertyNode parse(const std::string & input);

private:
	void parseBlock(PropertyNode & node);
	void addLine(const std::vector<std::string> & line, PropertyNode & node) const;
	
	std::vector<Token> tokens;
	size_t currentToken, braceCount;
};

#endif // !CONFIG_PROPERTYTREEPARSER_H
