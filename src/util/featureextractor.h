#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <QList>
#include <QVector>
#include <QHash>

#include "typedefs.h"
#include "featureset.h"
#include "data/client.h"
#include "data/history.h"
#include "data/offer.h"
#include "data/transaction.h"
#include "util/reader.h"

namespace Hivemind
{

class FeatureExtractor
{
public:
    FeatureExtractor(Hivemind::Reader<Offer> &reader);


    FeatureSet createFeatureSet(const Client &client);
    FeatureSet createFeatureSet(const TrainClient &client);

private:
    QVector<Feature> extractFeatures(const Client &client);
    int findOfferDepartment(const Client &client, const Offer &offer);

    QVector<Feature> calcPurchaseRatios(const Client &client, const Offer &offer, const Id &offerDept);
    QVector<Feature> countPurchasesAndReturns(const Client &client, const Offer &offer, const Id &offerDept);

    int signum(float f);
    int signum(uint64_t f);

    QHash<Id, Offer> offers;

};

}
#endif // FEATUREEXTRACTOR_H
