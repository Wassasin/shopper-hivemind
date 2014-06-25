#ifndef NORMALISER_H
#define NORMALISER_H

#include <QVector>

#include "featureset.h"
#include "typedefs.h"

namespace Hivemind
{

class Normaliser
{
public:
    Normaliser(QVector<FeatureSet> trainData);
    Normaliser(QString filename);
    void saveModel(QString filename);
    void normalise(FeatureSet &data);
    void normalise(QVector<FeatureSet> &data);
private:
    QVector<Feature> maxValues;
};

}

#endif // NORMALISER_H
