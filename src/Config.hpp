#pragma once

#include <iostream> // FIXME: delete
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <stdexcept>

class Config{
	public:
		class Element{
			public:
				std::string name;
				std::vector<std::string> values;
				std::vector<Element> children;

				Element(const std::string& name);
		};

	private:
		static const std::set<char> delimeter;

		// preprocessor, tokenizer and parser
		std::string input;
		std::size_t index;
		std::vector<std::string> tokens;
		bool isstring, mask;

		bool testString(const char c);

		bool accept(const std::string& token);
		void expect(const std::string& token);
		void next();
		const std::string& get();

		void parseVar(std::vector<Element>& parent);
		void parseArray(Element& e);
		void parseObject(Element& e);

		void preprocess(std::ifstream& file);
		void tokenize();
		void parse();

		// top level elements
		std::vector<Element> elements;

	public:
		bool read(const std::string& path);

		void test();
};
