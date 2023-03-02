#include "Request.h"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>

using namespace std;
using json = nlohmann::json;

int main()
{
	auto response = cpr::Get(cpr::Url{ "https://rdb.altlinux.org/api/export/branch_binary_packages/p10" });
	json json_response = nlohmann::json::parse(response.text);
	
	get_request::Request request = json_response.get<get_request::Request>();

	return 0;
}
