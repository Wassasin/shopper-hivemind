#include "featureextractor.h"

#include <QMultiHash>
#include <QSet>
#include <QDebug>
#include <QDate>

#include "cache.h"
#include "reader.h"

namespace Hivemind
{

    FeatureExtractor::FeatureExtractor(Hivemind::Reader<Offer> &reader)
    {
        Offer tmp;
        while(reader.read(tmp))
            offers.insert(tmp.id, tmp);
    }

    FeatureSet FeatureExtractor::createFeatureSet(Client client) {
        QVector<Feature> features = extractFeatures(client);
        return FeatureSet(features);
    }

    FeatureSet FeatureExtractor::createFeatureSet(TrainClient client) {
        QVector<Feature> features = extractFeatures(client);
        return FeatureSet(features, client.repeater);
    }

    QVector<Feature> FeatureExtractor::extractFeatures(Client client) {
        QVector<Feature> features(11 /* feature count */);

        Offer offer = offers.value(client.offer);

        features.append(offer.brand); // The brand of the offer
        features.append(offer.offervalue); // The offer value

        Id offerDept = findOfferDepartment(client, offer);
        features.append(offerDept); // The deparment of the offer

        QPair<int, int> tripsInfo = countTrips(client);
        features.append(tripsInfo.first); // Number of visits to a chain *before* receiving the offer
        features.append(tripsInfo.second); // Number of visits to a chain *after* receiving the offer

        QPair<float, float> offerInfo = calcOfferRatio(client, offer);
        features.append(offerInfo.first); // The ratio offer : allItems for the number of unique items per basket, averaged over all baskets
        features.append(offerInfo.second); // The ratio offer : allItems for the total value of the items per basket, averaged over all baskets

        QPair<QPair<bool, bool>, int> purchaseInfo = calcPurchaseInfo(client, offer, offerDept);
        features.append(purchaseInfo.first.first); // Whether the client has bought the offer's product before receiving the offer
        features.append(purchaseInfo.first.second); // Whether the client has bought the offer's brand and department before receiving the offer
        features.append(purchaseInfo.second); // The number of days that elapsed before the client redeemed the offer (-1 if never)

        QPair<int, int> returnInfo = countReturns(client, offer);
        features.append(returnInfo.first); // The number of times the client has returned something
        features.append(returnInfo.second); // The number of times the client has return the offer's brand

        return features;
    }

    /** Returns (offerDeparmentId) */
    int FeatureExtractor::findOfferDepartment(Client client, Offer offer) {
        foreach (Basket basket, client.baskets)
            foreach (Basketitem item, basket.items)
                if (isOfferItem(item, offer))
                    return item.dept;
        return -1;
    }

    /** Returns (tripsBeforeOffer, tripsAfterOffer) */
    QPair<int, int> FeatureExtractor::countTrips(Client client) {
        QSet<Date> datesBefore;
        QSet<Date> datesAfter;
        foreach(Basket basket, client.baskets) {
            if (basket.date < client.offerDate)
                datesBefore.insert(basket.date);
            else
                datesAfter.insert(basket.date);
        }
        return qMakePair(datesBefore.size(), datesAfter.size());
    }

    /** Returns (offerItemRatio, offerValueRatio) */
    QPair<float, float> FeatureExtractor::calcOfferRatio(Client client, Offer offer) {
        float totalItemCount = 0;
        float totalItemValue = 0;
        float totalOfferCount = 0;
        float totalOfferValue = 0;

        foreach (Basket basket, client.baskets) {
            foreach (Basketitem item, basket.items) {
                totalItemCount += 1;
                totalItemValue += item.purchaseamount;

                if (isOfferItem(item, offer)) {
                    totalOfferCount += 1;
                    totalOfferValue += item.purchaseamount;
                }
            }
        }

        return qMakePair(totalOfferCount / totalItemCount, totalOfferValue / totalItemValue);
    }

    /** Returns ((boughtOfferBefore, boughtBrandBefore), delay) */
	QPair<QPair<bool, bool>, int> FeatureExtractor::calcPurchaseInfo(Client client, Offer offer, Id offerDept) {
        bool boughtOfferBefore = false;
        bool boughtBrandCategoryBefore = false;

        foreach (Basket basket, client.baskets) {
            foreach (Basketitem item, basket.items) {
                if (isOfferItem(item, offer)) {
                    if (basket.date >= client.offerDate)
						return qMakePair(qMakePair(boughtOfferBefore, boughtBrandCategoryBefore), (int)client.offerDate.daysTo(basket.date));
                    else
                        boughtOfferBefore = true;
                }
                if (item.brand == offer.brand && item.dept == offerDept)
                    boughtBrandCategoryBefore = true;
            }
        }
        return qMakePair(qMakePair(boughtOfferBefore, boughtBrandCategoryBefore), -1);
    }

    /** Returns (totalReturns, brandReturns) */
    QPair<int, int> FeatureExtractor::countReturns(Client client, Offer offer) {
        int returnTotalCount = 0;
        int returnBrandCount = 0;

        foreach (Basket basket, client.baskets) {
            foreach (Basketitem item, basket.items) {
                if (item.purchaseamount < 0) {
                    returnTotalCount++;
                    if (item.brand == offer.brand)
                        returnBrandCount++;
                }
            }
        }
        return qMakePair(returnTotalCount, returnBrandCount);
    }

    bool FeatureExtractor::isOfferItem(Basketitem item, Offer offer) {
        return item.brand == offer.brand && item.category == offer.category && item.company == offer.company;
    }


}
