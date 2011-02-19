/*
 *  propertytreeparser.cpp
 *  config
 *
 *  Created by Peter Backman on 2/10/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "propertytreeparser.h"
#include <sstream>

PropertyNode PropertyTreeParser::parse(const std::string & input) {
	tokens.clear();
	tokens.reserve(input.size() / 10);
	
	SimpleTokenizer tokenizer;
	
	tokenizer.addToken("{", PropertyTokenBlockStart);
	tokenizer.addToken("}", PropertyTokenBlockEnd);
	tokenizer.addToken("\n", PropertyTokenEOL);
	
	tokenizer.addVerbatim(std::make_pair("\"", "\""));
	tokenizer.addComment(std::make_pair("//", "\n"));
	tokenizer.addComment(std::make_pair("/*", "*/"));
	
	tokenizer.addIgnore(" ");
	tokenizer.addIgnore("\t");
	tokenizer.addIgnore("\r");
	
	tokenizer.tokenize(input, tokens);
	
	
	
	PropertyNode root("root");
	
	braceCount = currentToken = 0;
	parseBlock(root);
		
	return root;
}


void PropertyTreeParser::parseBlock(PropertyNode & node) {
	std::vector<std::string> attributes;
	
	while (currentToken < tokens.size()) {
		const Token & token = tokens[currentToken];
		
		if (token.first == PropertyTokenBlockStart) {
			++braceCount;
			
			try {
				PropertyNode newNode(attributes.at(0));
			
				for (size_t i = 1; i < attributes.size(); ++i)
					newNode.addAttribute(attributes[i]);
			
				attributes.clear();
			
				++currentToken;
				parseBlock(newNode);
				node.addNode(newNode);
			}
			catch (const std::out_of_range & e) {
				throw BadBlock("missing block name");
			}
		}
		else if (token.first == PropertyTokenBlockEnd) {
			if (--braceCount < 0)
				throw BadBlock("closing block with none opened");
			
			if (!attributes.empty()) {
				addLine(attributes, node);
				attributes.clear();
			}
			
			break;
		}
		else if (token.first == PropertyTokenEOL) {
			if (!attributes.empty()) {
				addLine(attributes, node);
				attributes.clear();
			}
		}
		else {
			attributes.push_back(token.second);
		}
		
		++currentToken;
	}
}

void PropertyTreeParser::addLine(const std::vector<std::string> & line, PropertyNode & node) const {
	if (line.size() > 1) {
		node.addProperty(Property(line[0], line[1]));
	}
	else if (line.size() == 1) {	// key will be assigned an id if there's none
		std::stringstream ss;
		ss << node.getNumProperties();
		node.addProperty(Property(ss.str(), line[0]));
	}
}
