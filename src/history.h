#pragma once

#include <vector>
#include <string>
#include <msgpack.hpp>

#include "typedefs.h"
#include "transaction.h"

namespace hivemind
{
	class history
	{
	public:
		id_t id;
		id_t chain;
		id_t offer;
		id_t market;
		date_t offerdate;

		std::multimap<date_t, transaction> transactions;

		static history from_line(const std::vector<std::string>& line);

		// Macro for serialization
		MSGPACK_DEFINE(id, chain, offer, market, offerdate, transactions);
	};

	class train_history
	{
	public:
		history h;
		uint64_t repeattrips;
		bool repeater;

		static train_history from_line(const std::vector<std::string>& line);

		// Macro for serialization
		MSGPACK_DEFINE(h, repeattrips, repeater);
	};
}
