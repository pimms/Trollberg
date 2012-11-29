#include "OLLeaderboard.h"


OLLeaderboard* OLLeaderboard::singleton = NULL;


OLLeaderboard::OLLeaderboard(void)
{
	singleton = this;
}
OLLeaderboard::~OLLeaderboard(void)
{
	singleton = NULL;
}

bool OLLeaderboard::connect()
{
	/*
	curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, "http://hemb.priv.no/tb/trollberg.php");

	curl_easy_perform(curl);

	std::cout<<curl <<"\n";
	*/
	return true;
}