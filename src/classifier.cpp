#include "classifier.h"
#include <QSharedPointer>
#include <QDebug>
#include <QFile>
#include <QDataStream>

namespace Hivemind
{


Classifier::Classifier()
    : model(nullptr), problem(nullptr), param(nullptr), predictionCount(0)
{
}

void Classifier::saveModel(QString filename)
{
    if(model == nullptr)
    {
        qCritical() << "A request was made to save the model to a file, but the model was NULL.";
        return;
    }
    else if(svm_save_model(filename.toLocal8Bit(), model) != 0)
    {
        qCritical() << "Failed to save model to file.";
        return;
    }
}

void Classifier::loadModel(QString filename)
{
    model = svm_load_model(filename.toLocal8Bit());
    if(model == nullptr)
    {
        qFatal("Failed to load model from file.");
        return;
    }
}

void Classifier::train(QVector<FeatureSet> trainData, bool cross_validate)
{
    if(!cross_validate)
        qDebug() << "Hi there. Let's train a model.";
    else
        qDebug() << "Hi there. Let's cross validate.";

    problem = new svm_problem;

    // Build list of prediction values and features, in libSVM-format
    QVector<double> prediction;
    prediction.reserve(trainData.size());
    for(FeatureSet s: trainData)
        prediction.append(s.getTargetValue());

    problem->y = prediction.data();
    buildSVMNodeArray(trainData);
    problem->l = prediction.size();
    problem->x = svmX.data();

    param = new svm_parameter;
    param->svm_type = C_SVC;
    param->kernel_type = LINEAR;
    param->gamma = 0.5; // Not used
    param->degree = 2; // Not used
    param->cache_size = 100; // cache_size is the size of the kernel cache, specified in megabytes, No idea
    param->C = 10; // C is the cost of constraints violation, No idea
    param->eps = 0.001; // eps is the stopping criterion. (we usually use 0.00001 in nu-SVC, 0.001 in others)
    //param->p   = 1; // p is the epsilon in epsilon-insensitive loss function of epsilon-SVM regression, no idea
    param->shrinking = 0; // shrinking = 1 means shrinking is conducted; = 0 otherwise
    param->probability = 0; // probability = 1 means model with probability information is obtained; = 0 otherwise
    param->nr_weight = 0; // No weight changes, data should be normalized beforehand
    const char *err = svm_check_parameter(problem, param);
    if(err != nullptr)
        qFatal(err);

    if(!cross_validate)
    {
        qDebug() << "Let the training commence.";
        model = svm_train(problem, param);
        qDebug() << "Done training.";
    }
    else
    {
        qDebug() << "Let's validate.";
        double *output = new double[trainData.size()];

        svm_cross_validation(problem, param, 10, output);
        int correctCount = 0;

        for(int j = 0; j < trainData.size(); j++)
        {
            if(output[j] == prediction.at(j))
                correctCount++;
            qDebug() << prediction.at(j) << output[j];
        }

        delete output;

        qDebug() << "Percentage rightly classified:" << (double) correctCount / trainData.size();
    }
}

void Classifier::crossValidate(QVector<FeatureSet> trainData)
{
    train(trainData, true);
}

Probability Classifier::predict(FeatureSet testVector)
{
    if(model == nullptr)
        qFatal("Cannot predict with null model.");

    if(predictionCount % 10000 == 0)
        qDebug() << "Predicting sample" << predictionCount;
    predictionCount++;

    // Convert the testVector to svm_node's
    svm_node* nodeArray = new svm_node[testVector.getFeatureCount()+1];
    int arrayIndex = 0;
    int nodeIndex = 0;

    for(Feature f: testVector.getFeatures())
    {
        nodeArray[arrayIndex].value = f;
        nodeArray[arrayIndex].index = nodeIndex++;
        arrayIndex++;
    }

    nodeArray[arrayIndex].index = -1;

    // Predict probability
    double decisionValue = 0; // 0 vs 1
    double classLabel = svm_predict_values(model, nodeArray, &decisionValue);
    return decisionValue;
}

void Classifier::buildSVMNodeArray(QVector<FeatureSet> trainData)
{
    for(FeatureSet s: trainData)
    {
        if(s.getFeatures().isEmpty())
            continue;

        svm_node* nodeArray = new svm_node[s.getFeatureCount()+1];
        int arrayIndex = 0;
        int nodeIndex = 0;

        for(Feature f: s.getFeatures())
        {
            if(f == 0)
            {
                nodeIndex++;
                continue;
            }

            nodeArray[arrayIndex].value = f;
            nodeArray[arrayIndex].index = nodeIndex++;
            arrayIndex++;
        }

        nodeArray[arrayIndex].index = -1;
        svmX.append(nodeArray);
    }
}

Classifier::~Classifier()
{
    delete param;
    svm_free_and_destroy_model(&model);
    qDeleteAll(svmX);
}

}
