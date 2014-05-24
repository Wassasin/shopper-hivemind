#ifndef OFFER_H
#define OFFER_H

struct Offer {

    OfferId offer;
    CategoryId category;
    int quantity;
    CompanayId company;
    int offerValue;
    BrandId brand;

    Offer(OfferId offer, CategoryId category, int quantity, CompanyId company, int offerValue, BrandId brand) {
        this->offer = offer;
        this->category = category;
        this->quantity = quantity;
        this->company = company;
        this->offerValue = offerValue;
        this->brand = brand;
    }

    Offer() {
        Q_UNREACHABLE();
    }
};

#endif // OFFER_H
