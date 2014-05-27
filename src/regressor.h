#ifndef REGRESSOR_H
#define REGRESSOR_H

#include "svm.h"
#include <QVector>

#include "featureset.h"

namespace Hivemind
{


class Regressor
{
public:
    Regressor();
    void train(QVector<FeatureSet> trainData);
};

}
#endif // REGRESSOR_H
