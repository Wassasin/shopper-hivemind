#pragma once

#include <msgpack.hpp>
#include <vector>
#include <string>

#include "typedefs.h"
#include "productmeasure.h"

namespace hivemind {

	class transaction
	{
	public:
		id_t id;
		id_t chain;
		id_t dept;
		id_t category;
		id_t company;
		id_t brand;
		date_t date;
		float productsize;
		productmeasure_e productmeasure;
		uint64_t purchasequantity;
		float purchaseamount;

		static transaction from_line(const std::vector<std::string>& line);

		// Macro for serialization
		MSGPACK_DEFINE(id, chain, dept, category, company, brand, date, productsize, productmeasure, purchasequantity, purchaseamount);
	};

}
