Binance Server Can connect to binance web api
and parse information 

BinanceServer.exe can run with default setups: 
"BTCUSDT" type of coins 
5 order depth size (options: 5,10,20,50)
10 num of connections // #define CONNECTIONS_Open_Multiple 1
100 latency server socket parameter (options: 500ms, 100ms or 250ms - if no prameter set)

or with parameters, example (cmd): 
BinanceServer.exe BNBBTC 10 3 100
"BNBBTC" type of coins
10 order depth size (options: 5,10,20,50)
3 num of connections // #define CONNECTIONS_Open_Multiple 1
100 latency server socket parameter (options: 500ms, 100ms or 250ms - if no prameter set)

post examples:
// btcusdt@depth@100ms //bnbbtc@depth@100ms

Whan server running. It save data to files:
out.json -> save data in json format(whith whitespaces)
out.txt -> format: info, updateid, bids, asks, json request from server and etc.
And store data in arrays:
binanceserver::ASKSv binanceserver::BIDSv Whole data bits asks data
binanceserver::asksv binanceserver::bidsv Data get on current update

for server-model communication -> use explanation in ServerModelDataTransferExplanation.txt

defines explanations:
#define SKIP_Empty_Quantity 1 skip asks, bids with quantity<minimum_quantity_value 
#define minimum_quantity_value 0.00000000

#define CONNECTIONS_Open_Multiple 1 // 0 -> 1 connection or 1 -> int numofconnections
// use if CONNECTIONS_Open_Multiple 1 can use CHECK_UpdateId 1 -> for check updates(use most recent)

#define CHECK_UpdateId 1 //if use many connections, or just check

#define USEWITHKEY 1 /// use with binance key // allways work with key now