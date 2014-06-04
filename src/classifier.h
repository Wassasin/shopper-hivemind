#ifndef REGRESSOR_H
#define REGRESSOR_H

#include "svm.h"
#include <QVector>

#include "featureset.h"

namespace Hivemind
{


class Classifier
{
public:
    Classifier();
    ~Classifier();
    void saveModel(QString filename);
    void loadModel(QString filename);
    void train(QVector<FeatureSet> trainData);
    Probability predict(FeatureSet testVector);
private:
    svm_model *model;
    svm_problem *problem;
    svm_parameter *param;
    QVector<svm_node*> svmX;
    void buildSVMNodeArray(QVector<FeatureSet> trainData);
    void normalise(QVector<FeatureSet> &trainData);
};

}
#endif // REGRESSOR_H
