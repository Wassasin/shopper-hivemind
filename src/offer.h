#pragma once

#include <cstdint>
#include <msgpack.hpp>

#include "typedefs.h"

namespace hivemind
{
	class offer
	{
	public:
		id_t id;
		id_t category;
		uint64_t quantity;
		id_t company;
		float offervalue;
		id_t brand;

		static offer from_line(const std::vector<std::string>& line);

		// Macro for serialization
		MSGPACK_DEFINE(id, category, quantity, company, offervalue, brand);
	};
}
