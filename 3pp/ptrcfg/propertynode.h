/*
 *  propertynode.h
 *  config
 *
 *  Created by Peter Backman on 2/3/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef CONFIG_PROPERTYNODE_H
#define CONFIG_PROPERTYNODE_H

#include <string>
#include <vector>
#include <map>

typedef std::pair<std::string, std::string> Property;

class PropertyNode {
public:
	PropertyNode(const std::string & name);
	
	void addAttribute(const std::string & attribute);
	void addProperty(const Property & property);
	void addNode(const PropertyNode & node);
	
	int getNumProperties() const;
	int getNumAttributes() const;
	int getNumNodes() const;
	
	std::string getAttribute(int id) const;
	std::string getProperty(const std::string & name, const std::string & defaultValue) const;
	std::string getProperty(const std::string & name) const;
	Property getProperty(int id) const;

	std::string getName() const;
	
	const PropertyNode & getNode(int id) const;
	const PropertyNode & getNode(const std::string & name) const;
	PropertyNode & getNode(const std::string & name);
	
	static PropertyNode MergeTrees(const PropertyNode & base, const PropertyNode & sub);
	
private:
	typedef std::map<std::string, std::string> PropertyMap;
	
	PropertyMap properties;
	std::vector<PropertyNode> children;
	std::vector<std::string> attributes;
	std::string name;
};

#endif // !CONFIG_PROPERTYNODE_H
