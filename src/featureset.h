#ifndef FEATURESET_H
#define FEATURESET_H

#include <QVector>
#include <QSharedPointer>

#include "typedefs.h"

namespace Hivemind
{

class FeatureSet
{
public:
    FeatureSet(QVector<Feature> features, Prediction targetValue);
    FeatureSet(QVector<Feature> features);
    FeatureSet();
    QVector<Feature> getFeatures();
    QVector<Feature>& getFeaturesRef();
    int getFeatureCount() const;
    Prediction getTargetValue();
private:
    QVector<Feature> features;
    Prediction targetValue;
};

}
#endif // FEATURESET_H
