#pragma once

#include <QTextStream>

#include "typedefs.h"

namespace Hivemind
{

	class DataRow
	{
	public:
		DataRow();
		DataRow(Id id, Probability buyProbability = 0);

		Id getId() const;
		Probability getBuyProbability() const;

		friend QTextStream &operator <<(QTextStream &stream, const DataRow &row);

	private:
		Id id;
		Probability buyProbability;
	};

}
