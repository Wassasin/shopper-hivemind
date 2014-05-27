#ifndef FEATURESET_H
#define FEATURESET_H

#include <QVector>

#include "typedefs.h"

namespace Hivemind
{

class FeatureSet
{
public:
    FeatureSet(QVector<Feature> features, Probability targetValue);
    QVector<Feature> getFeatures();
    Probability getTargetValue();
private:
    QVector<Feature> features;
    Probability targetValue;
};

}
#endif // FEATURESET_H
