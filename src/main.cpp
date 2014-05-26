#include "util/convertor.h"

#include "offer.h"
#include "transaction.h"
#include "history.h"
#include "typedefs.h"

void test()
{
	using namespace Hivemind;

	MsgpackReader<TrainClient> r("../data/trainClients.msgpack.gz");
	TrainClient t;
	size_t result = 0;

	while(r.read(t))
		result += t.baskets.size();

	std::cerr << result << std::endl;
}

int main()
{
	using namespace Hivemind;

	const std::string datadir = "../data/";

	Convertor::preprocess<Offer>("offers", datadir);
	Convertor::preprocess<TrainHistory>("trainHistory", datadir);
	Convertor::preprocess<History>("testHistory", datadir);
	Convertor::preprocess<Transaction>("transactions", datadir);

	Convertor::createDataset();
	test();

	return 0;
}

