#pragma once

#include <memory>

#include "util/reader.h"

#include "data/transaction.h"
#include "data/client.h"

namespace Hivemind
{
	template<typename HISTORY, typename CLIENT>
	class Joiner : public Reader<CLIENT>
	{
		std::shared_ptr<Reader<HISTORY>> rHistory;
		std::shared_ptr<Reader<Transaction>> rTransaction;

		std::set<Id> processedClients;

		boost::optional<Transaction> transaction;

	public:
		Joiner(const decltype(rHistory) rHistory, const decltype(rTransaction) rTransaction)
		: rHistory(rHistory)
		, rTransaction(rTransaction)
		, processedClients()
		, transaction()
		{}

		/*
		 * assumption: transactions are in order
		 * assumption: baskets are in order
		 */
		bool read(CLIENT& client)
		{
			HISTORY h;
			if(!rHistory->read(h))
				return false;

			client = CLIENT(h);
			assert(client.baskets.size() == 0);

			boost::optional<Basket> b = boost::none;
			bool in_client_section = false;
			while(true)
			{
				if(!transaction)
				{
					Transaction tmp;
					if(!rTransaction->read(tmp)) // EOF
						break;

					transaction = tmp;

					if(processedClients.find(transaction->id) != processedClients.end())
						throw std::runtime_error("Client has already been processed; transactions is not in order.");
				}

				if(!in_client_section)
				{
					if(transaction->id == client.id)
						in_client_section = true;
					else // We need to wait until the first transaction of our client arrives
					{
						transaction = boost::none;
						continue;
					}
				}

				if(in_client_section)
				{
					if(transaction->id != client.id)
						break; // We've completed our part of the list, we're done

					if(!b || (b->chain != transaction->chain || b->date != transaction->date))
					{
						if(b)
							client.baskets.push_back(b.get());

						b = Basket(transaction->chain, transaction->date);
						assert(b->items.size() == 0);
					}

					b.get().items.push_back(Basketitem(*transaction));
					transaction = boost::none;
				}
			}

			if(b)
				client.baskets.push_back(*b);

			processedClients.insert(client.id);
			return true;
		}
	};
}
