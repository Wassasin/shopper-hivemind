#include "linearclassifier.h"

#include <algorithm>
#include <boost/lexical_cast.hpp>

#define TYPEDCALLOC(type,n) (type *)std::calloc(sizeof(type), (n))

namespace Hivemind
{

LinearClassifier::LinearClassifier()
    : m(nullptr)
{
    //
}

LinearClassifier::~LinearClassifier()
{
    if(m != nullptr)
        free_and_destroy_model(&m);
}

problem LinearClassifier::loadProblem(QVector<FeatureSet> trainData)
{
    problem prob;

    prob.y = TYPEDCALLOC(double, trainData.size());
    for(size_t i = 0; i < (size_t)trainData.size(); i++)
        prob.y[i] = trainData[i].getTargetValue();

    prob.x = TYPEDCALLOC(feature_node*, trainData.size());

    assert(trainData.size() > 0);
    prob.n = trainData[0].getFeatureCount();
    for(size_t i = 0; i < (size_t)trainData.size(); i++)
    {
        FeatureSet& s = trainData[i];

        // Last element must be "-1", thus add extra element
        QVector<Feature> features = s.getFeatures();
        feature_node* nodeArray = TYPEDCALLOC(feature_node, features.size()+1);

        assert(prob.n == features.size());
        for(size_t j = 0; j < (size_t)features.size(); j++)
            nodeArray[j] = {(int)j+1, features[j]};

        nodeArray[features.size()].index = -1;
        prob.x[i] = nodeArray;
    }

    prob.l = trainData.size();
    prob.bias = -1;

    return prob;
}

void LinearClassifier::train(QVector<FeatureSet> trainData)
{
    problem prob = loadProblem(trainData);

    parameter param;
    param.solver_type = L2R_LR;
    param.C = 0.001;
    param.eps = 0.0001;
    param.nr_weight = 0;
    param.weight_label = TYPEDCALLOC(int, 0);
    param.weight = TYPEDCALLOC(double, 0);

    const char *err = ::check_parameter(&prob, &param);
    if(err != nullptr)
        throw std::runtime_error(err);

    std::cerr << "Let the training commence." << std::endl;
    m = ::train(&prob, &param);

    std::free(prob.y);
    for(size_t i = 0; i < (size_t)trainData.size(); i++)
        std::free(prob.x[i]);
    std::free(prob.x);

    std::cerr << "Done training." << std::endl;
}

Probability LinearClassifier::predict(FeatureSet testVector)
{
    if(m == nullptr)
        throw std::runtime_error("First train on a dataset or load an existing model, before predicting");

    feature_node* x = TYPEDCALLOC(feature_node, testVector.getFeatureCount()+1);
    QVector<Feature> features = testVector.getFeatures();
    for(size_t i = 0; i < (size_t)features.size(); i++)
        x[i] = {(int)i+1, features[i]};

    x[features.size()].index = -1;

    double decisionValue = 0;
    predict_probability(m, x, &decisionValue);

    std::free(x);

    return decisionValue;
}

}
