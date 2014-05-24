#pragma once

#include <fstream>
#include <string>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include "csvparser.h"

namespace Hivemind
{
	template<typename T>
	class CsvReader
	{
		std::ifstream m_fi;
		boost::iostreams::filtering_istreambuf m_si;
		std::istream m_sia;
		CsvParser m_parser;

		CsvReader(const CsvReader&) = delete;
		CsvReader& operator=(const CsvReader&) = delete;

	public:
		CsvReader(std::string filename, bool skip_first = true)
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
				m_parser.readLine(nop);
			}
		}

		~CsvReader()
		{
			boost::iostreams::close(m_si);
			m_fi.close();
		}

		bool read(T& x_ref)
		{
			std::vector<std::string> line;
			if(!m_parser.readLine(line))
				return false;

			x_ref = T::from_line(line);
			return true;
		}
	};
}
