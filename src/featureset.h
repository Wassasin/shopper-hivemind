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
    int getFeatureCount() const;
    Prediction getTargetValue();
    void scaleFeatures(const QVector<Feature> vector);
private:
    QVector<Feature> features;
    Prediction targetValue;
};

}
#endif // FEATURESET_H
