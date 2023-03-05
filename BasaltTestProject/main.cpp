#include "Request.h"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <thread>
#include <unordered_set>

using namespace std;
using json = nlohmann::json;

unordered_map<string, vector<get_request::Package>> find_missing_elements(vector<get_request::Package> first_packages, vector<get_request::Package> second_packages)
{
	unordered_map<string, vector<get_request::Package>> missing_elements;
	unordered_set<get_request::Package, get_request::Package::PackageHash> elements;
	for (size_t i = 0; i < first_packages.size(); ++i)
	{
		elements.insert(first_packages[i]);
	}

	for (size_t i = 0; i < second_packages.size(); ++i)
	{
		if (elements.find(second_packages[i]) == elements.end())
		{
			missing_elements[second_packages[i].arch].push_back(second_packages[i]);
		}
	}

	return missing_elements;
}



int main()
{
	cpr::Response first_response = cpr::Get(cpr::Url{ "https://rdb.altlinux.org/api/export/branch_binary_packages/p10" });
	cpr::Response second_responce = cpr::Get(cpr::Url{ "https://rdb.altlinux.org/api/export/branch_binary_packages/sisyphus" });
	
	cpr::Response branches_responce = cpr::Get(cpr::Url{ "https://rdb.altlinux.org/api/task/progress/all_tasks_branches" });
	json branches_json_responce = json::parse(branches_responce.text);
	get_request::BranchRequest branch_request = branches_json_responce.get<get_request::BranchRequest>();

	vector<string> archs;

	for (size_t i = 0; i < branch_request.branches.size(); ++i)
	{
		auto archs_responce = cpr::Get(cpr::Url{ "https://rdb.altlinux.org/api/site/all_pkgset_archs?branch=" + branch_request.branches[i]});
		json archs_json_responce = json::parse(archs_responce.text);
		get_request::ArchRequest arch_request = archs_json_responce.get<get_request::ArchRequest>();
		for (size_t j = 0; j < arch_request.archs.size(); ++j) 
		{
			archs.push_back(arch_request.archs[j].arch);
		}
	}
	sort(archs.begin(), archs.end());
	archs.erase(unique(archs.begin(), archs.end()), archs.end());

	json first_json_response = json::parse(first_response.text);
	json second_json_responce = json::parse(second_responce.text);

	get_request::Request first_request = first_json_response.get<get_request::Request>();
	get_request::Request second_request = second_json_responce.get<get_request::Request>();

	unordered_map<string, vector<get_request::Package>> elements_missing_in_the_second_responce = 
		find_missing_elements(first_request.packages, second_request.packages);
	unordered_map<string, vector<get_request::Package>> elements_missing_in_the_first_responce =
		find_missing_elements(first_request.packages, second_request.packages);


	return 0;
}
