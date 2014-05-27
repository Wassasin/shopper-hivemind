#include "datarow.h"

namespace Hivemind
{
	DataRow::DataRow()
	: id(), buyProbability() {}

	DataRow::DataRow(Id id, Probability buyProbability)
	: id(id)
	, buyProbability(buyProbability)
	{}

	Id DataRow::getId() const
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

}
