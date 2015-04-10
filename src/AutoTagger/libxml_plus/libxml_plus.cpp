
#include "libxml_plus.h"

namespace tinyxml2
{
	std::vector<std::string> string_split(const std::string&str, char delim)
	{
		std::vector<std::string> elements;
		size_t str_length = str.length();
		size_t next_element_start = 0;
		for(size_t i=0; i<str_length; i++)
		{
			char c = str.at(i);
			if(c == delim)
			{
				if(i!=next_element_start)
				{
					elements.push_back(str.substr(next_element_start, i));
				}
				next_element_start = i+1;
			}
		}
		if(next_element_start != str_length)
		{
			elements.push_back(str.substr(next_element_start, str_length));
		}
		return elements;
	}
	
	template<typename T>
	bool vector_containsAll(const std::vector<T>&vect, const std::vector<T>&cmp)
	{
		size_t cmp_size = cmp.size();
		size_t vect_size = vect.size();
		for(size_t i=0; i<cmp_size; i++)
		{
			bool doesHave = false;
			for(size_t j=0; j<vect_size; j++)
			{
				if(cmp[i] == vect[j])
				{
					doesHave = true;
					j = vect_size;
				}
			}
			if(!doesHave)
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>
	void vector_push_back(std::vector<T>&vect, const std::vector<T>&adding)
	{
		size_t adding_size = adding.size();
		size_t startIndex = vect.size();
		size_t total_size = startIndex+adding_size;
		vect.resize(total_size);
		size_t counter = 0;
		for(size_t i=startIndex; i<total_size; i++)
		{
			vect[i] = adding[counter];
			counter++;
		}
	}
	
	xmlpp::Element* Node_getFirstChildElement(xmlpp::Node*node)
	{
		xmlpp::Node::NodeList children(node->get_children());
		size_t children_size = children.size();
		for(size_t i=0; i<children_size; i++)
		{
			xmlpp::Node* node = *std::next(children.begin(), i);
			if(node->cobj()->type == XML_ELEMENT_NODE)
			{
				return static_cast<xmlpp::Element*>(node);
			}
		}
		return nullptr;
	}
	
	xmlpp::Element* Node_getLastChildElement(xmlpp::Node*node)
	{
		xmlpp::Node::NodeList children(node->get_children());
		size_t children_size = children.size();
		for(size_t i=(children_size-1); i!=SIZE_MAX; i--)
		{
			xmlpp::Node* node = *std::next(children.begin(), i);
			if(node->cobj()->type == XML_ELEMENT_NODE)
			{
				return static_cast<xmlpp::Element*>(node);
			}
		}
		return nullptr;
	}

	xmlpp::Element* Node_getElementById(xmlpp::Node*node, const std::string&id)
	{
		std::string id_attr = "id";
		xmlpp::Node::NodeList children(node->get_children());
		size_t children_size = children.size();
		for(size_t i=0; i<children_size; i++)
		{
			xmlpp::Node* current_node = *std::next(children.begin(), i);
			if(node->cobj()->type == XML_ELEMENT_NODE)
			{
				xmlpp::Element*current_element = static_cast<xmlpp::Element*>(node);
				xmlpp::Attribute* attribute = current_element->get_attribute(id_attr);
				if(attribute!=nullptr && attribute->get_value()==id)
				{
					return current_element;
				}
				else
				{
					xmlpp::Element*element = Node_getElementById(current_node, id);
					if(element != nullptr)
					{
						return element;
					}
				}
			}
		}
		return nullptr;
	}
	
	std::vector<xmlpp::Element*> Node_getElementsByClassName(xmlpp::Node*node, const std::string&className)
	{
		std::vector<std::string> classNames(string_split(className, ' '));
		return Node_getElementsByClassNames(node, classNames);
	}
	
	std::vector<xmlpp::Element*> Node_getElementsByClassNames(xmlpp::Node*node, const std::vector<std::string>&classNames)
	{
		std::vector<xmlpp::Element*> elements;
		std::string class_attr = "class";
		
		xmlpp::Node::NodeList children(node->get_children());
		size_t children_size = children.size();
		for(size_t i=0; i<children_size; i++)
		{
			xmlpp::Node* current_node = *std::next(children.begin(), i);
			if(node->cobj()->type == XML_ELEMENT_NODE)
			{
				xmlpp::Element*current_element = static_cast<xmlpp::Element*>(node);
				xmlpp::Attribute* attribute = current_element->get_attribute(class_attr);
				if(attribute!=nullptr && vector_containsAll(string_split(attribute->get_value(),' '), classNames))
				{
					elements.push_back(current_element);
				}
				else
				{
					vector_push_back(elements, Node_getElementsByClassNames(current_node, classNames));
				}
			}
		}
		return elements;
	}
}
