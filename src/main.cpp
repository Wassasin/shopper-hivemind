#include "util/convertor.h"
#include "util/msgpack_reader.h"

#include "offer.h"
#include "transaction.h"
#include "history.h"
#include "typedefs.h"

void test()
{
	using namespace hivemind;





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
	using namespace hivemind;

	const std::string datadir = "../data/";

	convertor::preprocess<offer>("offers", datadir);
	convertor::preprocess<train_history>("trainHistory", datadir);
	convertor::preprocess<history>("testHistory", datadir);
	convertor::preprocess<transaction>("transactions", datadir);

	convertor::create_dataset();

	//convertor::csv_to_msgpack<transaction>("../data/transactions.csv.gz", "../data/transactions.msgpack.gz");
	//test();
	return 0;
}

