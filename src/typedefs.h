#pragma once

#include <cstdint>
#include <msgpack.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>

namespace hivemind {

	typedef uint64_t id_t;
	typedef boost::gregorian::date date_t;

	date_t to_date(std::string str);

}

namespace msgpack {

	inline hivemind::date_t& operator>>(object o, hivemind::date_t& x)
	{
		uint16_t y, m, d;

		if(o.type != type::ARRAY) { throw type_error(); }
		if(o.via.array.size != 3) { throw type_error(); }
		o.via.array.ptr[0].convert(&y);
		o.via.array.ptr[1].convert(&m);
		o.via.array.ptr[2].convert(&d);

		x = hivemind::date_t(y, m, d);

		return x;
	}

	template <typename Stream>
	inline packer<Stream>& operator<<(packer<Stream>& o, const hivemind::date_t& x)
	{
		o.pack_array(3);
		o.pack((uint16_t)x.year());
		o.pack((uint16_t)x.month());
		o.pack((uint16_t)x.day());
		return o;
	}

}
