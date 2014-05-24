#pragma once

#include <string>
#include <boost/filesystem.hpp>

#include "csv_reader.h"
#include "msgpack_reader.h"
#include "msgpack_writer.h"

#include "../offer.h"
#include "../transaction.h"
#include "../history.h"

namespace hivemind {

	class convertor
	{
		convertor() = delete;
		convertor(const convertor&) = delete;
		convertor& operator=(const convertor&) = delete;

	public:
		template<typename T>
		static void csv_to_msgpack(const std::string& csv_filename, const std::string& msgpack_filename)
		{
			msgpack_writer<T> w(msgpack_filename);
			csv_reader<T> r(csv_filename);

			T x;
			while(r.read(x))
				w.write(x);
		}

		template<typename T>
		static void preprocess(const std::string& filename, const std::string& datadir)
		{
			const std::string file_old = datadir+filename+".csv.gz";
			const std::string file_new = datadir+filename+".msgpack.gz";

			if(boost::filesystem::exists(file_new))
			{
				std::cerr << "Got " << filename << " from cache" << std::endl;
				return;
			}

			if(!boost::filesystem::exists(file_old))
				throw std::runtime_error(file_old + " does not exist.");

			std::cerr << "Pre-processing " << filename << std::endl;
			convertor::csv_to_msgpack<T>(file_old, file_new);
		}

		static void create_dataset()
		{
			std::cerr << "Relating data..." << std::endl;
			std::map<id_t, offer> offers;

			{
				offer o;
				msgpack_reader<offer> r("../data/offers.msgpack.gz");
				while(r.read(o))
					offers.insert(std::make_pair(o.id, o));
			}

			std::cerr << "Got " << offers.size() << " offers" << std::endl;

			std::map<id_t, train_history> clients;
			{
				train_history th;
				msgpack_reader<train_history> r("../data/trainHistory.msgpack.gz");
				while(r.read(th))
					clients.insert(std::make_pair(th.h.id, th));
			}

			std::cerr << "Got " << clients.size() << " clients" << std::endl;

			{
				transaction t;
				msgpack_writer<train_history> w("../data/trainHistory_merged.msgpack.gz");
				msgpack_reader<transaction> r("../data/transactions.msgpack.gz");

				id_t past_id;
				train_history th;
				bool empty = true;

				std::set<id_t> processed;

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
