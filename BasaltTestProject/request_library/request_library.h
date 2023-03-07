#pragma once
#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <unordered_map>

using json = nlohmann::json;

namespace request_library
{
    struct Package {
        std::string name;
        int64_t epoch;
        std::string version;
        std::string release;
        std::string arch;
        std::string disttag;
        int64_t buildtime;
        std::string source;

        bool operator == (const Package& package) const {
            return name == package.name && epoch == package.epoch &&
                version == package.version && release == package.release &&
                arch == package.arch && disttag == package.disttag &&
                buildtime == package.buildtime && source == package.source;
        }

        struct PackageHash {
            std::size_t operator()(const Package& package) const {
                return std::hash<std::string>()(package.name);
            }
        };
    };

    struct ReleasePackage {
        std::vector<Package> packages;
        size_t count;
    };

    struct pair_hash {
        size_t operator () (const std::pair<std::string, std::string>& p) const {
            auto first_hash = std::hash<std::string>{}(p.first);
            auto second_hash = std::hash<std::string>{}(p.second);

            return first_hash ^ second_hash;
        }
    };

    struct RequestElement {
        std::string arch;
        std::vector<Package> first_branch_missing_elements;
        std::vector<Package> second_branch_missing_elements;
        std::vector<Package> release_elements;
    };

    struct CompareBranchesStructure {
        std::vector<RequestElement> elements;
    };

    struct RequestArgs {
        json arch;
    };

    struct Request {
        RequestArgs request_args;
        int64_t length;
        std::vector<Package> packages;
    };

    struct Arch {
        std::string arch;
        int count;

        bool operator ==(const Arch& arch) {
            return arch.arch == this->arch && arch.count == count;
        }
    };

    struct ArchRequest {
        std::vector<Arch> archs;
        int length;
    };

    void from_json(const json& json_text, Arch& arch);
    void to_json(json& json_text, const Arch& arch);
    void from_json(const json& json_text, ArchRequest& request);
    void to_json(json& json_text, const ArchRequest& request);

    void from_json(const json& json_text, CompareBranchesStructure& structure);
    void to_json(json& json_text, const CompareBranchesStructure& structure);
    void from_json(const json& json_text, RequestElement& element);
    void to_json(json& json_text, const RequestElement& element);

    void from_json(const json& json_text, CompareBranchesStructure& structure);
    void to_json(json& json_text, const CompareBranchesStructure& structure);
    void from_json(const json& json_text, RequestElement& element);
    void to_json(json& json_text, const RequestElement& element);


    void from_json(const json& json_text, Package& package);
    void to_json(json& json_text, const Package& package);
    void from_json(const json& json_text, RequestArgs& args);
    void to_json(json& json_text, const RequestArgs& args);
    void from_json(const json& json_text, Request& request);
    void to_json(json& json_text, const Request& request);

    Request get_request(const std::string& url);
    ArchRequest get_arch_request(const std::string& url);

    std::unordered_map<std::string, std::vector<Package>> find_missing_elements(const Request& first_request, const Request& second_request);
    std::unordered_map<std::string, std::vector<Package>> find_released_elements(const Request& first_request, const Request& second_request);
    CompareBranchesStructure generate_structure(const std::string& first_branch, const std::string& second_branch,
        std::unordered_map<std::string, std::vector<Package>>& elements_missing_in_the_first_responce,
        std::unordered_map<std::string, std::vector<Package>>& elements_missing_in_the_second_responce,
        std::unordered_map<std::string, std::vector<Package>>& release_elements);
    void write_to_file(const std::string& file_name, const CompareBranchesStructure& structure);
}
