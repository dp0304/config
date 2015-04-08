#include "Config.hpp"

const unsigned int Config::TABS = 4;
const std::set<char> Config::delimeter = {'=', ';', '[', ']', '{', '}', ','};

// Element

Config::Element::Element(const std::string& name){
	this->name = name;
}

Config::Element::Element(){

}

void Config::Element::set(const std::string& value, const std::size_t i){
	if(i >= values.size()){
		values.resize(i+1);
	}

	values[i] = value;
}

void Config::Element::set(const int value, const std::size_t i){
	set(std::to_string(value), i);
}

void Config::Element::set(const float value, const std::size_t i){
	set(std::to_string(value), i);
}

void Config::Element::operator=(const std::string& value){
	set(value);
}

void Config::Element::operator=(const int value){
	set(value);
}

void Config::Element::operator=(const float value){
	set(value);
}

void Config::Element::operator=(const double value){
	set(float(value));
}

const std::string& Config::Element::getName(){
	return name;
}

const std::string& Config::Element::getString(const std::size_t i){
	if(i >= values.size()){
		values.resize(i+1);
		values[i] = "";
	}

	return values[i];
}

const int Config::Element::getInt(const std::size_t i){
	return std::stoi(getString(i));
}

const float Config::Element::getFloat(const std::size_t i){
	return std::stof(getString(i));
}

Config::Element& Config::Element::operator[](const std::string& name){
	for(auto i = children.begin(); i != children.end(); i++){
		if((*i).name == name){
			return *i;
		}
	}

	children.push_back(Element(name));

	return children.back();
}

bool Config::Element::exists(const std::string& name){
	for(auto e : children){
		if(e.name == name){
			return true;
		}
	}

	return false;
}

void Config::Element::remove(const std::string& name){
	for(auto i = children.begin(); i != children.end(); i++){
		if((*i).name == name){
			children.erase(i);
			return;
		}
	}
}

void Config::Element::clear(){
	children.clear();
}

const std::vector<Config::Element>& Config::Element::getAll(){
	return children;
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

	mask = false; // reset mask

	return false;
}

bool Config::accept(const std::string& token){
	return token == get();
}

void Config::expect(const std::string& token){
	if(token != get()){
		throw std::invalid_argument("Unexpected token, was \""+get()+"\" expected \""+token+"\" in line "+std::to_string(tokens[index].line)+" at character "+std::to_string(tokens[index].character)+".");
	}

	next();
}

void Config::next(){
	if(index < tokens.size()-1){
		index++;
	}
}

const std::string& Config::get(){
	return tokens[index].token;
}

void Config::parseVar(std::vector<Element>& parent){
	Element e(get());
	next();
	expect("=");

	if(accept("[")){
		parseArray(e);
	}
	else if(accept("{")){
		parseObject(e);
	}
	else{
		e.values.push_back(get());
		next();
		expect(";");
	}

	parent.push_back(e);
}

void Config::parseArray(Element& e){
	expect("[");

	while(!accept("]")){
		e.values.push_back(get());
		next();

		if(accept(",")){
			next();
		}

		while(accept(",")){
			e.values.push_back("");
			next();
		}
	}

	expect("]");

	if(accept(";")){ // optional
		next();
	}
}

void Config::parseObject(Element& e){
	expect("{");

	while(!accept("}")){
		parseVar(e.children);
	}

	expect("}");

	if(accept(";")){ // optional
		next();
	}
}

void Config::preprocess(std::ifstream& file){
	input = "";

	std::string line;
	isstring = false;
	mask = false;

	while(std::getline(file, line)){
		for(auto c : line){
			if(!isstring && (c == ' ' || c == '\t')){ // skip white space
				continue;
			}

			if(!isstring && c == '#'){ // skip comments
				break;
			}

			testString(c);

			input += c;
		}

		if(isstring){
			input += '\n';
		}
	}
}

void Config::tokenize(){
	tokens.clear();

	auto end = delimeter.end();
	std::string token, deli;
	isstring = false;
	mask = false;
	std::size_t line = 1;
	std::size_t character = 0;

	for(auto c : input){
		if(testString(c)){
			continue;
		}

		// count lines and character but skip new line
		if(c == '\n'){
			line++;
			character = 0;

			continue;
		}

		if(!isstring && delimeter.find(c) != end){
			// cut " if string
			if(token.length() && token.at(0) == '"' && token.at(token.length()-1) == '"'){
				token = token.substr(1, token.length()-2);
			}

			// store token
			if(!token.empty()){
				tokens.push_back(Token(token, line, character));
				token = "";
			}

			// store delimeter
			deli = c;
			tokens.push_back(Token(deli, line, character));

			continue;
		}

		token += c;
		character = 0;
	}
}

void Config::parse(){
	index = 0;
	root.clear();

	while(index != tokens.size()-1){
		parseVar(root.children);
	}
}

bool Config::isNumeric(const std::string& value){
	for(auto c : value){
		if(!std::isdigit(c, std::locale()) && c != '.' && c != '-'){
			return false;
		}
	}

	return true;
}

void Config::writeElement(std::ofstream& file, const Element& element, const unsigned int tabs){
	if(!element.values.size() && !element.children.size()){
		return;
	}

	for(unsigned int i = 0; i < tabs; i++){
		file<<" ";
	}

	bool isnumeric = false;

	file<<element.name<<" = ";

	if(element.children.size()){
		file<<"{\n";

		for(auto e : element.children){
			writeElement(file, e, tabs+TABS);
		}

		for(unsigned int i = 0; i < tabs; i++){
			file<<" ";
		}

		file<<"}";
	}
	else if(element.values.size() == 1){
		if(isNumeric(element.values[0])){
			file<<element.values[0]<<";";
		}
		else{
			file<<"\""<<element.values[0]<<"\";";
		}
	}
	else{
		file<<"[";

		for(std::size_t i = 0; i < element.values.size(); i++){
			isnumeric = isNumeric(element.values[i]);

			if(!isnumeric){
				file<<"\"";
			}

			file<<element.values[i];

			if(!isnumeric){
				file<<"\"";
			}

			if(i != element.values.size()-1){
				file<<", ";
			}
		}

		file<<"]";
	}

	file<<"\n";
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

bool Config::write(const std::string& path){
	std::ofstream file(path);

	if(!file.good()){
		file.close();
		return false;
	}

	for(auto e : root.children){
		writeElement(file, e);
	}

	file.close();

	return true;
}

Config::Element& Config::operator[](const std::string& name){
	return root[name];
}

bool Config::exists(const std::string& name){
	return root.exists(name);
}

void Config::remove(const std::string& name){
	root.remove(name);
}

void Config::clear(){
	root.clear();
}

const std::vector<Config::Element>& Config::getAll(){
	return root.children;
}
