
#include <map>
#include <vector>
#include <string>

#include "binacpp.h"
#include "binacpp_websocket.h"
#include <json/json.h>
#include <vector>

#include <iostream>
#include <fstream> //redirect cout

#include <map>
#include <utility>

#include <stdint.h> //typedef int64_t, uint64_t

#include <cctype>
#include <algorithm>

#define SKIP_Empty_Quantity 1 //skip asks, bids with quantity<minimum_quantity_value 
#define minimum_quantity_value 0.00000000

#define CONNECTIONS_Open_Multiple 1 // 0 -> 1 connection or 1 -> int numofconnections
// use if CONNECTIONS_Open_Multiple 1 can use CHECK_UpdateId 1 -> for check updates(use most recent)
#define CHECK_UpdateId 1 //if use many connections, or just check

#define USEWITHKEY 1 /// use with binance key // allways work with key now

#define Large_Array_Size 99999999
#define Array_Size 999



namespace binanceserver
{

	uint64_t UpdateId = 1;
	uint64_t OrderlastUpdateId = 1;

	std::vector<double> ASKSv, BIDSv, asksv, bidsv;
	// binanceserver::ASKSv binanceserver::BIDSv Whole data bits asks data
	// binanceserver::asksv binanceserver::bidsv Data get on current update
	// std::pair<double, double> PQ; /// can use pair

	std::ofstream outfile("out.json", std::ofstream::binary);

	void ProcessSnapShot(Json::Value &result)
	{

		/////

		// int new_updateId  = json_result["u"].asInt(); // order -> "lastUpdateId": 1027024,
		OrderlastUpdateId = result["lastUpdateId"].asLargestUInt();

		std::cout << "[lastUpdateId] as uint_64t: " << OrderlastUpdateId << std::endl; //

		double p, q; /// price, quantity var

		/// Asks
		std::cout << "/* Asks from json */" << std::endl;
		if (result["asks"].size() > 0)
		{ // check if any
			for (int i = 0; i < result["asks"].size(); i++)
			{

				p = (atof(result["asks"][i][0].asString().c_str())); // Price
				q = (atof(result["asks"][i][1].asString().c_str())); // Quantity

				/// can use minimum double Quantity value = "0.00000001" "0.00000010"

#if SKIP_Empty_Quantity > 0

				if (q <= minimum_quantity_value)
				{

					std::cout << "SKIP_Empty_Quantity" << std::endl; //

					continue;
				}

#endif

				/// use vector emlplace_back? measure perfomance

				binanceserver::ASKSv.push_back(p); // Price level to be updated
				binanceserver::ASKSv.push_back(q); // Quantity

				/// local update arrays
				// asksv.push_back(atof(result["asks"][i][0].asString().c_str())); // Price level to be updated
				// asksv.push_back(atof(result["asks"][i][1].asString().c_str())); // Quantity

				std::cout << result["asks"][i][0] << std::endl; // Price level to be updated
				std::cout << result["asks"][i][1] << std::endl; // Quantity

				/// tests:

				// 			std::cout << "Json: " << std::endl;
				// std::cout << result["asks"][i][0] << std::endl; // Price level to be updated
				// std::cout << result["asks"][i][1] << std::endl; // Quantity

				// 			std::cout << "atof: " << std::endl;
				// std::cout << atof(result["asks"][i][0].asString().c_str()) << std::endl; //
				// std::cout << atof(result["asks"][i][1].asString().c_str()) << std::endl; //

				// 			std::cout << "stod: " << std::endl;
				// std::cout << std::stod(result["asks"][i][0].asString()) << std::endl; //
				// std::cout << std::stod(result["asks"][i][1].asString()) << std::endl; //

				// /// some time throw err, defoult asDouble() -> use static cast to double
				// // 			std::cout << "asDouble: " << std::endl;
				// // std::cout << result["asks"][i][0].asDouble() << std::endl; //
				// // std::cout << result["asks"][i][1].asDouble() << std::endl; //

				// std::cout << "/* message Aids from json [3] */" << std::endl; /// empty
				// std::cout << atof(result["asks"][i][3].asString().c_str()) << std::endl; // == 0 if

				///
			}
		}

		/// Bids
		std::cout << "/* Bids from json */" << std::endl;
		if (result["bids"].size() > 0)
		{ // check if any
			for (int i = 0; i < result["bids"].size(); i++)
			{

				p = (atof(result["bids"][i][0].asString().c_str())); // Price
				q = (atof(result["bids"][i][1].asString().c_str())); // Quantity

				/// can use minimum double Quantity value = "0.00000001" "0.00000010"

#if SKIP_Empty_Quantity > 0

				if (q <= minimum_quantity_value)
				{

					std::cout << "SKIP_Empty_Quantity" << std::endl; //

					continue;
				}

#endif

				/// use vector emlplace_back? measure perfomance

				binanceserver::BIDSv.push_back(p); // Price level to be updated
				binanceserver::BIDSv.push_back(q); // Quantity

				/// local update arrays
				// bidsv.push_back(atof(result["bids"][i][0].asString().c_str())); // Price level to be updated
				// bidsv.push_back(atof(result["bids"][i][1].asString().c_str())); // Quantity

				std::cout << result["bids"][i][0] << std::endl; // Price level to be updated
				std::cout << result["bids"][i][1] << std::endl; // Quantity

				///
			}
		}

		std::cout << result << std::endl;

		std::cout << "/* SnapShot  */" << std::endl;

		outfile << result; // have whitespaces

		//========================================================================================
		/*                                                                                      *
		 *                                  Send Data to Model.                                 *
		 *                       ASKSv BIDSv Whole data - > bids asks data                      *
		 *                    asksv bidsv bids asks -> Data on current update                   *
		 *                                                                                      */
		//========================================================================================
	}

