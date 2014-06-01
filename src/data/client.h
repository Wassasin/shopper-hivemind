#pragma once

#include "typedefs.h"
#include "data/transaction.h"
#include "data/history.h"

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

        Id chain;
        Id offer;
        Id market;
        Date offerDate;

        std::vector<Basket> baskets;

        Client()
        : id(), chain(), offer(), market(), offerDate(), baskets() {}

        Client(const History& h)
        : id(h.id)
        , chain(h.chain)
        , offer(h.offer)
        , market(h.market)
        , offerDate(h.offerDate)
        , baskets()
        {}

        virtual ~Client() {}

        MSGPACK_DEFINE(id, chain, offer, market, offerDate, baskets)
    };

    class TrainClient : public Client
    {
    public:
        uint64_t repeattrips;
        bool repeater;

        TrainClient()
        : Client(), repeattrips(), repeater() {}

        TrainClient(const TrainHistory& th)
        : Client(th)
        , repeattrips(th.repeattrips)
        , repeater(th.repeater)
        {}

        MSGPACK_DEFINE(id, chain, offer, market, offerDate, baskets, repeattrips, repeater)
    };
}
