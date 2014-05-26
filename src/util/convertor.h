#pragma once

#include <string>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>

#include "csvreader.h"
#include "msgpackreader.h"
#include "msgpackwriter.h"

#include "../offer.h"
#include "../transaction.h"
#include "../history.h"
#include "../client.h"

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
			MsgpackWriter<T> w(msgpackFilename);
			CsvReader<T> r(csvFilename);

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

			std::map<Id, TrainClient> clients;

			{
				TrainHistory th;
				MsgpackReader<TrainHistory> r("../data/trainHistory.msgpack.gz");
				while(r.read(th))
					clients.emplace(th.id, TrainClient(th));
			}

			std::cerr << "Got " << clients.size() << " clients" << std::endl;

			{
				Transaction tr;
				MsgpackWriter<TrainClient> w("../data/trainClients.msgpack.gz");
				MsgpackReader<Transaction> r("../data/transactions.msgpack.gz");

				boost::optional<TrainClient> tc;
				boost::optional<Basket> b;

				std::set<Id> processed;

				while(r.read(tr))
				{
					// We've suddenly got a different client to work on
					if(tc && tc->id != tr.id)
					{
						// Write the old client to file
						w.write(*tc);
						processed.insert(tc->id);
						clients.erase(tc->id);
						tc.reset();
						b.reset();
					}

					if(!tc)
					{
						// Does this transaction belong to a client we know?
						auto it = clients.find(tr.id);
						if(it == clients.end()) // No, OK, we'll wait
						{
							if(processed.find(tr.id) != processed.end())
								throw std::runtime_error("File is not ordered on id, assumption broken");

							continue;
						}

						// Prepare our client so that we can add the transactions
						tc = it->second;
					}

					// We've got a client we're working on, so write the transaction
					if(tc)
					{
						if(b && (b->chain != tr.chain || b->date != tr.date))
						{
							tc->baskets.push_back(*b);
							b.reset();
						}

						if(!b)
							b = Basket(tr.chain, tr.date);

						b->items.push_back(Basketitem(tr));
					}
				}

				if(tc)
					w.write(*tc);
			}
		}
	};

}
