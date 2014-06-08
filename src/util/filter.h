#pragma once

#include <random>
#include <set>

#include "util/reader.h"

namespace Hivemind
{

template<typename T>
class Filter : public Reader<T>
{
    std::shared_ptr<Reader<T>> rParent;

    std::set<size_t> chosen;
    std::set<size_t>::const_iterator chosenIt;

    size_t i;

public:
    Filter(decltype(rParent) rParent, size_t n, size_t size)
    : rParent(rParent)
    , chosen()
    , chosenIt()
    , i(0)
    {
        assert(n <= size);

        std::vector<size_t> bag;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, size-1);

        // TODO Could be implemented better with std::iota and uniform random shuffle
        while(chosen.size() < n)
            chosen.insert(dis(gen));

        chosenIt = chosen.begin();
    }

    virtual bool read(T& x_ref)
    {
        while(true)
        {
            if(!rParent->read(x_ref))
                return false;

            if(chosenIt == chosen.end())
                return false;

            if(*chosenIt == i)
            {
                i++;
                chosenIt++;
                return true;
            }

            i++;
        }
    }
};

}
