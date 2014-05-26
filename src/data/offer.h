#pragma once

#include <cstdint>
#include <msgpack.hpp>

#include "typedefs.h"

namespace Hivemind
{
	class Offer
	{
	public:
		Id id;
		Id category;
		uint64_t quantity;
		Id company;
		float offervalue;
		Id brand;

		static Offer from_line(const std::vector<std::string>& line);

		// Macro for serialization
		MSGPACK_DEFINE(id, category, quantity, company, offervalue, brand)
	};
}
