
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>

#include <string>
#include <vector>

namespace tinyxml2
{
	xmlpp::Element* Node_getFirstChildElement(xmlpp::Node*node);
	xmlpp::Element* Node_getLastChildElement(xmlpp::Node*node);
	
	xmlpp::Element* Node_getElementById(xmlpp::Node*node, const std::string&id);
	std::vector<xmlpp::Element*> Node_getElementsByClassName(xmlpp::Node*node, const std::string&className);
	std::vector<xmlpp::Element*> Node_getElementsByClassNames(xmlpp::Node*node, const std::vector<std::string>&classNames);
}
