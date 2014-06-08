#pragma once

#include <memory>
#include <array>
#include <random>

#include "util/reader.h"
#include "util/writer.h"

namespace Hivemind
{

class Splitter
{
    Splitter() = delete;
    Splitter(Splitter&) = delete;
    Splitter& operator=(Splitter&) = delete;

public:
    template<typename T, size_t N>
    static void split(std::shared_ptr<Reader<T>> reader, std::array<std::shared_ptr<Writer<T>>, N> writers)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, N-1);

        T x;
        while(reader->read(x))
            writers[dis(gen)]->write(x);
    }
};

}
