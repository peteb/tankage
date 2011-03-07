/*
 *  propertytreeprinter.cpp
 *  config
 *
 *  Created by Peter Backman on 2/3/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include <ostream>
#include "propertytreeprinter.h"
#include "propertynode.h"

PropertyTreePrinter::PropertyTreePrinter(std::ostream & output) 
	: bracketOwnLine(false)
	, indentLevel("\t")
    , output(output)
{
}

void PropertyTreePrinter::print(const PropertyNode & tree, bool printRoot) const {
	if (printRoot) {
		printStep(tree, 0);
	}
	else {
		for (int i = 0; i < tree.getNumNodes(); ++i)
			printStep(tree.getNode(i), 0);
	}
}

void PropertyTreePrinter::printStep(const PropertyNode & node, int level) const {
	std::string outerIndent = getLevelIndent(level);
	std::string innerIndent = getLevelIndent(level + 1);
	
	output << outerIndent << formatString(node.getName()) << " ";
	
	for (int i = 0; i < node.getNumAttributes(); ++i)
		output << formatString(node.getAttribute(i)) << " ";
	
	if (bracketOwnLine)
		output << "\n" << outerIndent << "{\n";
	else
		output << "{\n";
	
	for (int i = 0; i < node.getNumProperties(); ++i) {
		Property property = node.getProperty(i);

		output << innerIndent << formatString(property.first) << " " << formatString(property.second) << "\n";
	}
		
	if (node.getNumProperties() > 0 && node.getNumNodes() > 0)
		output << "\n";
	
	for (int i = 0; i < node.getNumNodes(); ++i) {
		printStep(node.getNode(i), level + 1);
		
		if (i < node.getNumNodes() - 1)
			output << "\n";
	}
	
	output << outerIndent << "}\n";
}

std::string PropertyTreePrinter::formatString(const std::string & text) const {
	// use quotation marks around string if there's a whitespace in there
	std::string ret;
	
	if (text.find(" ") != std::string::npos || text.find("\t") != std::string::npos)
		ret = "\"" + text + "\"";
	else
		ret = text;
	
	return ret;
}

std::string PropertyTreePrinter::getLevelIndent(int level) const {
	std::string ret;
	for (int i = 0; i < level; ++i)
		ret += indentLevel;
	
	return ret;
}
