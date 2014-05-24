#include "transaction.h"

#include <boost/lexical_cast.hpp>

namespace hivemind {

	transaction transaction::from_line(const std::vector<std::string>& line)
	{
		if(line.size() != 11)
			throw std::runtime_error("Transaction csv is not valid");

		transaction result;
		result.id = boost::lexical_cast<id_t>(line[0]);
		result.chain = boost::lexical_cast<id_t>(line[1]);
		result.dept = boost::lexical_cast<id_t>(line[2]);
		result.category = boost::lexical_cast<id_t>(line[3]);
		result.company = boost::lexical_cast<id_t>(line[4]);
		result.brand = boost::lexical_cast<id_t>(line[5]);
		result.date = to_date(line[6]);
		result.productsize = boost::lexical_cast<float>(line[7]);
		result.productmeasure = productmeasure_from_string(line[8]);
		result.purchasequantity = boost::lexical_cast<uint64_t>(line[9]);
		result.purchaseamount = boost::lexical_cast<float>(line[10]);
		return result;
	}

}
