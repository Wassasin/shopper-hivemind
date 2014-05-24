#ifndef HISTORY_H
#define HISTORY_H

#include <QDate>

struct History {

    ShopperId id;
    ChainId chain;
    OfferId offer;
    MarketId market;
    int repeatTrips;
    bool repeater;
    QDate offerDate;

    History(ShopperId id, ChainId chain, OfferId offer, MarketId market, int repeatTrips, bool repeater, QDate offerDate) {
        this->id = id;
        this->chain = chain;
        this->offer = offer;
        this->market = market;
        this->repeatTrips = repeatTrips;
        this->repeater = repeater;
        this->offerDate = offerDate;
    }

    History() {
        Q_UNREACHABLE();
    }
};

#endif // HISTORY_H
