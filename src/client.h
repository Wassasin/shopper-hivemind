#pragma once

#include "typedefs.h"
#include "transaction.h"
#include "history.h"

namespace Hivemind
{
	class Basketitem
	{
	public:
		Id dept;
		Id category;
		Id company;
		Id brand;
		float productsize;
		Productmeasure productmeasure;
		uint64_t purchasequantity;
		float purchaseamount;

		Basketitem()
		: dept(), category(), company(), brand(), productsize(), productmeasure(), purchasequantity(), purchaseamount() {}

		Basketitem(const Transaction& t)
		: dept(t.dept)
		, category(t.category)
		, company(t.company)
		, brand(t.brand)
		, productsize(t.productsize)
		, productmeasure(t.productmeasure)
		, purchasequantity(t.purchasequantity)
		, purchaseamount(t.purchaseamount)
		{}

		MSGPACK_DEFINE(dept, category, company, brand, productsize, productmeasure, purchasequantity, purchaseamount)
	};

	class Basket
	{
	public:
		Id chain;
		Date date;
		std::vector<Basketitem> items;

		Basket()
		: chain(), date(), items() {}

		Basket(const Id chain, const Date date)
		: chain(chain)
		, date(date)
		, items()
		{}

		MSGPACK_DEFINE(chain, date, items)
	};

	class Client
	{
	public:
		Id id;
		std::vector<Basket> baskets;

		Client()
		: id(), baskets() {}

		Client(const Id id)
		: id(id)
		, baskets()
		{}

		virtual ~Client() {}

		MSGPACK_DEFINE(id, baskets)
	};

	class ClientOffer
	{
	public:
		Id client;
		Id chain;
		Id offer;
		Id market;
		Date offerdate;

		ClientOffer()
		: client(), chain(), offer(), market(), offerdate() {}

		ClientOffer(const History& h)
		: client(h.id)
		, chain(h.chain)
		, offer(h.offer)
		, market(h.market)
		, offerdate(h.offerdate)
		{}

		virtual ~ClientOffer() {}

		MSGPACK_DEFINE(client, chain, offer, market, offerdate)
	};

	class TrainClientOffer : public ClientOffer
	{
	public:
		uint64_t repeattrips;
		bool repeater;

		TrainClientOffer()
		: ClientOffer(), repeattrips(), repeater() {}

		TrainClientOffer(const TrainHistory& th)
		: ClientOffer(th)
		, repeattrips(th.repeattrips)
		, repeater(th.repeater)
		{}

		MSGPACK_DEFINE(client, chain, offer, market, offerdate, repeattrips, repeater)
	};

	class TrainClient : public Client
	{
	public:
		TrainClientOffer offer;

		TrainClient()
		: Client(), offer() {}

		TrainClient(const TrainHistory& th)
		: Client(th.id)
		, offer(th)
		{}

		MSGPACK_DEFINE(id, baskets, offer)
	};
}
