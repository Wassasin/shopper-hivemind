#include "history.h"

#include <boost/lexical_cast.hpp>

namespace hivemind
{
	history history::from_line(const std::vector<std::string>& line)
	{
		if(line.size() != 5)
			throw std::runtime_error("History csv is not valid");

		history result;
		result.id = boost::lexical_cast<id_t>(line[0]);
		result.chain = boost::lexical_cast<id_t>(line[1]);
		result.offer = boost::lexical_cast<id_t>(line[2]);
		result.market = boost::lexical_cast<id_t>(line[3]);
		result.offerdate = to_date(line[4]);
		return result;
	}

	train_history train_history::from_line(const std::vector<std::string>& line)
	{
		if(line.size() != 7)
			throw std::runtime_error("TrainHistory csv is not valid");

		train_history result;
		result.h.id = boost::lexical_cast<id_t>(line[0]);
		result.h.chain = boost::lexical_cast<id_t>(line[1]);
		result.h.offer = boost::lexical_cast<id_t>(line[2]);
		result.h.market = boost::lexical_cast<id_t>(line[3]);
		result.repeattrips = boost::lexical_cast<uint64_t>(line[4]);
		result.repeater = (line[5] == "t");
		result.h.offerdate = to_date(line[6]);
		return result;
	}
}
