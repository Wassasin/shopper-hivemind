#pragma once

#include <linear.h>

#include "featureset.h"

namespace Hivemind
{

class LinearClassifier
{
public:
    LinearClassifier();

    void train(QVector<FeatureSet> trainData);
    Probability predict(FeatureSet testVector);
};

}
