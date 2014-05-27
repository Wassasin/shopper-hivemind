#include "cli.h"
#include "regressor.h"
#include "featureset.h"
#include <QDebug>

int main(int argc, char** argv)
{
    return Hivemind::Cli::run(argc, argv);
//    Regression testing! Ha!
//    Hivemind::Regressor r;
//    QVector<Hivemind::FeatureSet> vector;
//    vector.append(Hivemind::FeatureSet(QVector<Hivemind::Feature>({0.5, 1.0, 0.0}), true));
//    vector.append(Hivemind::FeatureSet(QVector<Hivemind::Feature>({1.0, 1.0, 0.0}), false));
//    vector.append(Hivemind::FeatureSet(QVector<Hivemind::Feature>({0.0, 1.0, 0.0}), false));
//    vector.append(Hivemind::FeatureSet(QVector<Hivemind::Feature>({1.0, 1.0, 1.0}), false));
//    vector.append(Hivemind::FeatureSet(QVector<Hivemind::Feature>({0.0, 1.0, 0.0}), false));
//    vector.append(Hivemind::FeatureSet(QVector<Hivemind::Feature>({1.0, 0.0, 1.0}), false));
//    vector.append(Hivemind::FeatureSet(QVector<Hivemind::Feature>({0.0, 0.0, 0.0}), false));
//    r.train(vector);
//    qDebug() << r.predict(Hivemind::FeatureSet(QVector<Hivemind::Feature>({0.5, 0.0, 0.0})));
//    qDebug() << r.predict(Hivemind::FeatureSet(QVector<Hivemind::Feature>({1.0, 0.0, 0.0})));
//    qDebug() << r.predict(Hivemind::FeatureSet(QVector<Hivemind::Feature>({0.0, 0.0, 0.0})));
//    return 0;
}
