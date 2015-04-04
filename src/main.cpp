#include <iostream>
#include "Config.hpp"

int main(int argc, char** args){
	Config cfg;

	// load cfg file
	try{
		cfg.read("test.cfg");
	}
	catch(std::invalid_argument& e){
		std::cerr<<"Error reading config file!"<<std::endl;
		std::cerr<<e.what()<<std::endl;
	}

	// print out saved variables
	std::cout<<cfg["parent"]["child"]["var"].getString()<<std::endl;
	std::cout<<cfg["parent"]["child"]["nr"].getInt()<<std::endl;
	std::cout<<cfg["parent"]["child"]["floating"].getFloat()<<std::endl;

	// creates a new element if not existing yet
	std::cout<<cfg["not"]["existing"]["foo"].getName()<<std::endl;

	// create some top level elements
	cfg["test"] = "Hi";
	cfg["test2"] = 123;
	cfg["test3"] = 1.23;

	// print out a name of an element
	std::cout<<cfg["test"].getName()<<std::endl;

	// print out stored variables
	std::cout<<cfg["test"].getString()<<std::endl;
	std::cout<<cfg["test2"].getInt()<<std::endl;
	std::cout<<cfg["test3"].getFloat()<<std::endl;

	// creates an array
	cfg["my_array"].set("val0", 0);
	cfg["my_array"].set("val1", 1);
	cfg["my_array"].set("val2", 5);

	std::cout<<cfg["my_array"].getString(0)<<std::endl;
	std::cout<<cfg["my_array"].getString(1)<<std::endl;
	std::cout<<cfg["my_array"].getString(5)<<std::endl;

	// removes an element
	std::cout<<cfg.exists("not")<<std::endl;

	cfg.remove("not");

	std::cout<<cfg.exists("not")<<std::endl;

	return 0;
}
