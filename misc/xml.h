#ifndef XML_H
#define XML_H

#include "../main/header.h"
#include "../main/utility.h"
#include <nlohmann/json.hpp>

//
// TODO: check this lib: https://github.com/testillano/json2xml
//

#include <tinyxml.h>

// operate with tinyxml
void run_tinyxml()
{
    const auto xmlStr = R"(
    <root>
        <topic>
            <name>topic_1</name>
            <fund>fund_1</fund>
        </topic>
        <topic>
            <name>topic_2</name>
            <fund>fund_2</fund>
        </topic>
    </root>
    )";

    TiXmlDocument doc;
    doc.Parse(xmlStr);

    // print to stdout (not std::cout)
    doc.Print();

    doc.SaveFile("/tmp/a.xml");

    TiXmlPrinter printer;
    printer.SetIndent("");
    printer.SetLineBreak("");
    // above 2 lines are equal to SetStreamPrinting()
    // printer.SetStreamPrinting();
    doc.Accept(&printer);

    std::cout << "===\n" << printer.Str() << std::endl;

    std::cout << "------" << std::endl;

    std::ostringstream oss;
    // operator<< gives unformatted string, meaning no indent thing
    oss << doc;
    std::cout << oss.str() << std::endl;
}

using json         = nlohmann::json;
using ordered_json = nlohmann::ordered_json;
using namespace nlohmann::literals;

class json_to_xml
{
public:
    explicit json_to_xml(const nlohmann::ordered_json& j, uint32_t indentDepth = 0) : json_to_xml{j, indentDepth, ""} {}

    friend std::ostream& operator<<(std::ostream& os, const json_to_xml& x);

private:
    const uint32_t                indentDepth_{0};
    const std::string             array_name_;
    nlohmann::ordered_json const& json_;

    json_to_xml(const nlohmann::ordered_json& j, uint32_t indentDepth, const std::string& array_name)
        : indentDepth_{indentDepth},
          array_name_{array_name},
          json_(j)
    {
    }
};

std::ostream& operator<<(std::ostream& os, const json_to_xml& input)
{
    auto&      j            = input.json_;
    const auto currentDepth = input.indentDepth_;

    auto printIndent = [&](auto depth) {
        constexpr auto spacePerDepth = 2;
        os << std::setfill(' ') << std::setw(depth * spacePerDepth) << "";
    };

    if (j.is_array()) {
        for (auto& item : j.items()) {
            printIndent(currentDepth);
            os << "<" << input.array_name_ << ">";

            if (item.value().is_object()) {
                os << "\n";
            }

            os << json_to_xml(item.value(), currentDepth);

            if (item.value().is_object()) {
                printIndent(currentDepth);
            }
            os << "</" << input.array_name_ << ">\n";
        }
    } else if (j.is_object()) {
        for (auto& item : j.items()) {
            const auto key = item.key();
            if (item.value().is_array()) {
                os << json_to_xml(item.value(), currentDepth + 1, key);
            } else {
                printIndent(currentDepth + 1);
                os << "<" << key << ">" << json_to_xml(item.value(), currentDepth + 1, key) << "</" << key << ">\n";
            }
        }
    } else if (j.is_string()) {
        os << j.get<std::string>();
    } else if (j.is_null()) {
        return os;
    } else    // j.is_primitive()
    {
        os << j.dump();
    }

    return os;
}

bool writeXmlFile(const nlohmann::ordered_json& j, std::filesystem::path filepath)
{
    std::ofstream ofs;
    ofs.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    ofs.open(filepath.native(), std::ofstream::out);

    ofs << R"(<?xml version="1.0" encoding="UTF-8"?>
<root>
)";

    ofs << json_to_xml(j, 1);

    ofs << R"(</root>)";

    ofs.close();
    return ofs.good();
}

// operate with json > xml conversion
void run_json()
{
    // create an array using push_back
    ordered_json j;
    j.push_back("foo");
    j.push_back(1);
    j.push_back(true);

    ordered_json j2               = R"({})"_json;
    j2["Topics"]                  = R"([])"_json;
    j2["Topics"][0]["Filter"]     = R"([])"_json;
    j2["Topics"][0]["Filter"][0]  = "fake";
    j2["Topics"][0]["Filter"][1]  = "other";
    j2["Topics"][0]["Fund"]       = "fund1";
    j2["Topics"][0]["Desk"]       = "desk1";
    j2["Topics"][0]["AppTags"]    = R"([])"_json;
    j2["Topics"][0]["AppTags"][0] = 12;
    j2["Topics"][0]["AppTags"][1] = 33;

    j2["Topics"][1]["Filter"]  = R"([])"_json;
    j2["Topics"][1]["Filter"]  = {"filter1", "filter2"};
    j2["Topics"][1]["Fund"]    = "fund2";
    j2["Topics"][1]["Desk"]    = "desk2";
    j2["Topics"][1]["AppTags"] = R"([])"_json;
    j2["Topics"][1]["AppTags"] = {100, 200};

    writeXmlFile(j2, "/tmp/a.xml");

    std::cout << json_to_xml(j2) << std::endl;
    std::cout << "\n-----------\n" << std::endl;
    std::cout << json_to_xml(j) << std::endl;

    std::cout << j.dump() << "\n" << std::setw(2) << j << std::endl;
    std::cout << j2.dump() << "\n" << std::setw(2) << j2 << std::endl;
}

#endif

