#include <iostream>
#include <ezconfig/Config.hpp>

int main(int argc, char** args){
	Config cfg;

	// load cfg file
	try{
		if(!cfg.read("../../in.cfg")){//set path for cfg
			std::cerr<<"Could not open config file!"<<std::endl;
			return 1;
		}
	}
	catch(std::invalid_argument& e){
		std::cerr<<"Error reading config file! Program will FAIL. "
             "Examine if your CFG file is sane."<<std::endl;
		std::cerr<<e.what()<<std::endl;
	}

	// print out saved variables
	std::cout<<cfg["parent"]["child"]["string"].getString()<<std::endl;
	std::cout<<cfg["parent"]["child"]["integer"].getInt()<<std::endl;
	std::cout<<cfg["parent"]["child"]["floating"].getFloat()<<std::endl;
	std::cout<<cfg["str"].getString()<<std::endl;
	std::cout<<cfg["single_quote"].getString()<<std::endl;

    // accessing non-existing target
    std::cout<<"(getting blahblah_nonexisting) : [";
    std::cout<<cfg["blahblah_nonexisting"].getString()<<"]"<<std::endl;

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

	// write config
	cfg.clear();

	cfg["a"] = 123;
	cfg["b"] = 1.23;
	cfg["b"] = -1.23;
	cfg["arr"].set("a", 0);
	cfg["arr"].set("b", 1);
	cfg["arr"].set("c", 4);
	cfg["obj"]["var"] = "val";
	cfg["obj"]["foo"]["bar"] = "val";

	if(!cfg.write("out.cfg")){
		std::cerr<<"Could not write config file!"<<std::endl;
		return 2;
	}

	return 0;
}
