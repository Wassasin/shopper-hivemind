#pragma once

#include <vector>
#include <string>
#include <msgpack.hpp>

#include "typedefs.h"
#include "transaction.h"

namespace Hivemind
{
	class History
	{
	public:
		Id id;
		Id chain;
		Id offer;
		Id market;
		Date offerdate;

		std::multimap<Date, Transaction> transactions;

		static History from_line(const std::vector<std::string>& line);

		// Macro for serialization
		MSGPACK_DEFINE(id, chain, offer, market, offerdate, transactions);
	};

	class TrainHistory
	{
	public:
		History h;
		uint64_t repeattrips;
		bool repeater;

		static TrainHistory from_line(const std::vector<std::string>& line);

		// Macro for serialization
		MSGPACK_DEFINE(h, repeattrips, repeater);
	};
}
