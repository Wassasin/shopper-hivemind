#include "featureextractor.h"

#include <QSet>
#include <QHash>
#include <QPair>
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

    FeatureSet FeatureExtractor::createFeatureSet(const Client &client) {
        QVector<Feature> features = extractFeatures(client);
        return FeatureSet(features);
    }

    FeatureSet FeatureExtractor::createFeatureSet(const TrainClient &client) {
        QVector<Feature> features = extractFeatures(client);
        return FeatureSet(features, client.repeater);
    }

    QVector<Feature> FeatureExtractor::extractFeatures(const Client &client) {
        Offer offer = offers.value(client.offer);
        Id offerDept = findOfferDepartment(client, offer);

        QVector<Feature> features;

//      features.append(offer.quantity); // Quantity is useless
        features.append(offer.offervalue);

        features += calcPurchaseRatios(client, offer, offerDept);

        features += countPurchasesAndReturns(client, offer, offerDept);
        
        features += countDiversities(client);

        return features;
    }

    /** Returns (offerDeparmentId) */
    int FeatureExtractor::findOfferDepartment(const Client &client, const Offer &offer) {
        foreach (Basket basket, client.baskets)
            foreach (Basketitem item, basket.items)
                if (item.brand == offer.brand && item.company == offer.company && item.category == offer.category)
                    return item.dept;
        return -1;
    }

    /** Returns the ratio ABCD amount : total amount, with ABCD being Brand/Category/Company/Department or a combination of those. */
    QVector<Feature> FeatureExtractor::calcPurchaseRatios(const Client &client, const Offer &offer, const Id &offerDept) {
        QVector<Feature> counts(12);

        foreach (Basket basket, client.baskets) {
            foreach (Basketitem item, basket.items) {
                if(item.purchaseamount <= 0 || item.purchasequantity <= 0)
                    continue;

                bool brand = item.brand == offer.brand;
                bool category = item.category == offer.category;
                bool company = item.company == offer.company;
                bool dept = item.dept == offerDept;

                counts[0] += item.purchaseamount * brand;
                counts[1] += item.purchaseamount * category;
                counts[2] += item.purchaseamount * company;
                counts[3] += item.purchaseamount * dept;
                counts[4] += item.purchaseamount * (brand && category);
                counts[5] += item.purchaseamount * (brand && company);
                counts[6] += item.purchaseamount * (brand && dept);
                counts[7] += item.purchaseamount * (brand && company && category);
                counts[8] += item.purchaseamount * (brand && company && dept);
                counts[9] += item.purchaseamount * (company && category);
                counts[10]+= item.purchaseamount * (company && dept);
                counts[11]+= item.purchaseamount;
            }
        }

        for (int i = 0; i < counts.size(); ++i)
            counts[i] /= counts.last();
        counts.resize(counts.size() - 1);

        return counts;
    }

     /**
      * Returns a vector of purchased/returned-ABCDCount, with ABCD being Brand/Category/Company/Department
      * or a combination of those. The number of purchase and return trips is also included.
      * All above is done for a number of days between the offer date and the item purchase date.
      */
    QVector<Feature> FeatureExtractor::countPurchasesAndReturns(const Client &client, const Offer &offer, const Id &offerDept) {
        const int DAY_COUNT = 8;
        const int FEATURE_COUNT = 12 * DAY_COUNT;

        const int days[DAY_COUNT] = { 1000000, 360, 180, 150, 120, 90, 60, 30 };

        QVector<Feature> counts(FEATURE_COUNT * 2); // Feature count is for purchase only, we need to double it to make room for returns

        foreach (Basket basket, client.baskets) {
            foreach (Basketitem item, basket.items) {
                if (signum(item.purchaseamount) != signum(item.purchasequantity))
                    continue; // Noise

                bool brand = item.brand == offer.brand;
                bool category = item.category == offer.category;
                bool company = item.company == offer.company;
                bool dept = item.dept == offerDept;

                int index = item.purchaseamount >= 0 ? 0 : FEATURE_COUNT;
                for (int i = 0; i < DAY_COUNT; ++i) {
                    if (basket.date.daysTo(client.offerDate) > days[i])
                        break;

                    counts[index++] += brand;
                    counts[index++] += category;
                    counts[index++] += company;
                    counts[index++] += dept;
                    counts[index++] += brand && category;
                    counts[index++] += brand && company;
                    counts[index++] += brand && dept;
                    counts[index++] += brand && company && category;
                    counts[index++] += brand && company && dept;
                    counts[index++] += company && category;
                    counts[index++] += company && dept;
                    counts[index++]++;
                }
            }
        }

        return counts;
    }
    
    /**
     * Counts how 'diverse' the client is. This method returns the number of different Xs per Y, with
     * X/Y being dept/brand/company/category. It does this for both purchases and returns (separately).
     */ 
    QVector<Feature> FeatureExtractor::countDiversities(const Client &client) {
        int FEATURE_COUNT = 11;
        QVector<QHash<Id, QSet<Id>>> thingsToThangs(FEATURE_COUNT * 2);
        
        foreach (Basket basket, client.baskets) {
            foreach (Basketitem item, basket.items) {
                if (signum(item.purchaseamount) != signum(item.purchasequantity))
                    continue; // Noise
                    
                QVector<QPair<Id, Id>> pairs(FEATURE_COUNT);
                pairs[0] = qMakePair(item.dept, item.brand);
                pairs[1] = qMakePair(item.dept, item.company);
                pairs[2] = qMakePair(item.dept, item.category);
                pairs[3] = qMakePair(item.brand, item.dept);
                pairs[4] = qMakePair(item.brand, item.company);
                pairs[5] = qMakePair(item.brand, item.category);
                pairs[6] = qMakePair(item.company, item.brand);
                pairs[7] = qMakePair(item.company, item.company);
                pairs[8] = qMakePair(item.company, item.category);
                pairs[9] = qMakePair(item.category, item.brand);
                pairs[10]= qMakePair(item.category, item.company);
                
                int index = item.purchaseamount >= 0 ? 0 : FEATURE_COUNT;
                foreach(QPair<Id, Id> pair, pairs) {
                    QHash<Id, QSet<Id>> hash = thingsToThangs[index];
                    QSet<Id> set = hash.value(pair.first);
                    set.insert(pair.second);
                    hash.insert(pair.first, set);
                }
            }
        }
        
        QVector<Feature> counts;
        counts.reserve(FEATURE_COUNT * 2);
        
        foreach(Id key, thingsToThangs.keys()) {
            counts.append(thingsToThangs.value(key).size());
        }

        return counts;
    }

    int FeatureExtractor::signum(float f) {
        if (f > 0) return +1;
        if (f < 0) return -1;
        return 0;
    }

    int FeatureExtractor::signum(uint64_t f) {
        if (f > 0) return +1;
        if (f < 0) return -1;
        return 0;
    }

}
