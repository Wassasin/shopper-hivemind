#include "measures.h"

#include<cfloat>
#include <QVector>

namespace Hivemind
{

Measures::Measures(QVector<Feature> featureCol):featureCol(featureCol),mean(0.0)
{
    this->mean = calculateMean();
}

float Measures::calculateDeviation()
{
    return sqrt(calculateVariance());
}

float Measures::calculateVariance()
{
    double sumDiff = 0.0;
    double mean = calculateMean();
    for (Feature f : featureCol)
        sumDiff += pow(f - mean, 2);
	return sumDiff / featureCol.size();
}

float Measures::getMean()
{
    return mean;
}

Feature Measures::getMin()
{
    Feature min = FLT_MAX;
    for(Feature f : featureCol)
    {
        if(f < min)
            min = f;
    }
    return min;

}

Feature Measures::getMax()
{
    Feature max = FLT_MIN;
    for(Feature f : featureCol)
    {
        if(f > max)
            max = f;
    }
    return max;
}

float Measures::calculateMean()
{
    if (featureCol.isEmpty())
        return 0.0;
    double sum = 0.0;
    for (Feature f : featureCol)
        sum += f;
	return sum / featureCol.size();
}

}
