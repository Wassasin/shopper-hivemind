#ifndef OUTPUTWRITER_H
#define OUTPUTWRITER_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>

#include "datarow.h"

class OutputWriter
{
public:
    OutputWriter(QString fileName);
    /**
     * @brief Write the given data to the output file.
     * @param data
     */
    void write(QVector<DataRow> data);
private:
    QFile file;
    QTextStream fileStream;
    static const QString fileHeader;
};

#endif // OUTPUTWRITER_H
