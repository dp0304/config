#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <stdexcept>
#include <locale>

class Config{
	public:
		class Element{
			friend Config;

			private:
				std::string name;
				std::vector<std::string> values;
				std::vector<Element> children;

			public:
				Element(const std::string& name);
				Element();

				void set(const std::string& value, const std::size_t i = 0);
				void set(const int value, const std::size_t i = 0);
				void set(const float value, const std::size_t i = 0);
				void operator=(const std::string& value);
				void operator=(const int value);
				void operator=(const float value);
				void operator=(const double value);

				const std::string& getName();
				const std::string& getString(const std::size_t i = 0);
				const int getInt(const std::size_t i = 0);
				const float getFloat(const std::size_t i = 0);

				Element& operator[](const std::string& name);
				bool exists(const std::string& name);
				void remove(const std::string& name);
				void clear();

				const std::vector<Element>& getAll();
		};

	private:
		struct Token{
			std::string token;
			std::size_t line, character;

			Token(const std::string& token, const std::size_t line, const std::size_t character):
				token(token),
				line(line),
				character(character){

			}
		};

		static const unsigned int TABS;
		static const std::set<char> delimeter;

		// preprocessor, tokenizer and parser
		std::string input;
		std::size_t index;
		std::vector<Token> tokens;
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

		// output
		bool isNumeric(const std::string& value);
		void writeElement(std::ofstream& file, const Element& element, const unsigned int tabs = 0);

		// top level element
		Element root;

	public:
		bool read(const std::string& path);
		bool write(const std::string& path);

		Element& operator[](const std::string& name);
		bool exists(const std::string& name);
		void remove(const std::string& name);
		void clear();

		const std::vector<Element>& getAll();
};