	int CustomCoutSrt_ws_depth_onData(Json::Value &json_result)
	{

		// "u": 160,           // Final update ID in event as id

		//========================================================================================
		/*Use recents updates CHECK_UpdateId 1*/

#if CHECK_UpdateId > 0

		uint64_t new_updateId = json_result["u"].asLargestUInt();
		std::cout << "new_update id: " << new_updateId << std::endl; //
		std::cout << "Prev UpdateId id: " << UpdateId << std::endl;	 //

		if (new_updateId <= UpdateId)
		{ //<=?

			std::cout << "Skip update with id: " << new_updateId << std::endl; //

			return 1;
		}

		UpdateId = new_updateId;

#endif

		double p, q;

		/// Asks
		std::cout << "/* Asks from json */" << std::endl;
		if (json_result["a"].size() > 0)
		{ // check if any
			for (int i = 0; i < json_result["a"].size(); i++)
			{

				p = (atof(json_result["a"][i][0].asString().c_str()));
				q = (atof(json_result["a"][i][1].asString().c_str()));

				/// use minimum double Quantity value = "0.00000001" "0.00000010"

#if SKIP_Empty_Quantity > 0

				if (q <= minimum_quantity_value)
				{

					std::cout << "SKIP_Empty_Quantity" << std::endl; //

					continue;
				}

#endif

				/// use vector emlplace_back? measure perfomance

				binanceserver::ASKSv.push_back(p); // Price level to be updated
				binanceserver::ASKSv.push_back(q); // Quantity

				std::cout << json_result["a"][i][0] << std::endl; // Price level to be updated
				std::cout << json_result["a"][i][1] << std::endl; // Quantity

				/// local update arrays
				// bidsv.push_back(atof(result["a"][i][0].asString().c_str())); // Price level to be updated
				// bidsv.push_back(atof(result["a"][i][1].asString().c_str())); // Quantity

				/// tests:

				// 			std::cout << "Json: " << std::endl;
				// std::cout << result["asks"][i][0] << std::endl; // Price level to be updated
				// std::cout << result["asks"][i][1] << std::endl; // Quantity

				// 			std::cout << "atof: " << std::endl;
				// std::cout << atof(result["asks"][i][0].asString().c_str()) << std::endl; //
				// std::cout << atof(result["asks"][i][1].asString().c_str()) << std::endl; //

				// 			std::cout << "stod: " << std::endl;
				// std::cout << std::stod(result["asks"][i][0].asString()) << std::endl; //
				// std::cout << std::stod(result["asks"][i][1].asString()) << std::endl; //

				// /// some time throw err, defoult asDouble() -> use static cast to double
				// // 			std::cout << "asDouble: " << std::endl;
				// // std::cout << result["asks"][i][0].asDouble() << std::endl; //
				// // std::cout << result["asks"][i][1].asDouble() << std::endl; //

				// std::cout << "/* message Aids from json [3] */" << std::endl; /// empty
				// std::cout << atof(result["asks"][i][3].asString().c_str()) << std::endl; // == 0 if

				///
			}
		}

		/// Bids
		std::cout << "/* Bids from json */" << std::endl;
		if (json_result["b"].size() > 0)
		{ // check if any
			for (int i = 0; i < json_result["b"].size(); i++)
			{

				p = (atof(json_result["b"][i][0].asString().c_str()));
				q = (atof(json_result["b"][i][1].asString().c_str()));

				/// use minimum double Quantity value = "0.00000001" "0.00000010"

#if SKIP_Empty_Quantity > 0

				// q == 0?
				// if ((atof(result["bids"][i][1].asString().c_str())) == 0) { // -> pass 0: Price 1: Quantity
				if (q <= minimum_quantity_value)
				{ // -> pass 0: Price 1: Quantity

					// std::cout << q << std::endl; //
					std::cout << "SKIP_Empty_Quantity" << std::endl; //

					continue;
				}

#endif

				/// use vector emlplace_back? measure perfomance
				// BIDSv.emlplace_back(atof(result["bids"][i][0].asString().c_str()));

				binanceserver::BIDSv.push_back(p); // Price level to be updated
				binanceserver::BIDSv.push_back(q); // Quantity

				std::cout << json_result["b"][i][0] << std::endl; // Price level to be updated
				std::cout << json_result["b"][i][1] << std::endl; // Quantity

				/// local update arrays
				// bidsv.push_back(atof(result["a"][i][0].asString().c_str())); // Price level to be updated
				// bidsv.push_back(atof(result["a"][i][1].asString().c_str())); // Quantity

				///
			}
		}

		std::cout << json_result << std::endl;

		std::cout << "/* DepthUpdate */" << std::endl;

		outfile << json_result; // have whitespaces

		//========================================================================================
		/*                                                                                      *
		 *                                  Send Data to Model.                                 *
		 *                       ASKSv BIDSv Whole data - > bids asks data                      *
		 *                    asksv bidsv bids asks -> Data on current update                   *
		 *                                                                                      */
		//========================================================================================

		return UpdateId;
	}

}

