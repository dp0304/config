#include "Config.hpp"

const std::set<char> Config::delimeter = {'=', ';', '[', ']', '{', '}', ','};

// Element

Config::Element::Element(const std::string& name){
	this->name = name;
}

void Config::Element::set(const std::string& value, const std::size_t i){
	if(i > values.size()){
		values.resize(i);
	}

	values[i] = value;
}

void Config::Element::set(const int value, const std::size_t i){
	set(std::to_string(value), i);
}

void Config::Element::set(const float value, const std::size_t i){
	set(std::to_string(value), i);
}

std::string Config::Element::getString(const std::size_t i){
	if(values.size() && i < values.size()){
		return values[i];
	}

	throw std::out_of_range("index out of range");
}

int Config::Element::getInt(const std::size_t i){
	return std::stoi(getString(i));
}

float Config::Element::getFloat(const std::size_t i){
	return std::stof(getString(i));
}

// Config

bool Config::testString(const char c){
	if(isstring && c == '\\'){ // allow mask in strings
		mask = !mask;

		if(mask){
			return true;
		}
	}

	if(!mask && (c == '"' || c == '\'')){ // strings " or '
		isstring = !isstring;
	}

	return false;
}

void Config::preprocess(std::ifstream& file){
	input = "";

	std::string line;
	bool first = true;
	isstring = false;
	mask = false;

	while(std::getline(file, line)){
		for(auto c : line){
			if(!isstring && (c == ' ' || c == '\t')){ // skip white space
				continue;
			}

			if(first && c == '#'){ // skip comments
				break;
			}

			if(testString(c)){
				continue;
			}

			input += c;
			mask = false;
			first = false;
		}

		if(isstring){
			input += '\n';
		}

		first = true;
	}
}

void Config::tokenize(){
	tokens.clear();

	auto end = delimeter.end();
	std::string token, deli;
	isstring = false;
	mask = false;

	for(auto c : input){
		if(testString(c)){
			continue;
		}

		if(!isstring && delimeter.find(c) != end){
			// cut " if string
			if(token.length() && token.at(0) == '"' && token.at(token.length()-1) == '"'){
				token = token.substr(1, token.length()-2);
			}

			// store token
			if(!token.empty()){
				tokens.push_back(token);
				token = "";
			}

			// store delimeter
			deli = c;
			tokens.push_back(deli);

			continue;
		}

		token += c;
		mask = false;
	}

	// FIXME: remove test
	for(auto t : tokens){
		std::cout<<t<<std::endl;
	}
}

void Config::parse(){
	index = 0;

	// ...
}

bool Config::read(const std::string& path){
	std::ifstream file(path);

	if(!file.good()){
		file.close();
		return false;
	}

	preprocess(file);
	file.close();
	tokenize();
	parse();

	return true;
}
