#pragma once

#include <vector>
#include <string>
#include <msgpack.hpp>

#include "typedefs.h"
#include "data/transaction.h"

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

		History()
		: id(), chain(), offer(), market(), offerdate() {}

		virtual ~History() {}

		static History from_line(const std::vector<std::string>& line);

		// Macro for serialization
		MSGPACK_DEFINE(id, chain, offer, market, offerdate)
	};

	class TrainHistory : public History
	{
	public:
		uint64_t repeattrips;
		bool repeater;

		TrainHistory()
		: History(), repeattrips(), repeater() {}

		static TrainHistory from_line(const std::vector<std::string>& line);

		// Macro for serialization
		MSGPACK_DEFINE(id, chain, offer, market, offerdate, repeattrips, repeater)
	};
}
