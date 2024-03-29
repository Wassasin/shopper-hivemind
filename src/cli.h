#pragma once

#include <memory>
#include <boost/program_options.hpp>

#include <QDebug>

#include "typedefs.h"

#include "classifier.h"
#include "util/normaliser.h"
#include "util/measures.h"
#include "linearclassifier.h"

#include "outputwriter.h"

#include "dataset.h"

#include "util/featureextractor.h"

namespace Hivemind
{
    class Cli
    {
        Cli() = delete;
        Cli(Cli&) = delete;
        Cli& operator=(Cli&) = delete;

        enum class ClassifierType
        {
            SVM,
            LINEAR
        };

        struct Options
        {
            bool generate_cache = false;
            bool clear_cache = false;
            bool force = false;
            bool debug = false;
            bool train = false;
            std::string datadir = "./data";
            ClassifierType classifier = ClassifierType::SVM;
        };

        static bool askConfirmation()
        {
            std::string response;

            while(true)
            {
                std::getline(std::cin, response);

                if(response == "y")
                    return true;
                else if(response == "n")
                    return false;

                std::cerr << "Invalid answer, please respond with either \"y\" or \"n\": ";
            }
        }

        static int interpret(Options& opt, int argc, char** argv)
        {
            std::string classifierString = "";

            boost::program_options::options_description o_general("General options");
            o_general.add_options()
            ("help,h", "display this message")
            ("cache,c", "generate cache files,  if they do not exist yet")
            ("clear-cache,p", "clears cache files (will ask for confirmation)")
            ("debug,t", "use small debug subset, for code testing purposes")
            ("force,f", "will never ask confirmation, answer [YES] to everything")
            ("datadir,d", boost::program_options::value<decltype(opt.datadir)>(&opt.datadir), "specify in which directory to search for the data files (default ./data)")
            ("algorithm,a", boost::program_options::value(&classifierString), "{svm, linear} (defaults to svm)");

            boost::program_options::variables_map vm;
            boost::program_options::positional_options_description pos;

            boost::program_options::options_description options("Allowed options");
            options.add(o_general);

            try
            {
                boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(options).positional(pos).run(), vm);
            } catch(boost::program_options::unknown_option &e)
            {
                std::cerr << "Unknown option --" << e.get_option_name() << ", see --help" << std::endl;
                return -1;
            }

            try
            {
                boost::program_options::notify(vm);
            } catch(const boost::program_options::required_option &e)
            {
                std::cerr << "You forgot this: " << e.what() << std::endl;
                return 1;
            }

            if(vm.count("cache"))
                opt.generate_cache = true;

            if(vm.count("clear-cache"))
                opt.clear_cache = true;

            if(vm.count("debug"))
                opt.debug = true;

            if(vm.count("force"))
                opt.force = true;

            if(vm.count("train"))
                opt.train = true;

            if(vm.count("help"))
            {
                std::cout
                    << "Machine Learning Engine for the Acquire Valued Shoppers Challenge. [https://github.com/Wassasin/shopper-hivemind]" << std::endl
                    << "Usage: ./shopper-hivemind [options]" << std::endl
                    << std::endl
                    << o_general;

                return 1;
            }

            if(classifierString == "svm" || classifierString == "")
                opt.classifier = ClassifierType::SVM;
            else if(classifierString == "linear")
                opt.classifier = ClassifierType::LINEAR;
            else
            {
                std::cerr << "Unrecognized classifier \"" << classifierString << "\"" << std::endl;
                return 1;
            }

            return 0;
        }

