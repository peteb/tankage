/*
 *  propertytreeprinter.h
 *  config
 *
 *  Created by Peter Backman on 2/3/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef CONFIG_PROPERTYTREEPRINTER_H
#define CONFIG_PROPERTYTREEPRINTER_H

class PropertyNode;

class PropertyTreePrinter {
public:
	PropertyTreePrinter(std::ostream & output);

	void print(const PropertyNode & tree, bool printRoot = false) const;
	
private:
	void printStep(const PropertyNode & node, int level) const;
	std::string formatString(const std::string & text) const;
	std::string getLevelIndent(int level) const;
	
	bool bracketOwnLine;
	std::string indentLevel;
	
	std::ostream & output;	
};

#endif // !CONFIG_PROPERTYTREEPRINTER_H
