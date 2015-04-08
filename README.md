# config

Simple C++ config reader and writer.

# Format

```
# single line comment

string = "this is a string";
number = 123;
floating_number = 1.23;

# , without a following value will create an empty string in that place
# 3 for example has the index 4
array = [1, 2, , , 3, "4", 5.5] # the ; is optional after brackets

object = {
	name = "value within object";
	nr = 1;
	
	nested = {
		name = "nested object";
	}
	
	# careful, you cannot store values directly in objects
}
```

Variable, array and object names can be anything, except of double quotes ("). Spaces outside of strings will be skipped, so a name like "my var" will be parsed to "myvar".
For strings you can also use single quotes. Currently there is no support for arrays of objects or multi dimensional arrays.

# Usage

```C++
#include "Config.hpp"

void main(){
	Config cfg;
	
	try{
		// read cfg file, if possible
		if(!cfg.read("in.cfg")){
			std::cerr<<"Could not open config file!"<<std::endl;
			return 1;
		}
	}
	catch(std::invalid_argument& e){
		// this error can occur if the cfg file cannot be completly parsed
		// the parsed content until failure will be kept
		std::cerr<<"Error reading config file!"<<std::endl;
		std::cerr<<e.what()<<std::endl;
	}
	
	// read a value (this can cause an invalid_argument exception if cast is not possible
	// if a variable does not exists, it will be created
	std::string string = cfg["string"].getString(); // will always work
	int number = cfg["number"].getInt();
	float floating_number = cfg["floating_number"].getFloat();
	
	// read from object
	int arrayIndex1 = cfg["array"].getInt(1);
	std::string arrayIndex5 = cfg["array"].getString(5);
	
	// read from object
	int nr = cfg["object"]["nr"];
	
	// write variable
	cfg["new_string"] = string;
	
	// create array
	cfg["new_array"].set("value0", 0);
	cfg["new_array"].set("value1", 1);
	cfg["new_array"].set("value2", 5); // spaces will be inserted
	
	// create object
	cfg["new_object"]["foo"] = "bar";
	
	// write cfg file (wont keep comments)
	if(!cfg.write("out.cfg")){
		std:.cerr<<"Could not write config file!"<<std::endl;
	}
	
	// clear content
	cfg.clear();
}
```

The read() method can throw an std::invalid_argument exception, which signals that the config file read cannot be parsed completly. Everything until the error will be kept and what() returns in which line and at which character the error occured.
If you store data in an variable that also contain child variables, it will be an objects. Which means all data in the parent object won't be stored on save.

# Features that will be added in the future

* keep comments on save
* ~~better parser error output~~
* nested arrays
* array of objects

# Build

Simply compile with C++11 compiler. No dependencies required.

# License

MIT
