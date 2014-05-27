#include "history.h"

#include <boost/lexical_cast.hpp>

namespace Hivemind
{
	History History::from_line(const std::vector<std::string>& line)
	{
		if(line.size() != 5)
			throw std::runtime_error("History csv is not valid");

		History result;
		result.id = boost::lexical_cast<Id>(line[0]);
		result.chain = boost::lexical_cast<Id>(line[1]);
		result.offer = boost::lexical_cast<Id>(line[2]);
		result.market = boost::lexical_cast<Id>(line[3]);
		result.offerdate = toDate(line[4]);
		return result;
	}

	TrainHistory TrainHistory::from_line(const std::vector<std::string>& line)
	{
		if(line.size() != 7)
			throw std::runtime_error("TrainHistory csv is not valid");

		TrainHistory result;
		result.id = boost::lexical_cast<Id>(line[0]);
		result.chain = boost::lexical_cast<Id>(line[1]);
		result.offer = boost::lexical_cast<Id>(line[2]);
		result.market = boost::lexical_cast<Id>(line[3]);
		result.repeattrips = boost::lexical_cast<uint64_t>(line[4]);
		result.repeater = (line[5] == "t");
		result.offerdate = toDate(line[6]);
		return result;
	}
}
