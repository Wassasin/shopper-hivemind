#include "util/convertor.h"

#include "offer.h"
#include "transaction.h"
#include "history.h"
#include "typedefs.h"

int main()
{
	using namespace Hivemind;

	const std::string datadir = "../data/";

	Convertor::preprocess<Offer>("offers", datadir);
	Convertor::preprocess<TrainHistory>("trainHistory", datadir);
	Convertor::preprocess<History>("testHistory", datadir);
	Convertor::preprocess<Transaction>("transactions", datadir);

	Convertor::createDataset();

	return 0;
}

