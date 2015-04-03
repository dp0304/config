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

				Element(const std::string& name);

				void set(const std::string& value, const std::size_t i = 0);
				void set(const int value, const std::size_t i = 0);
				void set(const float value, const std::size_t i = 0);

				std::string getString(const std::size_t i = 0);
				int getInt(const std::size_t i = 0);
				float getFloat(const std::size_t i = 0);
		};

	private:
		static const std::set<char> delimeter;

		std::string input;
		std::size_t index;
		std::vector<std::string> tokens;
		bool isstring, mask;

		bool testString(const char c);

		void preprocess(std::ifstream& file);
		void tokenize();
		void parse();

	public:
		bool read(const std::string& path);
};
