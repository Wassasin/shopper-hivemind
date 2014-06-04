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


    FeatureSet createFeatureSet(Client client);
    FeatureSet createFeatureSet(TrainClient client);

private:
    QVector<Feature> extractFeatures(Client client);
    int findOfferDepartment(Client client, Offer offer);
    QPair<int, int> countTrips(Client client);
    QPair<float, float> calcOfferRatio(Client client, Offer offer);
	QPair<QPair<bool, bool>, int> calcPurchaseInfo(Client client, Offer offer, Id offerDept);
    QPair<int, int> countReturns(Client client, Offer offer);
    bool isOfferItem(Basketitem item, Offer offer);

    QHash<Id, Offer> offers;

};

}
#endif // FEATUREEXTRACTOR_H
