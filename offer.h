#ifndef OFFER_H
#define OFFER_H

struct Offer {

    OfferId offer;
    QString category;
    int quantity;
    CompanayId company;
    int value;
    BrandId brand;

    Offer(OfferId offer, QString category, int quantity, CompanyId company, int value, BrandId brand) {
        this->offer = offer;
        this->category = category;
        this->quantity = quantity;
        this->company = company;
        this->value = value;
        this->brand = brand;
    }

    Offer() {
        Q_UNREACHABLE();
    }
};

#endif // OFFER_H
