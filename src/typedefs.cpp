#include "typedefs.h"

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

namespace hivemind
{
	date_t to_date(std::string str)
	{
		const static boost::regex int_regex("([0-9]{4})-([0-9]{2})-([0-9]{2})");

		boost::smatch match;
		if(boost::regex_match(str, match, int_regex))
			return date_t(
				boost::lexical_cast<uint32_t>(match[1]),
				boost::lexical_cast<uint32_t>(match[2]),
				boost::lexical_cast<uint32_t>(match[3])
			);

		throw std::logic_error("Can not parse date-string");
	}
}
