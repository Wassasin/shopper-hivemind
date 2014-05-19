#ifndef DATAROW_H
#define DATAROW_H

#include "constants.h"

#include <QTextStream>

class DataRow
{
public:
    DataRow(ShopperId id, Probability buyProbability = 0);
    ShopperId getId() const;
    Probability getBuyProbability() const;
    /**
     * @brief operator <<, used for generating output.
     * @param stream
     * @param row
     * @return
     */
    friend QTextStream &operator <<(QTextStream &stream, const DataRow &row);
private:
    ShopperId id;
    Probability buyProbability;

};

#endif // DATAROW_H
