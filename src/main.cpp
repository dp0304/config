#include <iostream>
#include "Config.hpp"

int main(int argc, char** args){
	Config cfg;
	cfg.read("test.cfg");
	cfg.test();

	return 0;
}
