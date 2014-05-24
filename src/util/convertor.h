#pragma once

#include <string>
#include <boost/filesystem.hpp>

#include "csv_reader.h"
#include "msgpack_reader.h"
#include "msgpack_writer.h"

#include "../offer.h"
#include "../transaction.h"
#include "../history.h"

namespace Hivemind {

	class Convertor
	{
		Convertor() = delete;
		Convertor(const Convertor&) = delete;
		Convertor& operator=(const Convertor&) = delete;

	public:
		template<typename T>
		static void csvToMsgpack(const std::string& csvFilename, const std::string& msgpackFilename)
		{
			msgpack_writer<T> w(msgpackFilename);
			csv_reader<T> r(csvFilename);

			T x;
			while(r.read(x))
				w.write(x);
		}

		template<typename T>
		static void preprocess(const std::string& filename, const std::string& datadir)
		{
			const std::string fileOld = datadir+filename+".csv.gz";
			const std::string fileNew = datadir+filename+".msgpack.gz";

			if(boost::filesystem::exists(fileNew))
			{
				std::cerr << "Got " << filename << " from cache" << std::endl;
				return;
			}

			if(!boost::filesystem::exists(fileOld))
				throw std::runtime_error(fileOld + " does not exist.");

			std::cerr << "Pre-processing " << filename << std::endl;
			Convertor::csvToMsgpack<T>(fileOld, fileNew);
		}

		static void createDataset()
		{
			std::cerr << "Relating data..." << std::endl;
			std::map<Id, Offer> offers;

			{
				Offer o;
				msgpack_reader<Offer> r("../data/offers.msgpack.gz");
				while(r.read(o))
					offers.insert(std::make_pair(o.id, o));
			}

			std::cerr << "Got " << offers.size() << " offers" << std::endl;

			std::map<Id, TrainHistory> clients;
			{
				TrainHistory th;
				msgpack_reader<TrainHistory> r("../data/trainHistory.msgpack.gz");
				while(r.read(th))
					clients.insert(std::make_pair(th.h.id, th));
			}

			std::cerr << "Got " << clients.size() << " clients" << std::endl;

			{
				Transaction t;
				msgpack_writer<TrainHistory> w("../data/trainHistory_merged.msgpack.gz");
				msgpack_reader<Transaction> r("../data/transactions.msgpack.gz");

				Id past_id;
				TrainHistory th;
				bool empty = true;

				std::set<Id> processed;

				while(r.read(t))
				{
					// We've suddenly got a different client to work on
					if(past_id != t.id)
					{
						// Write the old client to file
						if(!empty)
						{
							w.write(th);
							processed.insert(past_id);
							clients.erase(past_id);
							empty = true;
						}

						// Does this transaction belong to a client we know?
						past_id = t.id;
						auto it = clients.find(past_id);
						if(it == clients.end()) // No, OK, we'll wait
						{
							if(processed.find(past_id) != processed.end())
								throw std::runtime_error("File is not ordered on id, assumption broken");

							continue;
						}

						// Prepare our client so that we can add the transactions
						th = it->second;
						empty = false;
					}

					// We've got a client we're working on, so write the transaction
					if(!empty)
						th.h.transactions.insert(std::make_pair(t.date, t));
				}
			}
		}
	};

}
