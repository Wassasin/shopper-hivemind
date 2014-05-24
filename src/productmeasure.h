#pragma once

#include <map>
#include <cstdint>
#include <msgpack.hpp>

namespace hivemind
{
	enum class productmeasure_e : std::uint8_t
	{
		items, CT, FT, GL, LB, LT, OZ, PT, QT, RL, YD
	};

	inline productmeasure_e productmeasure_from_string(const std::string& str)
	{
		typedef productmeasure_e pm;

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

	inline hivemind::productmeasure_e& operator>>(object o, hivemind::productmeasure_e& x)
	{
		if(o.type != type::POSITIVE_INTEGER)
			throw type_error();

		x = (hivemind::productmeasure_e) o.via.u64;
		return x;
	}

	template <typename Stream>
	inline packer<Stream>& operator<<(packer<Stream>& o, const hivemind::productmeasure_e& x)
	{
		o.pack_uint8((std::uint8_t)x);
		return o;
	}

}
