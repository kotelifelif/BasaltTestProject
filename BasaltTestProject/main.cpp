#pragma once
#include "Request.h"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <utility>
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


unordered_map<string, vector<get_request::Package>> find_released_elements(vector<get_request::Package> first_packages, vector<get_request::Package> second_packages)
{
	struct ReleasePackage
	{
		vector<get_request::Package> packages;
		size_t count;
	};

	struct pair_hash {
		std::size_t operator () (const std::pair<std::string, std::string>& p) const {
			auto first_hash = std::hash<std::string>{}(p.first);
			auto second_hash = std::hash<std::string>{}(p.second);

			return first_hash ^ second_hash;
		}
	};

	unordered_map<pair<string, string>, ReleasePackage, pair_hash> release_packages;

	for (size_t i = 0; i < first_packages.size(); ++i)
	{
		pair<string, string> key = std::make_pair(first_packages[i].arch, first_packages[i].release);
		if (release_packages.find(key) != release_packages.end())
		{
			release_packages[key].count++;
			release_packages[key].packages.push_back(first_packages[i]);
		}
		else
		{
			ReleasePackage release_package;
			release_package.count = 1;
			release_package.packages.push_back(first_packages[i]);
			release_packages[key] = release_package;
		}
	}

	for (size_t i = 0; i < second_packages.size(); ++i)
	{
		pair<string, string> key = std::make_pair(second_packages[i].arch, second_packages[i].release);
		if (release_packages.find(key) != release_packages.end())
		{
			release_packages[key].count--;
		}
	}


	unordered_map<string, vector<get_request::Package>> released_elements;
	for (size_t i = 0; i < first_packages.size(); ++i)
	{
		pair<string, string> key = std::make_pair(first_packages[i].arch, first_packages[i].release);
		if (release_packages[key].count > 0)
		{
			released_elements[second_packages[i].arch].push_back(first_packages[i]);
		}
	}

	return released_elements;
}


int main()
{
	cpr::Response first_response = cpr::Get(cpr::Url{ "https://rdb.altlinux.org/api/export/branch_binary_packages/p10" });
	cpr::Response second_responce = cpr::Get(cpr::Url{ "https://rdb.altlinux.org/api/export/branch_binary_packages/sisyphus" });

	if (first_response.status_code != 200 || second_responce.status_code != 200)
	{
		return 1;
	}
	
	json first_json_response = json::parse(first_response.text);
	json second_json_responce = json::parse(second_responce.text);

	get_request::Request first_request = first_json_response.get<get_request::Request>();
	get_request::Request second_request = second_json_responce.get<get_request::Request>();

	unordered_map<string, vector<get_request::Package>> elements_missing_in_the_second_responce = 
		find_missing_elements(first_request.packages, second_request.packages);
	unordered_map<string, vector<get_request::Package>> elements_missing_in_the_first_responce =
		find_missing_elements(second_request.packages, first_request.packages);
	unordered_map<string, vector<get_request::Package>> released_elements =
		find_released_elements(first_request.packages, second_request.packages);

	return 0;
}
