#include "outputwriter.h"

namespace Hivemind
{

	const QString OutputWriter::fileHeader("id,repeatProbability\n");

	OutputWriter::OutputWriter(QString fileName)
	: file(fileName)
	, fileStream()
	{
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

}
