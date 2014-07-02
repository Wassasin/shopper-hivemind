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

void FeatureSet::scaleFeatures(const QVector<Feature> vector)
{
    Q_ASSERT(vector.size() == features.size());
    int i = 0;
    QMutableVectorIterator<Feature> it(features);
    while(it.hasNext())
    {
        it.next();
        if(vector.at(i) == 0)
        {
            it.remove();
            continue;
        }

        Feature &f = it.value();
        f *= vector.at(i++);
        if(f<0) f = 0;
    }
}

}
