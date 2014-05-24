#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QDate>

struct Transaction {

    ShopperId id;
    ChainId chain;
    DeptId dept;
    CategoryId category;
    CompanyId company;
    BrandId brand;
    QDate date;
    int productSize;
    QString productMeasure;
    int purchaseQuantity;
    int purchaseAmount;

    Transaction(ShopperId id, ChainId chain, DeptId dept, CategoryId category, CompanyId company, BrandId brand, QDate date, int productSize, QString productMeasure, int purchaseQuantity, int purchaseAmount) {
        this->id = id;
        this->chain = chain;
        this->dept = dept;
        this->category = category;
        this->company = company;
        this->brand = brand;
        this->date = date;
        this->productSize = productSize;
        this->productMeasure = productMeasure;
        this->purchaseQuantity = purchaseQuantity;
        this->purchaseAmount = purchaseAmount;
    }

    Transaction() {
        Q_UNREACHABLE();
    }
};

#endif // TRANSACTION_H
