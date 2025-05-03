#include <catch_amalgamated.hpp>
#include "CharCounting.hpp"

TEST_CASE("Test CharCounting class", "[CharCounting]") {
    CharCounting charCounting;

    SECTION("Test CalculateCounting and GetDictionary") {
        std::string testStr = "aabbcc";
        charCounting.CalculateCounting(testStr);

        auto dict = charCounting.GetDictionary();

        // Проверяем количество уникальных символов
        REQUIRE(dict.size() == 3);

        // Проверяем частоты символов
        REQUIRE(dict['a'] == 2);
        REQUIRE(dict['b'] == 2);
        REQUIRE(dict['c'] == 2);
    }

    SECTION("Test empty string") {
        std::string testStr = "";
        charCounting.CalculateCounting(testStr);

        auto dict = charCounting.GetDictionary();

        // Проверяем, что словарь пуст
        REQUIRE(dict.size() == 0);
    }

    SECTION("Test single character") {
        std::string testStr = "aaaaa";
        charCounting.CalculateCounting(testStr);

        auto dict = charCounting.GetDictionary();

        // Проверяем, что словарь содержит только один символ 'a'
        REQUIRE(dict.size() == 1);
        REQUIRE(dict['a'] == 5);
    }
}
