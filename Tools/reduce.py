import sys
from datetime import datetime


def reduce_data(loc_offers, loc_transactions, loc_reduced):
    start = datetime.now()
    #get all categories on offer in a dict
    offers = {}
    for e, line in enumerate( open(loc_offers) ):
        offers[ line.split(",")[1] ] = 1
    #open output file
    with open(loc_reduced, "wb") as outfile:
        #go through transactions file and reduce
        reduced = 0
        for e, line in enumerate( open(loc_transactions) ):
            if e == 0:
                outfile.write( line ) #print header
            else:
                #only write when category in offers dict
                if line.split(",")[3] in offers:
                    outfile.write( line )
                    reduced += 1
            #progress
            if e % 5000000 == 0:
                print e, reduced, datetime.now() - start
    print e, reduced, datetime.now() - start

if (len(sys.argv) != 4):
    print 'Expected 3 arguments: <offersFile> <transactionsFile> <reducedTransactionsFile>'
    sys.exit()
    
loc_offers = sys.argv[1]
loc_transactions = sys.argv[2]
loc_reduced = sys.argv[3]

reduce_data(loc_offers, loc_transactions, loc_reduced)
