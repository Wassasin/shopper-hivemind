#pragma once

#include <istream>
#include <string>
#include <vector>

namespace Hivemind {

	class CsvParser
	{
		std::istream& m_s;

		CsvParser(const CsvParser&) = delete;
		const CsvParser operator=(const CsvParser&) = delete;

	public:
		CsvParser(std::istream& s);
		bool readLine(std::vector<std::string>& result);
	};

}
