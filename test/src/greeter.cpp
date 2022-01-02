#include <doctest/doctest.h>

#include <greeter/greeter.hpp>
#include <greeter/version.hpp>
#include <string>

TEST_CASE("Greeter") {
    greeter::Greeter greeter("Tests");

    CHECK(greeter.greet(greeter::LanguageCode::EN) == "Hello, Tests!");
    CHECK(greeter.greet(greeter::LanguageCode::DE) == "Hallo Tests!");
    CHECK(greeter.greet(greeter::LanguageCode::ES) == "¡Hola Tests!");
    CHECK(greeter.greet(greeter::LanguageCode::FR) == "Bonjour Tests!");
}

TEST_CASE("Greeter version") {
    static_assert(std::string_view(GREETER_VERSION) == std::string_view("1.0"));
    CHECK(std::string(GREETER_VERSION) == std::string("1.0"));
}
