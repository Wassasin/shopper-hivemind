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
    QPair<int, int> countTrips(const Client &client);
    QPair<float, float> calcOfferRatio(const Client &client, const Offer &offer);
    QPair<QPair<bool, bool>, int> calcPurchaseInfo(const Client &client, const Offer &offer, const Id &offerDept);
    QPair<int, int> countReturns(const Client &client, const Offer &offer);
    bool isOfferItem(const Basketitem &item, const Offer &offer);

    QHash<Id, Offer> offers;

};

}
#endif // FEATUREEXTRACTOR_H
