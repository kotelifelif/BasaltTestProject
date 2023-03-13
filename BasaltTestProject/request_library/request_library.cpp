#include "request_library.h"

#include <unordered_set>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace request_library {
    
    Request get_request(const std::string& url) {
        Request request;
        cpr::Response response = cpr::Get(cpr::Url{ "https://rdb.altlinux.org/api/export/branch_binary_packages/" + url });

        constexpr size_t success_code = 200;
        if (response.status_code != success_code) {
            return request;
        }

        json json_response = json::parse(response.text);
        request = json_response.get<Request>();

        return request;
    }

    ArchRequest get_arch_request(const std::string& url) {
        ArchRequest request;
        cpr::Response response = cpr::Get(cpr::Url{ "https://rdb.altlinux.org/api/site/all_pkgset_archs?branch=" + url });

        constexpr size_t success_code = 200;
        if (response.status_code != success_code) {
            return request;
        }

        json json_response = json::parse(response.text);
        request = json_response.get<ArchRequest>();

        return request;
    }

    void from_json(const json& json_text, Arch& arch) {
        arch.arch = json_text["arch"];
        arch.count = json_text["count"];
    }

    void to_json(json& json_text, const Arch& arch) {
        json_text = json::object();
        json_text["arch"] = arch.arch;
        json_text["count"] = arch.count;
    }

    void from_json(const json& json_text, ArchRequest& request) {
        request.archs = json_text.at("archs").get<std::vector<Arch>>();
        request.length = json_text["length"];
    }

    void to_json(json& json_text, const ArchRequest& request) {
        json_text = json::object();
        json_text["archs"] = request.archs;
        json_text["length"] = request.length;
    }

    void from_json(const json& json_text, CompareBranchesStructure& structure) {
        structure.elements = json_text.at("elements").get<std::vector<RequestElement>>();
    }

    void to_json(json& json_text, const CompareBranchesStructure& structure) {
        json_text = json::object();
        json_text["elements"] = structure.elements;
    }

    void from_json(const json& json_text, RequestElement& element) {
        element.arch = json_text.at("arch").get<std::string>();
        element.first_branch_missing_elements = json_text.at("first_branch_missing_elements").get<std::vector<Package>>();
        element.second_branch_missing_elements = json_text.at("second_branch_missing_elements").get<std::vector<Package>>();
        element.release_elements = json_text.at("release_elements").get<std::vector<Package>>();
    }

    void to_json(json& json_text, const RequestElement& element) {
        json_text = json::object();
        json_text["arch"] = element.arch;
        json_text["first_branch_missing_elements"] = element.first_branch_missing_elements;
        json_text["second_branch_missing_elements"] = element.second_branch_missing_elements;
        json_text["release_elements"] = element.release_elements;
    }

    void from_json(const json& json_text, Package& package) {
        package.name = json_text.at("name").get<std::string>();
        package.epoch = json_text.at("epoch").get<int64_t>();
        package.version = json_text.at("version").get<std::string>();
        package.release = json_text.at("release").get<std::string>();
        package.arch = json_text.at("arch").get<std::string>();
        package.disttag = json_text.at("disttag").get<std::string>();
        package.buildtime = json_text.at("buildtime").get<int64_t>();
        package.source = json_text.at("source").get<std::string>();
    }

    void to_json(json& json_text, const Package& package) {
        json_text = json::object();
        json_text["name"] = package.name;
        json_text["epoch"] = package.epoch;
        json_text["version"] = package.version;
        json_text["release"] = package.release;
        json_text["arch"] = package.arch;
        json_text["disttag"] = package.disttag;
        json_text["buildtime"] = package.buildtime;
        json_text["source"] = package.source;
    }

    void from_json(const json& json_text, RequestArgs& args) {
        args.arch = json_text;
    }

    void to_json(json& json_text, const RequestArgs& args) {
        json_text = json::object();
        json_text["arch"] = args.arch;
    }

    void from_json(const json& json_text, Request& request) {
        request.request_args = json_text.at("request_args").get<RequestArgs>();
        request.length = json_text.at("length").get<int64_t>();
        request.packages = json_text.at("packages").get<std::vector<Package>>();
    }

    void to_json(json& json_text, const Request& request) {
        json_text = json::object();
        json_text["request_args"] = request.request_args;
        json_text["length"] = request.length;
        json_text["packages"] = request.packages;
    }

    std::unordered_map<std::string, std::vector<Package>> find_missing_elements(const Request& first_request, const Request& second_request) {
        std::unordered_map<std::string, std::vector<Package>> missing_elements;
        std::unordered_set<Package, Package::PackageHash> elements;
        for (size_t i = 0; i < first_request.packages.size(); ++i) {
            elements.insert(first_request.packages[i]);
        }

        for (size_t i = 0; i < second_request.packages.size(); ++i) {
            if (elements.find(second_request.packages[i]) == elements.end()) {
                missing_elements[second_request.packages[i].arch].push_back(second_request.packages[i]);
            }
        }

        return missing_elements;
    }

    std::unordered_map<std::string, std::vector<Package>> find_released_elements(const Request& first_request, const Request& second_request) {
        std::unordered_map<std::pair<std::string, std::string>, ReleasePackage, pair_hash> release_packages;

        for (size_t i = 0; i < first_request.packages.size(); ++i) {
            std::pair<std::string, std::string> key = std::make_pair(first_request.packages[i].arch, first_request.packages[i].release);
            if (release_packages.find(key) != release_packages.end()) {
                release_packages[key].count++;
                release_packages[key].packages.push_back(first_request.packages[i]);
            }
            else {
                ReleasePackage release_package;
                release_package.count = 1;
                release_package.packages.push_back(first_request.packages[i]);
                release_packages[key] = release_package;
            }
        }

        for (size_t i = 0; i < second_request.packages.size(); ++i) {
            std::pair<std::string, std::string> key = std::make_pair(second_request.packages[i].arch, second_request.packages[i].release);
            if (release_packages.find(key) != release_packages.end()) {
                release_packages[key].count--;
            }
        }


        std::unordered_map<std::string, std::vector<Package>> released_elements;
        for (size_t i = 0; i < first_request.packages.size(); ++i) {
            std::pair<std::string, std::string> key = std::make_pair(first_request.packages[i].arch, first_request.packages[i].release);
            if (release_packages[key].count > 0) {
                released_elements[first_request.packages[i].arch].push_back(first_request.packages[i]);
            }
        }

        return released_elements;
    }

    CompareBranchesStructure generate_structure(const std::string& first_branch,
        const std::string& second_branch,
        std::unordered_map<std::string, std::vector<Package>>& elements_missing_in_the_first_responce,
        std::unordered_map<std::string, std::vector<Package>>& elements_missing_in_the_second_responce,
        std::unordered_map<std::string, std::vector<Package>>& release_elements) {
        ArchRequest arch_request = get_arch_request(first_branch);
        ArchRequest second_arch_request = get_arch_request(second_branch);

        for (int i = 0; i < second_arch_request.archs.size(); ++i) {
            auto find_arch = std::find(arch_request.archs.begin(), arch_request.archs.end(), second_arch_request.archs[i]);
            if (find_arch == arch_request.archs.end()) {
                arch_request.archs.push_back(second_arch_request.archs[i]);
            }
        }


        CompareBranchesStructure structure;
        for (int i = 0; i < arch_request.archs.size(); ++i)  {
            RequestElement element;
            element.arch = arch_request.archs[i].arch;
            structure.elements.push_back(element);
        }

        for (int i = 0; i < arch_request.archs.size(); ++i) {
            if (elements_missing_in_the_second_responce.find(arch_request.archs[i].arch) != elements_missing_in_the_second_responce.end()) {
                for (int j = 0; j < elements_missing_in_the_second_responce[arch_request.archs[i].arch].size(); ++j) {
                    structure.elements[i].second_branch_missing_elements.push_back(
                        elements_missing_in_the_second_responce[arch_request.archs[i].arch][j]);
                }
            }
        }


        for (int i = 0; i < arch_request.archs.size(); ++i) {
            if (elements_missing_in_the_first_responce.find(arch_request.archs[i].arch) != elements_missing_in_the_first_responce.end()) {
                for (int j = 0; j < elements_missing_in_the_first_responce[arch_request.archs[i].arch].size(); ++j) {
                    structure.elements[i].first_branch_missing_elements.push_back(
                        elements_missing_in_the_first_responce[arch_request.archs[i].arch][j]);
                }
            }
        }

        for (int i = 0; i < arch_request.archs.size(); ++i) {
            if (release_elements.find(arch_request.archs[i].arch) != release_elements.end()) {
                for (int j = 0; j < release_elements[arch_request.archs[i].arch].size(); ++j) {
                    structure.elements[i].release_elements.push_back(
                        release_elements[arch_request.archs[i].arch][j]);
                }
            }
        }
        return structure;
    }

    void write_to_file(const std::string& file_name, const CompareBranchesStructure& structure) {
        json json_structure;
        json_structure["elements"] = structure.elements;

        std::ofstream file(file_name);
        file << json_structure << std::endl;
    }
}


