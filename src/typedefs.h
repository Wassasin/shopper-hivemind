#pragma once

#include <cstdint>
#include <msgpack.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>

namespace Hivemind {

    typedef std::uint64_t Id;
    typedef float Probability;
    typedef float Feature;
    typedef bool  Prediction;
    typedef boost::gregorian::date Date;

    Date toDate(std::string str);

}

namespace msgpack {

    inline Hivemind::Date& operator>>(object o, Hivemind::Date& x)
    {
        uint16_t y, m, d;

        if(o.type != type::ARRAY) { throw type_error(); }
        if(o.via.array.size != 3) { throw type_error(); }
        o.via.array.ptr[0].convert(&y);
        o.via.array.ptr[1].convert(&m);
        o.via.array.ptr[2].convert(&d);

        x = Hivemind::Date(y, m, d);

        return x;
    }

    template <typename Stream>
    inline packer<Stream>& operator<<(packer<Stream>& o, const Hivemind::Date& x)
    {
        o.pack_array(3);
        o.pack((uint16_t)x.year());
        o.pack((uint16_t)x.month());
        o.pack((uint16_t)x.day());
        return o;
    }

}
