#pragma once

#include <msgpack.hpp>
#include <vector>
#include <string>

#include "typedefs.h"
#include "data/productmeasure.h"

namespace Hivemind {

    class Transaction
    {
    public:
        Id id;
        Id chain;
        Id dept;
        Id category;
        Id company;
        Id brand;
        Date date;
        float productsize;
        Productmeasure productmeasure;
        uint64_t purchasequantity;
        float purchaseamount;

        Transaction()
        : id(), chain(), dept(), category(), company(), brand(), date(), productsize(), productmeasure(), purchasequantity(), purchaseamount() {}

        static Transaction from_line(const std::vector<std::string>& line);

        // Macro for serialization
        MSGPACK_DEFINE(id, chain, dept, category, company, brand, date, productsize, productmeasure, purchasequantity, purchaseamount)
    };

}
