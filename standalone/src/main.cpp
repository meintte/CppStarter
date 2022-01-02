#include <cxxopts.hpp>
#include <greeter/greeter.hpp>
#include <greeter/version.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

auto main(int argc, char** argv) -> int {
    const std::unordered_map<std::string, greeter_namespace::LanguageCode> languages{
        {"en", greeter_namespace::LanguageCode::EN},
        {"de", greeter_namespace::LanguageCode::DE},
        {"es", greeter_namespace::LanguageCode::ES},
        {"fr", greeter_namespace::LanguageCode::FR},
    };

    cxxopts::Options options(*argv, "A program to welcome the world!");

    std::string language;
    std::string name;

    // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
    ("n,name", "Name to greet", cxxopts::value(name)->default_value("World"))
    ("l,lang", "Language code to use", cxxopts::value(language)->default_value("en"))
  ;
    // clang-format on

    auto result = options.parse(argc, argv);

    if (result["help"].as<bool>()) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (result["version"].as<bool>()) {
        std::cout << "Greeter, version " << GREETER_VERSION << std::endl;
        return 0;
    }

    auto langIt = languages.find(language);
    if (langIt == languages.end()) {
        std::cerr << "unknown language code: " << language << std::endl;
        return 1;
    }

    greeter_namespace::Greeter greeter(name);
    std::cout << greeter.greet(langIt->second) << std::endl;

    return 0;
}
