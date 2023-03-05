#pragma once
#include <nlohmann/json.hpp>

#include <string>
#include <vector>

using json = nlohmann::json;

namespace get_request {
      
    struct BranchRequest {
        std::vector<std::string> branches;
    };

    struct Arch {
        size_t count;
        std::string arch;
    };

    struct ArchRequest {
        std::vector<Arch> archs;
    };
    
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

    struct RequestArgs {
        json arch;
    };

    struct Request {
        RequestArgs request_args;
        int64_t length;
        std::vector<Package> packages;
    };

    inline void from_json(const json& json_text, Arch& request) {
        request.count = json_text.at("count").get<size_t>();
        request.arch = json_text.at("arch").get<std::string>();
    }

    inline void to_json(json& json_text, const Arch& request) {
        json_text = json::object();
        json_text["count"] = request.count;
        json_text["arch"] = request.arch;
    }

    inline void from_json(const json& json_text, ArchRequest& request) {
        request.archs = json_text.at("archs").get<std::vector<Arch>>();
    }

    inline void to_json(json& json_text, const ArchRequest& request) {
        json_text = json::object();
        json_text["archs"] = request.archs;
    }

    inline void from_json(const json& json_text, BranchRequest& request) {
        request.branches = json_text.at("branches").get<std::vector<std::string>>();
    }

    inline void to_json(json& json_text, const BranchRequest& request) {
        json_text = json::object();
        json_text["branches"] = request.branches;
    }

    inline void from_json(const json& json_text, Package& package) {
        package.name = json_text.at("name").get<std::string>();
        package.epoch = json_text.at("epoch").get<int64_t>();
        package.version = json_text.at("version").get<std::string>();
        package.release = json_text.at("release").get<std::string>();
        package.arch = json_text.at("arch").get<std::string>();
        package.disttag = json_text.at("disttag").get<std::string>();
        package.buildtime = json_text.at("buildtime").get<int64_t>();
        package.source = json_text.at("source").get<std::string>();
    }

    inline void to_json(json& json_text, const Package& package) {
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

    inline void from_json(const json& json_text, RequestArgs& args) {
        args.arch = json_text;
    }

    inline void to_json(json& json_text, const RequestArgs& args) {
        json_text = json::object();
        json_text["arch"] = args.arch;
    }

    inline void from_json(const json& json_text, Request& request) {
        request.request_args = json_text.at("request_args").get<RequestArgs>();
        request.length = json_text.at("length").get<int64_t>();
        request.packages = json_text.at("packages").get<std::vector<Package>>();
    }

    inline void to_json(json& json_text, const Request& request) {
        json_text = json::object();
        json_text["request_args"] = request.request_args;
        json_text["length"] = request.length;
        json_text["packages"] = request.packages;
    }
}