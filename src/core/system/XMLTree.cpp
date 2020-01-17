#include "XMLTree.h"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


namespace sibr {
	XMLTree::XMLTree(const std::string &  path)
	{
		std::cout << "Parsing xml file < " << path << " > : ";
		std::ifstream file(path.c_str());
		if (file) {
			std::stringstream buffer;
			buffer << file.rdbuf();
			file.close();
			xmlString = std::move(std::string(buffer.str()));
			this->parse<0>(&xmlString[0]);
			std::cout << "success " << std::endl;
		}
		else {
			std::cout << "error, cant open file " << std::endl;
		}
	}


	XMLTree::~XMLTree(void)
	{
	}


	bool XMLTree::save(const std::string & filePath) const {
		std::ofstream file(filePath);
		if(!file.is_open()) {
			SIBR_WRG << "Unable to save XML to path \"" << filePath << "\"." << std::endl;
			return false;
		}

		file << *this;
		file.close();
		return true;
	}

}
