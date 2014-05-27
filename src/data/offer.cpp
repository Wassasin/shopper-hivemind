#include "offer.h"

#include <boost/lexical_cast.hpp>

namespace Hivemind
{
	Offer Offer::from_line(const std::vector<std::string>& line)
	{
		if(line.size() != 6)
			throw std::runtime_error("Offer csv is not valid");

		Offer result;
		result.id = boost::lexical_cast<Id>(line[0]);
		result.category = boost::lexical_cast<Id>(line[1]);
		result.quantity = boost::lexical_cast<uint64_t>(line[2]);
		result.company = boost::lexical_cast<Id>(line[3]);
		result.offervalue = boost::lexical_cast<float>(line[4]);
		result.brand = boost::lexical_cast<Id>(line[5]);
		return result;
	}
}
