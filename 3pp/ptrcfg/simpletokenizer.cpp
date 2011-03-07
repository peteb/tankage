/*
 *  simpletokenizer.cpp
 *  config
 *
 *  Created by Peter Backman on 2/9/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "simpletokenizer.h"

#include <sstream>
#include <cstring>

void SimpleTokenizer::tokenize(const std::string & input, std::vector<Token> & output) {
	currentChar = input.c_str();
	lastToken = currentChar;
	
	while (*currentChar != '\0') {
		int ignoreLength = getIgnoreLength();
		VerbatimLength verbatimLength = getVerbatimLength();
		Token customToken = getCustomToken();

		if (ignoreLength > 0) {		// starts ignore data, jumps to end
			addLastToken(output);

			currentChar += ignoreLength;
			lastToken = currentChar;
		}
		else if (verbatimLength.second > 0) {	// starts verbatim data, jumps to end, adds token
			addLastToken(output);
			
			output.push_back(Token(TokenVerbatim, std::string(currentChar + verbatimLength.first.first, verbatimLength.second)));
			
			currentChar += verbatimLength.first.first + verbatimLength.second + verbatimLength.first.second;
			lastToken = currentChar;
		}
		else if (customToken.first != -1) {	// starts custom token, jumps to end, adds token
			addLastToken(output);
			
			output.push_back(customToken);
			currentChar += customToken.second.size();
			lastToken = currentChar;
		}
		else {	// inside custom data
			++currentChar;
		}
	}
	
	
	addLastToken(output);
}

int SimpleTokenizer::getIgnoreLength() const {
	// checks for ignores and comments
	for (size_t i = 0; i < ignores.size(); ++i) {
		if (strncmp(currentChar, ignores[i].c_str(), ignores[i].size()) == 0)
			return ignores[i].size();
	}

	for (size_t i = 0; i < comments.size(); ++i) {
		if (strncmp(currentChar, comments[i].first.c_str(), comments[i].first.size()) == 0) {
			return strstr(currentChar, comments[i].second.c_str()) - currentChar + comments[i].first.size();
		}
	}
	
	return 0;
}

SimpleTokenizer::VerbatimLength SimpleTokenizer::getVerbatimLength() const {
	for (size_t i = 0; i < verbatims.size(); ++i) {
		if (strncmp(currentChar, verbatims[i].first.c_str(), verbatims[i].first.size()) == 0) {
			std::pair<int, int> endcaps(verbatims[i].first.size(), verbatims[i].second.size());
			int length = strstr(currentChar + verbatims[i].first.size(), verbatims[i].second.c_str()) - currentChar - verbatims[i].first.size();
			
			return VerbatimLength(endcaps, length);
		}
	}
	
	return std::make_pair(std::make_pair(0, 0), 0);
}

Token SimpleTokenizer::getCustomToken() const {
	for (size_t i = 0; i < customTokens.size(); ++i) {
		if (strncmp(currentChar, customTokens[i].second.c_str(), customTokens[i].second.size()) == 0) {
			return customTokens[i];
		}
	}
	
	return Token(-1, "");
}

int SimpleTokenizer::getTokenType(const std::string & value) const {
	std::stringstream ss;
	ss << value;
	float hey = 0.0f;
	if (ss >> hey)
		return TokenNumeric;
	
	return TokenString;
}

void SimpleTokenizer::addLastToken(std::vector<Token> & output) const {
	if (currentChar > lastToken) {
		std::string tokenValue(lastToken, currentChar - lastToken);
		
		if (!tokenValue.empty())
			output.push_back(Token(getTokenType(tokenValue), tokenValue));
	}
}

void SimpleTokenizer::addToken(const std::string & ident, int value) {
	customTokens.push_back(Token(value, ident));
}

void SimpleTokenizer::addVerbatim(const std::pair<std::string, std::string> & verbatim) {
	verbatims.push_back(verbatim);
}

void SimpleTokenizer::addComment(const std::pair<std::string, std::string> & comment) {
	comments.push_back(comment);
}

void SimpleTokenizer::addIgnore(const std::string & ignore) {
	ignores.push_back(ignore);
}
