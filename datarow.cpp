#include "datarow.h"

DataRow::DataRow(ShopperId id, Probability buyProbability)
{
    this->id = id;
    this->buyProbability = buyProbability;
}

ShopperId DataRow::getId() const
{
    return id;
}

Probability DataRow::getBuyProbability() const
{
    return buyProbability;
}

QTextStream &operator <<(QTextStream &stream, const DataRow &row)
{
    stream << QString::number(row.getId()) + "," + QString::number(row.getBuyProbability()) + "\n";
    return stream;
}
