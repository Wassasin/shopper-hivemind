#include "classifier.h"
#include <QSharedPointer>
#include <QDebug>

namespace Hivemind
{


Classifier::Classifier()
    : model(nullptr), problem(nullptr), param(nullptr)
{
}

void Classifier::saveModel(QString filename)
{
    if(model == nullptr)
        qCritical() << "A request was made to save the model to a file, but the model was NULL.";
    else if(svm_save_model(filename.toLocal8Bit(), model) != 0)
        qCritical() << "Failed to save model to file.";
}

void Classifier::loadModel(QString filename)
{
    model = svm_load_model(filename.toLocal8Bit());
    if(model == nullptr)
        qFatal("Failed to load model from file.");
}

void Classifier::train(QVector<FeatureSet> trainData)
{
    problem = new svm_problem;
    problem->l = trainData.size();

    // Build list of prediction values and features, in libSVM-format
    QVector<double> prediction;
    prediction.reserve(trainData.size());
    for(FeatureSet s: trainData)
        prediction.append(s.getTargetValue());

    problem->y = prediction.data();
    buildSVMNodeArray(trainData);
    problem->x = svmX.data();

    param = new svm_parameter;
    param->svm_type = C_SVC;
    param->kernel_type = RBF;
    param->gamma = 1;
    param->degree = 1;
    param->cache_size = 1000; // cache_size is the size of the kernel cache, specified in megabytes, No idea
    param->C = 1; // C is the cost of constraints violation, No idea
    param->eps = 0.1; // eps is the stopping criterion. (we usually use 0.00001 in nu-SVC, 0.001 in others)
    //param->p   = 1; // p is the epsilon in epsilon-insensitive loss function of epsilon-SVM regression, no idea
    param->shrinking = 0; // shrinking = 1 means shrinking is conducted; = 0 otherwise
    param->probability = 0; // probability = 1 means model with probability information is obtained; = 0 otherwise
    param->nr_weight = 0; // No weight changes, data should be normalized beforehand
    const char *err = svm_check_parameter(problem, param);
    if(err != nullptr)
        qFatal(err);

    model = svm_train(problem, param);
//    double *output = new double[trainData.size()];
//    svm_cross_validation(problem, param, 10, output);
//    delete output;
    qDebug() << "Done training";
}

Probability Classifier::predict(FeatureSet testVector)
{
    if(model == nullptr)
        qFatal("Cannot predict with null model.");

    if(!svm_check_probability_model(model))
    {
        qCritical() << "Cannot give probability estimates for the current model.";
        return -1;
    }

    // Convert the testVector to svm_node's
    svm_node* nodeArray = new svm_node[testVector.getFeatureCount()+1];
    int i = 0;
    for(Feature f: testVector.getFeatures())
    {
        nodeArray[i].index = i;
        nodeArray[i].value = f;
        i++;
    }

    nodeArray[i].index = -1;

    // Predict probability
    double outcome[2];
    svm_predict_probability(model, nodeArray, outcome);
    return outcome[1];
}

void Classifier::buildSVMNodeArray(QVector<FeatureSet> trainData)
{
    svmX.reserve(trainData.size());
    for(FeatureSet s: trainData)
    {
        svm_node* nodeArray = new svm_node[s.getFeatureCount()+1];
        int i = 0;
        for(Feature f: s.getFeatures())
        {
            if(f == 0)
            {
                i++;
                continue;
            }

            nodeArray[i].value = f;
            nodeArray[i].index = i;
            i++;
        }

        nodeArray[i].index = -1;
        svmX.append(nodeArray);
    }
}

void Classifier::normalise(QVector<FeatureSet> &trainData)
{
    QVector<Feature> maxValues;
    for(FeatureSet s : trainData)
    {
        QVector<Feature> features = s.getFeatures();
        for(int i = 0; i < features.size(); i++)
            if(features.at(i) > maxValues.at(i))
                maxValues[i] = features.at(i);
    }

    for(int i = 0; i < maxValues.size(); i++)
        maxValues[i] = 1. / maxValues[i];

    for(FeatureSet &s : trainData)
    {
        QVector<Feature> &features = s.getFeaturesRef();
        for(int i = 0; i < features.size(); i++)
            features[i] *= maxValues[i];
    }
}

Classifier::~Classifier()
{
    delete param;
    svm_free_and_destroy_model(&model);
    qDeleteAll(svmX);
}

}
