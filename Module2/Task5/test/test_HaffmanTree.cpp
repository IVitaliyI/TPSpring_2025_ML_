#define CATCH_CONFIG_MAIN
#include <catch_amalgamated.hpp>
#include "HaffmanTree.hpp"

TEST_CASE("HaffmanTree build and codes generation") {
    // Создаём дерево Хаффмана
    HaffmanTree tree;

    // Пример словаря с частотами символов
    std::unordered_map<unsigned char, size_t> dictionary = {
        {'a', 5},
        {'b', 2},
        {'c', 1},
        {'d', 1},
        {'r', 2}
    };

    // Строим дерево
    tree.build(dictionary);

    // Проверка, что длина кодов символов была сгенерирована правильно
    SECTION("Codes length generated correctly") {
        std::unordered_map<unsigned char, size_t> expected_lengths = {
            {'a', 1},
            {'b', 3},
            {'c', 3},
            {'d', 3},
            {'r', 3}
        };
        tree.visualiseTree();
        tree.printCodes();

        for (const auto& [symbol, expected_length] : expected_lengths) {
            REQUIRE(tree.getCode(symbol).length() == expected_length);
        }
    }
}

TEST_CASE("Visualize Tree (basic structure)") {
    // Создаём дерево Хаффмана
    HaffmanTree tree;

    // Пример словаря с частотами символов
    std::unordered_map<unsigned char, size_t> dictionary = {
        {'a', 5},
        {'b', 7},
        {'c', 10},
        {'d', 15}
    };

    // Строим дерево
    tree.build(dictionary);

    // Печать дерева в текстовом виде
    SECTION("Tree visualisation") {
        // Используйте метод, который визуализирует дерево (реализуйте его)
        tree.visualiseTree();
        // Вы можете проверить, что вывод на экран соответствует ожидаемому
        // Например, вы можете перенаправить вывод в строку и затем проверить
        // соответствие, но в Catch2 нет прямой проверки вывода в консоль
    }
}

TEST_CASE("Tree destruction") {
    // Создаём дерево Хаффмана
    {
        HaffmanTree tree;

        std::unordered_map<unsigned char, size_t> dictionary = {
            {'a', 5},
            {'b', 7}
        };

        // Строим дерево
        tree.build(dictionary);
    }
    // Здесь тестируем, что деструктор правильно удаляет все элементы
    // Этот тест не нуждается в явной проверке, так как память будет очищена при выходе из области видимости
    // объекта tree, но вы можете добавить проверку на утечку памяти с помощью инструментов, таких как valgrind.
}

TEST_CASE("Empty Tree") {
    // Тестируем работу с пустым деревом
    HaffmanTree tree;

    SECTION("Empty tree should have no codes") {
        // Пустое дерево не должно иметь кодов
        REQUIRE(tree.getCode('a') == "");
    }
}
