#include "csv_parser.h"

#include <stdexcept>
#include <boost/tokenizer.hpp>

namespace Hivemind {

	csv_parser::csv_parser(std::istream& s)
	: m_s(s)
	{
	}

	bool csv_parser::read_line(std::vector<std::string>& result)
	{
		const static boost::char_separator<char> sep(",", "", boost::keep_empty_tokens);

		result.clear();

		std::string buffer;

		if(!std::getline(m_s, buffer))
			return false;

		boost::tokenizer<boost::char_separator<char>> tok(buffer, sep);
		for(const std::string v : tok)
			result.emplace_back(v);

		return true;
	}

}
