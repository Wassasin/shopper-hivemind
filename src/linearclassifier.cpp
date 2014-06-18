#include "linearclassifier.h"

#include <algorithm>
#include <boost/lexical_cast.hpp>

namespace Hivemind
{

LinearClassifier::LinearClassifier()
    : m(nullptr)
    , normalisationFactors(nullptr)
{
    //
}

LinearClassifier::~LinearClassifier()
{
    if(m != nullptr)
    {
        free_model_content(m);
        free_and_destroy_model(&m);
    }

    if(normalisationFactors != nullptr)
        delete[] normalisationFactors;
}

void LinearClassifier::loadNormalisationFactors(QVector<FeatureSet> trainData)
{
    assert(trainData.size() > 0);
    const size_t dimensions = trainData[0].getFeatureCount();

    normalisationFactors = new double[dimensions];

    for(size_t i = 0; i < dimensions; i++)
        normalisationFactors[i] = -1.0;

    for(size_t i = 0; i < (size_t)trainData.size(); i++)
    {
        FeatureSet& s = trainData[i];
        if(s.getFeatures().isEmpty())
            continue;

        QVector<Feature> features = s.getFeatures();
        for(size_t j = 0; j < dimensions; j++)
        {
            assert(features[j] >= 0.0);
            normalisationFactors[j] = std::max<double>(features[j], normalisationFactors[j]);
        }
    }

    for(size_t i = 0; i < dimensions; i++)
        if(normalisationFactors[i] <= 0.0)
            throw std::runtime_error(std::string("Column ") + boost::lexical_cast<std::string>(i) + " does not contain any useful information (always 0.0)");
}

problem LinearClassifier::loadProblem(QVector<FeatureSet> trainData)
{
    problem prob;

    loadNormalisationFactors(trainData);

    prob.y = new int[trainData.size()];
    for(size_t i = 0; i < (size_t)trainData.size(); i++)
        prob.y[i] = trainData[i].getTargetValue();

    prob.x = new feature_node*[trainData.size()];

    assert(trainData.size() > 0);
    prob.n = trainData[0].getFeatureCount();
    for(size_t i = 0; i < (size_t)trainData.size(); i++)
    {
        FeatureSet& s = trainData[i];
        if(s.getFeatures().isEmpty())
            continue;

        // Last element must be "-1", thus add extra element
        QVector<Feature> features = s.getFeatures();
        feature_node* nodeArray = new feature_node[features.size()+1];

        assert(prob.n == features.size());
        for(size_t j = 0; j < (size_t)features.size(); j++)
            nodeArray[j] = {(int)j, features[j] / normalisationFactors[j]};

        nodeArray[features.size()].index = -1;
        prob.x[i] = nodeArray;
    }

    prob.l = trainData.size();

    return prob;
}

void LinearClassifier::train(QVector<FeatureSet> trainData)
{
    problem prob = loadProblem(trainData);

    parameter param;
    param.solver_type = L2R_LR;
    param.C = 1;
    param.eps = 9001; // INF
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;

    const char *err = ::check_parameter(&prob, &param);
    if(err != nullptr)
        throw std::runtime_error(err);

    std::cerr << "Let the training commence." << std::endl;
    m = ::train(&prob, &param);

    delete[] prob.y;
    for(size_t i = 0; i < (size_t)trainData.size(); i++)
        delete[] prob.x[i];
    delete[] prob.x;

    std::cerr << "Done training." << std::endl;
}

Probability LinearClassifier::predict(FeatureSet testVector)
{
    if(m == nullptr)
        throw std::runtime_error("First train on a dataset or load an existing model, before predicting");

    assert(normalisationFactors != nullptr);

    feature_node* x = new feature_node[testVector.getFeatureCount()+1];
    QVector<Feature> features = testVector.getFeatures();
    for(size_t i = 0; i < (size_t)features.size(); i++)
        x[i] = {(int)i, features[i] / normalisationFactors[i]};

    x[testVector.getFeatureCount()].index = -1;

    double decisionValue = 0;
    predict_probability(m, x, &decisionValue);

    delete[] x;

    return decisionValue;
}

}
