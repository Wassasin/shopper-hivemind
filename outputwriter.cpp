#include "outputwriter.h"

const QString OutputWriter::fileHeader("id,repeatProbability\n");

OutputWriter::OutputWriter(QString fileName)
{
    file.setFileName(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        qFatal("Failed to open file.");
    fileStream.setDevice(&file);
    fileStream << fileHeader;
}

void OutputWriter::write(QVector<DataRow> data)
{
    QVectorIterator<DataRow> it(data);
    while(it.hasNext())
        fileStream << it.next();
}