        static void useLinearClassifier(const Options& opt, Dataset& d)
        {
            auto readers = d.getRealReaders();

            {
                LinearClassifier c;
                size_t result = 0;
                {
                    auto rOffer(Cache::getFastestReader<Offer>("offers", opt.datadir));
                    QVector<FeatureSet> trainData;
                    FeatureExtractor f(*rOffer);

                    TrainClient trainClient;
                    while(readers.rTrainClients->read(trainClient))
                    {
                        auto features = f.createFeatureSet(trainClient);
                        if(features.getFeatureCount() != 0)
                            trainData.append(features);
                    }

                    std::cerr << "Built trainData" << std::endl;

                    Normaliser n(trainData);
                    n.normalise(trainData);
                    n.saveModel("model.norm");

                    c.train(trainData);

                    std::cerr << "Trained" << std::endl;
                }

                {
                    auto rOffer(Cache::getFastestReader<Offer>("offers", opt.datadir));
                    FeatureExtractor f(*rOffer);

                    Normaliser n("model.norm");

                    QVector<DataRow> output;

                    Client testClient;
                    while(readers.rTestClients->read(testClient))
                    {
                        FeatureSet feat = f.createFeatureSet(testClient);
                        n.normalise(feat);
                        Probability p = c.predict(feat);
                        output.append(DataRow(testClient.id, p));
                    }

                    OutputWriter writer("output.csv");
                    writer.write(output);

                }

                std::cerr << result << std::endl;
            }
        }

        static void useSVMClassifier(const Options& opt, Dataset& d)
        {
            auto readers = d.getRealReaders();

            {
                size_t result = 0;
                {
                    auto rOffer(Cache::getFastestReader<Offer>("offers", opt.datadir));
                    QVector<FeatureSet> trainData;
                    FeatureExtractor f(*rOffer);

                    TrainClient trainClient;
                    while(readers.rTrainClients->read(trainClient))
                    {
                        auto features = f.createFeatureSet(trainClient);
                        if(features.getFeatureCount() != 0)
                            trainData.append(features);
                    }

                    std::cerr << "Built trainData" << std::endl;

                    int colCount = 0;
                    QVector<QVector<Feature>> dataCols(trainData.first().getFeatureCount());
                    for (FeatureSet fs : trainData) {
                        int i = 0;
                        for (Feature f : fs.getFeatures())
                            dataCols[i++].append(f);
                    }
                    for (QVector<Feature> v : dataCols) {
                        Measures m(v);
                        qDebug() << "---\nRow " << (colCount++);
                        qDebug() << "Mean: " << m.getMean();
                        qDebug() << "Min : " << m.getMin();
                        qDebug() << "Max : " << m.getMax();
                        qDebug() << "Var : " << m.calculateVariance();
                        qDebug() << "Dev : " << m.calculateDeviation();
                    }

                    Classifier c;
                    Normaliser n(trainData);
                    n.normalise(trainData);
                    n.saveModel("model.norm");

                    c.train(trainData);

                    std::cerr << "Trained" << std::endl;
                    c.saveModel("model.data");
                }

                {
                    auto rOffer(Cache::getFastestReader<Offer>("offers", opt.datadir));
                    FeatureExtractor f(*rOffer);

                    Classifier c;
                    Normaliser n("model.norm");
                    c.loadModel("model.data");

                    QVector<DataRow> output;

                    Client testClient;
                    while(readers.rTestClients->read(testClient))
                    {
                        FeatureSet feat = f.createFeatureSet(testClient);
                        n.normalise(feat);
                        Probability p = c.predict(feat);
                        output.append(DataRow(testClient.id, p));
                    }

                    OutputWriter writer("output.csv");
                    writer.write(output);

                }

                std::cerr << result << std::endl;
            }
        }

        static int act(const Options& opt)
        {
            Cache::checkDatadir(opt.datadir);

            if(opt.clear_cache)
            {
                if(!opt.force)
                {
                    std::cerr << "You are about to clear a very costly cache, continue? (y/n) ";
                    if(!askConfirmation())
                        return 1;
                }

                Dataset::clearCache(opt.datadir);
            }

            Dataset d(opt.datadir, opt.generate_cache, opt.debug);

            switch(opt.classifier)
            {
            case ClassifierType::SVM:
                useSVMClassifier(opt, d);
                break;
            case ClassifierType::LINEAR:
                useLinearClassifier(opt, d);
                break;
            default:
                throw std::logic_error("Unknown ClassifierType");
            }

            return 0;
        }

    public:
        static int run(int argc, char** argv)
        {
            Options opt;
            int result = interpret(opt, argc, argv);
            if(result != 0)
                return result;
            return act(opt);
        }
    };
}
