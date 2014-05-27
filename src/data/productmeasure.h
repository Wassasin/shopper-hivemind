#pragma once

#include <map>
#include <cstdint>
#include <msgpack.hpp>

namespace Hivemind
{
	enum class Productmeasure : std::uint8_t
	{
		items, CT, FT, GL, LB, LT, OZ, PT, QT, RL, YD
	};

	inline Productmeasure toProductmeasure(const std::string& str)
	{
		typedef Productmeasure pm;

		static const std::map<std::string, pm> mapping = {
			{"", pm::items},
			{"1", pm::items}, // Yeah, I don't know too
			{"CT", pm::CT},
			{"FT", pm::FT},
			{"GL", pm::GL},
			{"LB", pm::LB},
			{"LT", pm::LT},
			{"OZ", pm::OZ},
			{"PT", pm::PT},
			{"QT", pm::QT},
			{"RL", pm::RL},
			{"YD", pm::YD}
		};

		try
		{
			return mapping.at(str);
		} catch(std::out_of_range)
		{
			throw std::runtime_error("Unknown productmeasure \"" + str + "\"");
		}
	}

}

namespace msgpack {

	inline Hivemind::Productmeasure& operator>>(object o, Hivemind::Productmeasure& x)
	{
		if(o.type != type::POSITIVE_INTEGER)
			throw type_error();

		x = (Hivemind::Productmeasure) o.via.u64;
		return x;
	}

	template <typename Stream>
	inline packer<Stream>& operator<<(packer<Stream>& o, const Hivemind::Productmeasure& x)
	{
		o.pack_uint8((std::uint8_t)x);
		return o;
	}

}
