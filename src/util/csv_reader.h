#pragma once

#include <fstream>
#include <string>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include "csv_parser.h"

namespace hivemind
{
	template<typename T>
	class csv_reader
	{
		std::ifstream m_fi;
		boost::iostreams::filtering_istreambuf m_si;
		std::istream m_sia;
		csv_parser m_parser;

		csv_reader(const csv_reader&) = delete;
		csv_reader& operator=(const csv_reader&) = delete;

	public:
		csv_reader(std::string filename, bool skip_first = true)
		: m_fi(filename, std::ios_base::binary)
		, m_si()
		, m_sia(&m_si)
		, m_parser(m_sia)
		{
			m_si.push(boost::iostreams::gzip_decompressor());
			m_si.push(m_fi);

			if(skip_first)
			{
				std::vector<std::string> nop;
				m_parser.read_line(nop);
			}
		}

		~csv_reader()
		{
			boost::iostreams::close(m_si);
			m_fi.close();
		}

		bool read(T& x_ref)
		{
			std::vector<std::string> line;
			if(!m_parser.read_line(line))
				return false;

			x_ref = T::from_line(line);
			return true;
		}
	};
}
