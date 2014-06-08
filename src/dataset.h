#pragma once

#include <array>
#include <string>

#include "cache.h"

#include "joiner.h"

#include "util/filter.h"
#include "util/splitter.h"

#include "data/offer.h"
#include "data/transaction.h"
#include "data/history.h"

namespace Hivemind
{

class Dataset
{
private:
    bool debug, generate_cache;
    std::string datadir;

    static const std::string nameOffers,
                             nameTrainHistory,
                             nameTestHistory,
                             nameTransactions,

                             nameTrainClients,
                             nameTestClients,

                             nameCVTrainClients,
                             nameCVTuneClients,
                             nameCVTestClients;

    template<typename HISTORY, typename CLIENT>
    std::shared_ptr<Reader<CLIENT>> getClientData(const std::string& filename, std::shared_ptr<Reader<HISTORY>> rHistory, std::shared_ptr<Reader<Transaction>> rTransaction)
    {
        std::shared_ptr<Reader<CLIENT>> rClient;

        {
           std::string path = Cache::getPath(filename, datadir, Cache::ReaderOption::msgpack);
           if(!Cache::isAvailable(filename, datadir))
           {
               rClient.reset(new Joiner<HISTORY, CLIENT>(rHistory, rTransaction));

               if(generate_cache)
               {
                   std::cerr << "Generating cache for ClientData" << std::endl;

                   MsgpackWriter<CLIENT> wClient(path);
                   Cache::generateCache(*rClient, wClient);
                   rClient.reset(new MsgpackReader<CLIENT>(path));
               }
           }
           else
               rClient.reset(new MsgpackReader<CLIENT>(path));
        }

        return rClient;
    }

    static std::string getDebugName(std::string filename)
    {
        return filename + "Debug";
    }

    template<typename T>
    std::shared_ptr<Reader<T>> getDebugData(std::string filename, std::shared_ptr<Reader<T>> rOriginal, size_t desiredNumber, size_t setSize)
    {
        std::string debugFilename = getDebugName(filename);
        std::string path = Cache::getPath(debugFilename, datadir, Cache::ReaderOption::msgpack);

        if(!Cache::isAvailable(debugFilename, datadir))
        {
            std::shared_ptr<Reader<T>> rDebug(new Filter<T>(rOriginal, desiredNumber, setSize));

            if(!generate_cache)
                return rDebug;

            {
                std::shared_ptr<Writer<T>> wDebug(new MsgpackWriter<T>(path));

                T tmp;
                while(rDebug->read(tmp))
                    wDebug->write(tmp);
            }
        }

        assert(Cache::isAvailable(debugFilename, datadir));
        return std::make_shared<MsgpackReader<T>>(path);
    }

public:
    struct RawReaders
    {
        std::shared_ptr<Reader<Offer>> rOffers;
        std::shared_ptr<Reader<TrainHistory>> rTrainHistory;
        std::shared_ptr<Reader<History>> rTestHistory;
        std::shared_ptr<Reader<Transaction>> rTransactions;
    };

    struct RealReaders
    {
        std::shared_ptr<Reader<TrainClient>> rTrainClients;
        std::shared_ptr<Reader<Client>> rTestClients;
    };

    struct CVReaders
    {
        std::shared_ptr<Reader<TrainClient>> rTrainClients, rTuneClients, rTestClients;
    };

    Dataset(std::string datadir, bool generate_cache, bool debug)
        : debug(debug)
        , generate_cache(generate_cache)
        , datadir(datadir)
    {}

    static void clearCache(std::string datadir)
    {
        std::vector<std::string> names = {nameOffers, nameTrainHistory, nameTestHistory, nameTransactions, nameTrainClients, nameTestClients};

        for(std::string name : names)
        {
            Cache::clearCache(name, datadir);
            Cache::clearCache(getDebugName(name), datadir);
        }
    }

    RawReaders getRawReaders()
    {
        if(generate_cache)
        {
            Cache::generateCache<Offer>(nameOffers, datadir);
            Cache::generateCache<TrainHistory>(nameTrainHistory, datadir);
            Cache::generateCache<History>(nameTestHistory, datadir);
            Cache::generateCache<Transaction>(nameTransactions, datadir);
        }

        RawReaders result;
        result.rOffers = Cache::getFastestReader<Offer>(nameOffers, datadir);
        result.rTrainHistory = Cache::getFastestReader<TrainHistory>(nameTrainHistory, datadir);
        result.rTestHistory = Cache::getFastestReader<History>(nameTestHistory, datadir);
        result.rTransactions = Cache::getFastestReader<Transaction>(nameTransactions, datadir);
        return result;
    }

    RealReaders getRealReaders()
    {
        RealReaders result;

        {
            RawReaders readers = getRawReaders(); // A bit wasteful
            result.rTrainClients = getClientData<TrainHistory, TrainClient>(nameTrainClients, readers.rTrainHistory, readers.rTransactions);

            if(debug)
                result.rTrainClients = getDebugData<TrainClient>(nameTrainClients, result.rTrainClients, 1000, 160058); // Hardcoded, yes, I know
        }

        {
            RawReaders readers = getRawReaders(); // A bit wasteful
            result.rTestClients = getClientData<History, Client>(nameTestClients, readers.rTestHistory, readers.rTransactions);

            if(debug)
                result.rTestClients = getDebugData<Client>(nameTestClients, result.rTestClients, 1000, 151485);
        }

        return result;
    }

    /*CVReaders getCVReaders()
    {
        StructuredReaders<Client> result;

        {
            RawReaders readers = getRawReaders(); // A bit wasteful
            result.rTrainClients = getClientData<TrainHistory, TrainClient>(nameTrainClients, readers.rTrainHistory, readers.rTransactions);
        }

        {
            RawReaders readers = getRawReaders(); // A bit wasteful
            result.rTestClients = getClientData<History, Client>(nameTestClients, readers.rTestHistory, readers.rTransactions);
        }

        return result;
    }*/
};

}