int main(int argc, char **argv)
{

	/// Variables:

	std::string binancetype("BTCUSDT"); // type of coins
	int depthsize = 5;					// order depth size
	int numofconnections = 10;			// num of connections // #define CONNECTIONS_Open_Multiple 1
	int latency = 100;					// latency server parameter

	std::string posts; // post structure

	std::cout << "numofargc" << argc << std::endl;

	/// cout to text file
	std::cout.precision(9);
	double dtest = 3.14159265358979;
	std::cout << "dtest = 3.14159265358979 with cout.precision: " << dtest << std::endl;

	std::ofstream out("out.txt");
	std::streambuf *coutbuf = std::cout.rdbuf(); // save old buf
	std::cout.rdbuf(out.rdbuf());				 // redirect std::cout to out.txt!
	///

	/// argv

	std::vector<std::string> argstrv;

	if (argc > 1)
	{
		for (int i = 0; i < argc; ++i)
		{
			// std::cout << "argvnum" << i << argv[i] << std::endl;
			argstrv.push_back(argv[i]);
		}
		/// example: BinanceServer.exe BNBBTC 10 3 100
		binancetype = argstrv[1];						  // example BNBBTC
		depthsize = std::atoi(argstrv[2].c_str());		  // example 10 (variants: 5,10,20,50)
		numofconnections = std::atoi(argstrv[3].c_str()); // example 3
		latency = std::atoi(argstrv[4].c_str());		  // example 100 (variants: 100, 250, 500)ms
	}

	///
	binanceserver::ASKSv.reserve(Large_Array_Size);
	binanceserver::BIDSv.reserve(Large_Array_Size);

	/// arr for 1 update
	binanceserver::asksv.reserve(Array_Size); // remove?
	binanceserver::bidsv.reserve(Array_Size); // remove?

	std::string api_key = "IprwuRqYzK7cB369c4vtZwPCPfcfgcuunPXUlFEW4H3Pm1WASBNKf6wKlMrQsh3c";
	std::string secret_key = "yhzSPLdrvBmxrzM1GdUgh7R43Rp2Q3pyckPZ9ZqhEfiOYZChcNLNBJu6LECQuQHd";

#if USEWITHKEY > 0
	BinaCPP::init(api_key, secret_key); /// initwithkey
#endif
	BinaCPP::init(api_key, secret_key); /// initwithkey // work only with key now

	BinaCPP_websocket::init();

	Json::Value result;

	/// Market Depth

	BinaCPP::get_depth(binancetype.c_str(), depthsize, result); // BinaCPP::get_depth( symbol.c_str(), 5, result ) ;

	binanceserver::ProcessSnapShot(result);

	/////

	std::transform(binancetype.begin(), binancetype.end(), binancetype.begin(), [](unsigned char c)
				   { return std::tolower(c); });

	posts.append("/ws/");

	posts.append(binancetype);
	posts.append("@depth@");
	posts.append(to_string(latency));
	posts.append("ms");

	std::cout << "posts: " << posts << std::endl;
// examples: ///ws/btcusdt@depth@100ms ///ws/bnbbtc@depth@100ms
#if CONNECTIONS_Open_Multiple > 0

	for (size_t i = 0; i < numofconnections; i++)
	{
		/// std::vector<int> indexcv; indexcv.push_back(i) ///connection id
		BinaCPP_websocket::connect_endpoint(binanceserver::CustomCoutSrt_ws_depth_onData, posts.c_str()); // /ws/bnbbtc@depth@100ms
	}
	BinaCPP_websocket::enter_event_loop();
#endif
#if CONNECTIONS_Open_Multiple <= 0
	BinaCPP_websocket::connect_endpoint(binanceserver::CustomCoutSrt_ws_depth_onData, posts.c_str());

	BinaCPP_websocket::enter_event_loop();
#endif

	///
	std::cout.rdbuf(coutbuf); // reset to standard output again

	std::cin.get();
}

// BinaCPP_websocket::enter_event_loop()
// {
// 	while( 1 )
// 	{
// 		try {
// 			lws_service( context, 500 );

// 500 - is delay, better set to 0; (If no incoming events)

//--------------------------------------------------
// void
// BinaCPP::curl_api( string &url, string &result_json ) {
// 	vector <string> v;
// 	string action = "GET";
// 	string post_data = "";
// 	curl_api_with_header( url , result_json , v, post_data , action );
// }

// //--------------------
// // Do the curl
// void
// BinaCPP::curl_api_with_header( string &url, string &str_result, vector <string> &extra_http_header , string &post_data , string &action )
// {