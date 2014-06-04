#include "featureset.h"

namespace Hivemind
{

FeatureSet::FeatureSet(QVector<Feature> features, Prediction targetValue)
    : features(features), targetValue(targetValue)
{
    features.squeeze();
}

FeatureSet::FeatureSet(QVector<Feature> features)
    : features(features)
{
    features.squeeze();
}

FeatureSet::FeatureSet()
    : targetValue(0)
{}

QVector<Feature> FeatureSet::getFeatures()
{
    return features;
}

int FeatureSet::getFeatureCount() const
{
    return features.size();
}

Prediction FeatureSet::getTargetValue()
{
    return targetValue;
}

}
