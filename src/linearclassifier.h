#pragma once

#include <linear.h>

#include "featureset.h"

namespace Hivemind
{

class LinearClassifier
{
private:
    model* m;
    double* normalisationFactors;

    LinearClassifier(LinearClassifier&) = delete;
    LinearClassifier& operator=(LinearClassifier&) = delete;

    void loadNormalisationFactors(QVector<FeatureSet> trainData);
    problem loadProblem(QVector<FeatureSet> trainData);

public:
    LinearClassifier();
    ~LinearClassifier();

    void train(QVector<FeatureSet> trainData);
    Probability predict(FeatureSet testVector);
};

}
