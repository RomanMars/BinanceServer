
#include <map>
#include <vector>
#include <string>


#include "binacpp.h"
#include "binacpp_websocket.h"
#include <json/json.h>

#include <fstream> //redirect cout

using namespace std;

map < string, map <double,double> >  depthCache;
int lastUpdateId;

//------------------------------
void print_depthCache() {

	// map < string, map <double,double> >::iterator it_i;

	// for ( it_i = depthCache.begin() ; it_i != depthCache.end() ; it_i++ ) {
			
	// 	string bid_or_ask = (*it_i).first ;
	// 	cout << bid_or_ask << endl ;
	// 	cout << "Price             Qty" << endl ;		

	// 	map <double,double>::reverse_iterator it_j;

	// 	for ( it_j = depthCache[bid_or_ask].rbegin() ; it_j != depthCache[bid_or_ask].rend() ; it_j++ ) {

	// 		double price = (*it_j).first;
	// 		double qty   = (*it_j).second;
	// 		printf("%.08f          %.08f\n", price, qty );
	// 	}
	// }
}





//-------------
int ws_depth_onData( Json::Value &json_result ) {
	
	int i;

	int new_updateId  = json_result["u"].asInt();
	
	if ( new_updateId > lastUpdateId ) {
	
		for ( i = 0 ; i < json_result["b"].size() ; i++ ) {
			double price = atof( json_result["b"][i][0].asString().c_str());
			double qty 	 = atof( json_result["b"][i][1].asString().c_str());
			if ( qty == 0.0 ) {
				depthCache["bids"].erase(price);
			} else {
				depthCache["bids"][price] = qty;
			}
		}
		for ( i = 0 ; i < json_result["a"].size() ; i++ ) {
			double price = atof( json_result["a"][i][0].asString().c_str());
			double qty 	 = atof( json_result["a"][i][1].asString().c_str());
			if ( qty == 0.0 ) {
				depthCache["asks"].erase(price);
			} else {
				depthCache["asks"][price] = qty;
			}
		}		
		lastUpdateId = new_updateId;
	}
	print_depthCache();
}



//-----------------------------
int CustomCoutSrt_ws_depth_onData( Json::Value &json_result) {

    std::cout << json_result << std::endl;

}



//---------------------------
/*
	To compile, type
	make example_depthCache

*/

//--------------------------

int main() {

///
    std::ofstream out("out.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
///



	Json::Value result;
	long recvWindow = 10000;	

    std::cout << result << std::endl;


std::string api_key = "IprwuRqYzK7cB369c4vtZwPCPfcfgcuunPXUlFEW4H3Pm1WASBNKf6wKlMrQsh3c";
std::string secret_key = "yhzSPLdrvBmxrzM1GdUgh7R43Rp2Q3pyckPZ9ZqhEfiOYZChcNLNBJu6LECQuQHd";
BinaCPP::init( api_key , secret_key );	


/////
	// Market Depth 
	int i;
	string symbol = "BNBBTC";
	BinaCPP::get_depth( symbol.c_str(), 20, result ) ;


    std::cout << result << std::endl;

std::cout << "/* message */" << std::endl;
std::cout << "Depth" << std::endl;


/////



	// // Initialize the lastUpdateId
	// lastUpdateId = result["lastUpdateId"].asInt();
 		
	// for ( int i = 0 ; i < result["asks"].size(); i++ ) {

	// 	double price = atof( result["asks"][i][0].asString().c_str() );
	// 	double qty   = atof( result["asks"][i][1].asString().c_str() );
	// 	depthCache["asks"][price] = qty;
	// }
	// for  ( int i = 0 ; i < result["bids"].size() ; i++ ) {

	// 	double price = atof( result["bids"][i][0].asString().c_str() );
	// 	double qty   = atof( result["bids"][i][1].asString().c_str() );
	// 	depthCache["bids"][price] = qty;
	// }
	// print_depthCache();
 	



/////

	BinaCPP::get_bookTicker(symbol.c_str(), result );

	std::cout << result << std::endl;

std::cout << "/* message */" << std::endl;
std::cout << "Order book" << std::endl;

/////

	 ///sockets
std::cout << "/* message */" << std::endl;
std::cout << "Next sockets depth: " << std::endl;
	BinaCPP_websocket::init();
	Json::Value socketresult;
	// BinaCPP_websocket::connect_endpoint( CustomCoutSrt_ws_depth_onData ,"/ws/https://api.binance.com/api/v1/depth?symbol=BNBBTC&limit=3" ); 
	BinaCPP_websocket::connect_endpoint( CustomCoutSrt_ws_depth_onData ,"/ws/bnbbtc@depth@100ms" ); 
 	// BinaCPP_websocket::connect_endpoint( ws_depth_onData ,"/ws/bnbbtc@depth" ); 
 	BinaCPP_websocket::enter_event_loop(); 



///
	std::cout.rdbuf(coutbuf); //reset to standard output again
}




// 	string url(BINANCE_HOST);  
// 	url += "/api/v1/depth?";

// 	string querystring("symbol=");
// 	querystring.append( symbol );
// 	querystring.append("&limit=");
// 	querystring.append( to_string( limit ) );

// 	url.append( querystring );

// 	#define BINANCE_HOST "https://api.binance.com"
// // #define BINANCE_HOST "https://testnet.binance.vision/api"