#include "transaction.h"

#include <boost/lexical_cast.hpp>

namespace Hivemind {

	Transaction Transaction::from_line(const std::vector<std::string>& line)
	{
		if(line.size() != 11)
			throw std::runtime_error("Transaction csv is not valid");

		Transaction result;
		result.id = boost::lexical_cast<Id>(line[0]);
		result.chain = boost::lexical_cast<Id>(line[1]);
		result.dept = boost::lexical_cast<Id>(line[2]);
		result.category = boost::lexical_cast<Id>(line[3]);
		result.company = boost::lexical_cast<Id>(line[4]);
		result.brand = boost::lexical_cast<Id>(line[5]);
		result.date = toDate(line[6]);
		result.productsize = boost::lexical_cast<float>(line[7]);
		result.productmeasure = toProductmeasure(line[8]);
		result.purchasequantity = boost::lexical_cast<uint64_t>(line[9]);
		result.purchaseamount = boost::lexical_cast<float>(line[10]);
		return result;
	}

}
