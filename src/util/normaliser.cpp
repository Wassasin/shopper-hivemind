#include "normaliser.h"

#include <QFile>
#include <QDataStream>
#include <QDebug>

namespace Hivemind
{

Normaliser::Normaliser(QVector<FeatureSet> trainData)
{
    QVector<bool> useValues;
    for(FeatureSet s : trainData)
    {
        QVector<Feature> features = s.getFeatures();
        if(!features.isEmpty())
        {
            maxValues.resize(features.size());
            useValues.resize(features.size());
            useValues.fill(false);
            break;
        }
    }

    for(FeatureSet s : trainData)
    {
        QVector<Feature> features = s.getFeatures();
        if(features.isEmpty())
            continue;

        for(int i = 0; i < features.size(); i++)
            if(features.at(i) > maxValues.at(i))
            {
                if(maxValues.at(i) != 0)
                    useValues[i] = true;
                maxValues[i] = features.at(i);
            }
    }

    for(int i = 0; i < maxValues.size(); i++)
    {
        maxValues[i] = 1. / maxValues.at(i);
        if(!useValues[i])
            maxValues[i] = 0;
    }
}

Normaliser::Normaliser(QString filename)
{
    // Now load the scaling vector
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        qFatal(QString("Failed to open file " + filename + ": loading of scaling vector failed.").toLocal8Bit());
        return;
    }

    QDataStream stream(&file);
    stream >> maxValues;
}

void Normaliser::saveModel(QString filename)
{
    // Now save the scaling vector
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qCritical() << "Failed to open file " + filename + ": saving of scaling vector failed.";
        return;
    }

    QDataStream stream(&file);
    stream << maxValues;
}

void Normaliser::normalise(FeatureSet &data)
{
    data.scaleFeatures(maxValues);
}

void Normaliser::normalise(QVector<FeatureSet> &data)
{
    for(FeatureSet &f : data)
        normalise(f);
}

}
