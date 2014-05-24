#pragma once

#include <istream>
#include <string>
#include <vector>

namespace hivemind {

	class csv_parser
	{
		std::istream& m_s;

		csv_parser(const csv_parser&) = delete;
		const csv_parser operator=(const csv_parser&) = delete;

	public:
		csv_parser(std::istream& s);
		bool read_line(std::vector<std::string>& result);
	};

}
