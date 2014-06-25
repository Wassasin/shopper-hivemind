#ifndef MEASURES_H
#define MEASURES_H

#include <QVector>
//#include <qmath.h>

#include "typedefs.h"

namespace Hivemind
{

class Measures
{
public:
    Measures(QVector<Feature> featureCol);
    float calculateVariance();
    float calculateDeviation();
    float getMean();
    Feature getMin();
    Feature getMax();
private:
    float calculateMean();
    QVector<Feature> featureCol;
    float mean;
};

}

#endif // MEASURES_H
