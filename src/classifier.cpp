#include "classifier.h"
#include <QSharedPointer>
#include <QDebug>
#include <QFile>

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

    // Now save the scaling vector
    QFile file(filename + ".scale");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qCritical() << "Failed to open file " + filename + ": saving of scaling vector failed.";
        return;
    }

    QDataStream stream(&file);
    stream << maxValues;
}

void Classifier::loadModel(QString filename)
{
    model = svm_load_model(filename.toLocal8Bit());
    if(model == nullptr)
    {
        qFatal("Failed to load model from file.");
        return;
    }

    // Now load the scaling vector
    QFile file(filename + ".scale");
    if(!file.open(QIODevice::ReadOnly))
    {
        qFatal(QString("Failed to open file " + filename + ": loading of scaling vector failed.").toLocal8Bit());
        return;
    }

    QDataStream stream(&file);
    stream >> maxValues;
}

void Classifier::train(QVector<FeatureSet> trainData)
{
    qDebug() << "Hi there. Let's train a model.";
    problem = new svm_problem;

    // Normalise data
    qDebug() << "Calculating maximum values of features...";
    calculateMax(trainData);
    qDebug() << "Done.";

    // Build list of prediction values and features, in libSVM-format
    qDebug() << "Determining prediction labels...";
    QVector<double> prediction;
    prediction.reserve(trainData.size());
    for(FeatureSet s: trainData)
        prediction.append(s.getTargetValue());
    qDebug() << "Done.";

    problem->y = prediction.data();
    qDebug() << "Converting to libSVM-format while performing scaling...";
    buildSVMNodeArray(trainData);
    problem->l = svmX.size();
    qDebug() << "Done.";
    problem->x = svmX.data();

    param = new svm_parameter;
    param->svm_type = C_SVC;
    param->kernel_type = RBF;
    param->gamma = 0.5;
    param->degree = 2;
    param->cache_size = 1000; // cache_size is the size of the kernel cache, specified in megabytes, No idea
    param->C = 1; // C is the cost of constraints violation, No idea
    param->eps = 0.01; // eps is the stopping criterion. (we usually use 0.00001 in nu-SVC, 0.001 in others)
    //param->p   = 1; // p is the epsilon in epsilon-insensitive loss function of epsilon-SVM regression, no idea
    param->shrinking = 0; // shrinking = 1 means shrinking is conducted; = 0 otherwise
    param->probability = 0; // probability = 1 means model with probability information is obtained; = 0 otherwise
    param->nr_weight = 0; // No weight changes, data should be normalized beforehand
    const char *err = svm_check_parameter(problem, param);
    if(err != nullptr)
        qFatal(err);

    qDebug() << "Let the training commence.";
    model = svm_train(problem, param);
//    double *output = new double[trainData.size()];
//    svm_cross_validation(problem, param, 10, output);
//    delete output;
    qDebug() << "Done training.";
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
    int i = 0;
    for(Feature f: testVector.getFeatures())
    {
        nodeArray[i].index = i;
        nodeArray[i].value = f * maxValues.at(i);
        i++;
    }

    nodeArray[i].index = -1;

    // Predict probability
    double decisionValue = 0; // 0 vs 1
    double classLabel = svm_predict_values(model, nodeArray, &decisionValue);
//    if(classLabel == 0)
//        return 1 - decisionValue;
    return decisionValue;
}

int Classifier::buildSVMNodeArray(QVector<FeatureSet> trainData)
{
    svmX.reserve(trainData.size());
    for(FeatureSet s: trainData)
    {
        if(s.getFeatures().isEmpty())
            continue;

        svm_node* nodeArray = new svm_node[s.getFeatureCount()+1];
        int i = 0;
        for(Feature f: s.getFeatures())
        {
            if(f == 0)
            {
                i++;
                continue;
            }

            nodeArray[i].value = f * maxValues.at(i);
            nodeArray[i].index = i;
            i++;
        }

        nodeArray[i].index = -1;
        svmX.append(nodeArray);
    }
}

void Classifier::calculateMax(QVector<FeatureSet> trainData)
{
    for(FeatureSet s : trainData)
    {
        QVector<Feature> features = s.getFeatures();
        if(!features.isEmpty())
        {
            maxValues.resize(features.size());
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
                maxValues[i] = features.at(i);
    }

    for(int i = 0; i < maxValues.size(); i++)
        maxValues[i] = 1. / maxValues.at(i);
}

Classifier::~Classifier()
{
    delete param;
    svm_free_and_destroy_model(&model);
    qDeleteAll(svmX);
}

}
