#include "featureset.h"

namespace Hivemind
{

FeatureSet::FeatureSet(QVector<Feature> features, Probability targetValue)
    : features(features), targetValue(targetValue)
{
    features.squeeze();
}

QVector<Feature> FeatureSet::getFeatures()
{
    return features;
}

Probability FeatureSet::getTargetValue()
{
    return targetValue;
}

}
