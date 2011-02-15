/*
 *  simpletokenizer.h
 *  config
 *
 *  Created by Peter Backman on 2/9/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef CONFIG_SIMPLETOKENIZER_H
#define CONFIG_SIMPLETOKENIZER_H

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

typedef std::pair<int, std::string> Token;

enum BuiltinToken {
	TokenVerbatim = 1,
	TokenString,
	TokenNumeric,
	TokenUser = 20,
};

class SimpleTokenizer {
public:	
	void tokenize(const std::string & input, std::vector<Token> & output);
	void addToken(const std::string & ident, int value);
	void addVerbatim(const std::pair<std::string, std::string> & verbatim);
	void addComment(const std::pair<std::string, std::string> & comment);
	void addIgnore(const std::string & ignore);
	
private:
	typedef std::pair<std::pair<int, int>, int> VerbatimLength;
	
	int getIgnoreLength() const;
	VerbatimLength getVerbatimLength() const;
	Token getCustomToken() const;
	int getTokenType(const std::string & value) const;
	void addLastToken(std::vector<Token> & output) const;
	
	std::vector<std::string> ignores;
	std::vector<std::pair<std::string, std::string> > verbatims, comments;
	std::vector<Token > customTokens;

	const char * currentChar, * lastToken;
};

#endif // !CONFIG_SIMPLETOKENIZER_H
