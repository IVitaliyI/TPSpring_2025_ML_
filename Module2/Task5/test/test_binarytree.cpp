#include "catch_amalgamated.hpp"
#include "BinaryTree.hpp"

TEST_CASE("Remove from empty tree") {
    BinaryTree<unsigned char> tree;
    REQUIRE(tree.remove(10) == false);  // Удалить из пустого дерева не удастся
    REQUIRE(tree.GetRoot() == nullptr); // Корень дерева должен быть nullptr
}

TEST_CASE("Remove root with no children") {
    BinaryTree<unsigned char> tree;
    tree.insert(10);  // Вставляем корень
    REQUIRE(tree.remove(10) == true);  // Удаляем корень
    REQUIRE(tree.GetRoot() == nullptr); // Дерево должно стать пустым
}

TEST_CASE("Remove root with one child") {
    BinaryTree<unsigned char> tree;
    tree.insert(10);
    tree.insert(5);  // Вставляем дочерний узел
    REQUIRE(tree.remove(10) == true);  // Удаляем корень
    REQUIRE(tree.GetRoot()->Data == 5); // Новый корень должен быть 5
}

TEST_CASE("Remove node with two children") {
    BinaryTree<unsigned char> tree;
    tree.insert(10);
    tree.insert(5);  // Левый потомок
    tree.insert(15); // Правый потомок
    tree.insert(3);  // Левый потомок 5
    tree.insert(7);  // Правый потомок 5
    tree.insert(12); // Левый потомок 15
    tree.insert(18); // Правый потомок 15
    
    // Удаляем узел с двумя потомками (например, 10)
    REQUIRE(tree.remove(10) == true);

    // После удаления, корень должен быть 12 (минимум из правого поддерева)
    REQUIRE(tree.GetRoot()->Data == 12); 

    // 10 должен быть удален, 12 становится корнем, 5 и 15 остаются
    REQUIRE(tree.GetRoot()->left->Data == 5);
    REQUIRE(tree.GetRoot()->right->Data == 15);
}

TEST_CASE("Remove non-existent node") {
    BinaryTree<unsigned char> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    REQUIRE(tree.remove(20) == false);  // Узел 20 не существует в дереве
    REQUIRE(tree.GetRoot()->Data == 10); // Корень остается 10
    REQUIRE(tree.GetRoot()->left->Data == 5);  // Левый потомок остаётся 5
    REQUIRE(tree.GetRoot()->right->Data == 15); // Правый потомок остаётся 15
}

TEST_CASE("Remove leaf node") {
    BinaryTree<unsigned char> tree;
    tree.insert(10);
    tree.insert(5);  // Левый потомок
    tree.insert(15); // Правый потомок

    REQUIRE(tree.remove(5) == true); // Удаляем листья
    REQUIRE(tree.GetRoot()->left == nullptr); // Левый потомок должен быть null
    REQUIRE(tree.GetRoot()->right->Data == 15); // Правый потомок остаётся
}

TEST_CASE("Remove node with one child") {
    BinaryTree<unsigned char> tree;
    tree.insert(10);
    tree.insert(5);  // Левый потомок
    tree.insert(3);  // Левый потомок 5

    REQUIRE(tree.remove(5) == true);  // Удаляем узел с одним потомком
    REQUIRE(tree.GetRoot()->left->Data == 3);  // Левый потомок 10 теперь 3
}
