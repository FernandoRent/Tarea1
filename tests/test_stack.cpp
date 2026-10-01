/**
 *  @file test_stack.cpp
 *  @brief Pruebas de Stack. El ID de cada prueba corresponde a TEST_CASES.md
 */
#include "structures/Stack.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using structures::Stack;
using structures::EmptyStackException;
using structures::StructureException;

TEST(StackTest, S01_NewStackIsEmpty){
    Stack<int> pila;
    EXPECT_TRUE(pila.isEmpty());
    EXPECT_EQ(pila.size(), 0u);
}

TEST(StackTest, S02_PushThenPeek){
    Stack<int> pila;
    pila.push(10);
    pila.push(20);
    EXPECT_EQ(pila.peek(), 20);
    EXPECT_EQ(pila.size(), 2u);
    EXPECT_FALSE(pila.isEmpty());
}

TEST(StackTest, S03_PopReturnsInLifoOrder){
    Stack<int> pila;
    pila.push(1);
    pila.push(2);
    pila.push(3);
    EXPECT_EQ(pila.pop(), 3);
    EXPECT_EQ(pila.pop(), 2);
    EXPECT_EQ(pila.pop(), 1);
    EXPECT_TRUE(pila.isEmpty());
}

TEST(StackTest, S04_PopOnEmptyThrows){
    Stack<int> pila;
    EXPECT_THROW(pila.pop(), EmptyStackException);
}

TEST(StackTest, S05_PeekOnEmptyThrows){
    Stack<int> pila;
    const Stack<int>& constante = pila;
    EXPECT_THROW(pila.peek(), EmptyStackException);
    EXPECT_THROW(constante.peek(), EmptyStackException);
}

TEST(StackTest, S06_SingleElement){
    Stack<int> pila;
    pila.push(7);
    EXPECT_EQ(pila.size(), 1u);
    EXPECT_EQ(pila.peek(), 7);
    EXPECT_EQ(pila.pop(), 7);
    EXPECT_TRUE(pila.isEmpty());
    EXPECT_THROW(pila.pop(), EmptyStackException);
}

TEST(StackTest, S07_PeekAllowsModifyingTop){
    Stack<int> pila;
    pila.push(1);
    pila.push(2);
    pila.peek() = 99;
    EXPECT_EQ(pila.pop(), 99);
    EXPECT_EQ(pila.pop(), 1);
}

TEST(StackTest, S08_ConstPeek){
    Stack<int> pila;
    pila.push(5);
    const Stack<int>& constante = pila;
    EXPECT_EQ(constante.peek(), 5);
    EXPECT_EQ(constante.size(), 1u);
}

TEST(StackTest, S09_ClearEmptiesStack){
    Stack<int> pila;
    pila.clear();
    EXPECT_TRUE(pila.isEmpty());
    pila.push(1);
    pila.push(2);
    pila.clear();
    EXPECT_TRUE(pila.isEmpty());
    EXPECT_EQ(pila.size(), 0u);
    pila.push(3);
    EXPECT_EQ(pila.peek(), 3);
}

TEST(StackTest, S10_IterationTopToBottom){
    Stack<int> pila;
    pila.push(1);
    pila.push(2);
    pila.push(3);
    std::vector<int> recorrido;
    for (const auto& elemento : pila){
        recorrido.push_back(elemento);
    }
    EXPECT_EQ(recorrido, (std::vector<int>{3, 2, 1}));
    EXPECT_EQ(pila.size(), 3u);
}

TEST(StackTest, S11_PrintFormat){
    Stack<int> pila;
    std::ostringstream vacia;
    vacia << pila;
    EXPECT_EQ(vacia.str(), "[]");

    pila.push(7);
    std::ostringstream uno;
    uno << pila;
    EXPECT_EQ(uno.str(), "[7]");

    pila.push(8);
    pila.push(9);
    std::ostringstream varios;
    varios << pila;
    EXPECT_EQ(varios.str(), "[9, 8, 7]");
}

TEST(StackTest, S12_StringsCopyKeepsOriginal){
    Stack<std::string> pila;
    std::string original = "hola";
    pila.push(original);
    pila.push(std::string("mundo"));
    EXPECT_EQ(original, "hola");
    EXPECT_EQ(pila.pop(), "mundo");
    EXPECT_EQ(pila.pop(), "hola");
}

TEST(StackTest, S13_MoveOnlyUniquePtr){
    Stack<std::unique_ptr<int>> pila;
    pila.push(std::make_unique<int>(1));
    auto puntero = std::make_unique<int>(2);
    pila.push(std::move(puntero));
    EXPECT_EQ(puntero, nullptr);
    EXPECT_EQ(*pila.peek(), 2);
    std::unique_ptr<int> sacado = pila.pop();
    EXPECT_EQ(*sacado, 2);
    EXPECT_EQ(*pila.pop(), 1);
}

TEST(StackTest, S14_TenThousandElements){
    Stack<int> pila;
    for (int i = 0; i < 10000; ++i){
        pila.push(i);
    }
    EXPECT_EQ(pila.size(), 10000u);
    for (int i = 9999; i >= 0; --i){
        ASSERT_EQ(pila.pop(), i);
    }
    EXPECT_TRUE(pila.isEmpty());
}

TEST(StackTest, S15_ExceptionIsStructureException){
    Stack<int> pila;
    EXPECT_THROW(pila.pop(), StructureException);
    EXPECT_THROW(pila.pop(), std::runtime_error);
    try{
        pila.pop();
        FAIL() << "pop() debio lanzar";
    } catch (const EmptyStackException& e){
        EXPECT_STREQ(e.what(), "la pila esta vacia");
    }
}
