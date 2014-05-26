#pragma once

#include <string>
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>

#include "util/csvreader.h"
#include "util/msgpackreader.h"
#include "util/msgpackwriter.h"

#include "data/offer.h"
#include "data/transaction.h"
#include "data/history.h"
#include "data/client.h"

namespace Hivemind {

	class Cache
	{
		Cache() = delete;
		Cache(const Cache&) = delete;
		Cache& operator=(const Cache&) = delete;

	public:
		enum class ReaderOption
		{
			csv,
			msgpack
		};

		class NotAvailableException : public std::runtime_error
		{
		public:
			NotAvailableException(const std::string& filename)
			: std::runtime_error(filename + " does not exist.")
			{}
		};

		static std::string getPath(const std::string& filename, const std::string& datadir, const ReaderOption o)
		{
			switch(o)
			{
			case ReaderOption::csv:
				return datadir+"/"+filename+".csv.gz";
			case ReaderOption::msgpack:
				return datadir+"/"+filename+".msgpack.gz";
			default:
				throw std::runtime_error("Unknown ReaderOption");
			}
		}

		static boost::optional<std::pair<ReaderOption, std::string>> findFastest(const std::string& filename, const std::string& datadir)
		{
			std::string path;

			path = getPath(filename, datadir, ReaderOption::csv);
			if(boost::filesystem::exists(path))
				return std::make_pair(ReaderOption::csv, path);

			path = getPath(filename, datadir, ReaderOption::msgpack);
			if(boost::filesystem::exists(path))
				return std::make_pair(ReaderOption::msgpack, path);

			return boost::none;
		}

		static bool isAvailable(const std::string& filename, const std::string& datadir)
		{
			return (bool)findFastest(filename, datadir);
		}

		template<typename T>
		static std::shared_ptr<Reader<T>> getFastestReader(const std::string& filename, const std::string& datadir)
		{
			auto pair = findFastest(filename, datadir);

			if(!pair)
				throw NotAvailableException(filename);

			switch(pair->first)
			{
				case ReaderOption::csv:
				{
					std::shared_ptr<CsvReader<T>> ptr(new CsvReader<T>(pair->second));
					return std::static_pointer_cast<Reader<T>, CsvReader<T>>(ptr);
				}
				case ReaderOption::msgpack:
				{
					std::shared_ptr<MsgpackReader<T>> ptr(new MsgpackReader<T>(pair->second));
					return std::static_pointer_cast<Reader<T>, MsgpackReader<T>>(ptr);
				}
				default:
				throw std::runtime_error("Unknown ReaderOption returned");
			}
		}

		template<typename T>
		static void generateCache(Reader<T>& reader, Writer<T>& writer)
		{
			T x;
			while(reader.read(x))
				writer.write(x);
		}

		template<typename T>
		static void generateCache(const std::string& filename, const std::string& datadir)
		{
			const std::string fileOld = getPath(filename, datadir, ReaderOption::csv);
			const std::string fileNew = getPath(filename, datadir, ReaderOption::msgpack);

			if(boost::filesystem::exists(fileNew))
				return;

			if(!boost::filesystem::exists(fileOld))
				throw std::runtime_error(fileOld + " does not exist.");

			std::cerr << "Generating " << filename << " cache." << std::endl;

			CsvReader<T> r(fileOld);
			MsgpackWriter<T> w(fileNew);
			generateCache<T>(r, w);
		}

		static void clearCache(const std::string& filename, const std::string& datadir)
		{
			boost::filesystem::remove(getPath(filename, datadir, ReaderOption::msgpack));
		}

		static void checkDatadir(const std::string& datadir)
		{
			if(!boost::filesystem::exists(datadir))
				throw std::runtime_error(datadir + " does not exist.");
		}
	};

}
