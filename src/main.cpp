#include "util/convertor.h"
#include "util/msgpack_reader.h"

#include "offer.h"
#include "transaction.h"
#include "history.h"
#include "typedefs.h"

void test()
{
	using namespace Hivemind;





	/*transaction t;
	int result = 0, i = 0;
	msgpack_reader<transaction> r("../data/transactions.msgpack.gz");
	while(r.read(t))
	{
		result += t.id;

		i++;
		if(i == 500000)
			break;
	}

	std::cout << i << std::endl;
	std::cout << result << std::endl;*/
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

	//convertor::csv_to_msgpack<transaction>("../data/transactions.csv.gz", "../data/transactions.msgpack.gz");
	//test();
	return 0;
}

