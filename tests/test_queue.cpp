/**
 *  @file test_queue.cpp
 *  @brief Pruebas de Queue. El ID de cada prueba corresponde a TEST_CASES.md
 */
#include "structures/Queue.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using structures::Queue;
using structures::EmptyQueueException;
using structures::StructureException;

TEST(QueueTest, Q01_NewQueueIsEmpty){
    Queue<int> fila;
    EXPECT_TRUE(fila.isEmpty());
    EXPECT_EQ(fila.size(), 0u);
}

TEST(QueueTest, Q02_EnqueueThenFrontAndBack){
    Queue<int> fila;
    fila.enqueue(10);
    fila.enqueue(20);
    fila.enqueue(30);
    EXPECT_EQ(fila.front(), 10);
    EXPECT_EQ(fila.back(), 30);
    EXPECT_EQ(fila.size(), 3u);
}

TEST(QueueTest, Q03_DequeueReturnsInFifoOrder){
    Queue<int> fila;
    fila.enqueue(1);
    fila.enqueue(2);
    fila.enqueue(3);
    EXPECT_EQ(fila.dequeue(), 1);
    EXPECT_EQ(fila.dequeue(), 2);
    EXPECT_EQ(fila.dequeue(), 3);
    EXPECT_TRUE(fila.isEmpty());
}

TEST(QueueTest, Q04_DequeueOnEmptyThrows){
    Queue<int> fila;
    EXPECT_THROW(fila.dequeue(), EmptyQueueException);
}

TEST(QueueTest, Q05_FrontOnEmptyThrows){
    Queue<int> fila;
    const Queue<int>& constante = fila;
    EXPECT_THROW(fila.front(), EmptyQueueException);
    EXPECT_THROW(constante.front(), EmptyQueueException);
}

TEST(QueueTest, Q06_BackOnEmptyThrows){
    Queue<int> fila;
    const Queue<int>& constante = fila;
    EXPECT_THROW(fila.back(), EmptyQueueException);
    EXPECT_THROW(constante.back(), EmptyQueueException);
}

TEST(QueueTest, Q07_SingleElementFrontEqualsBack){
    Queue<int> fila;
    fila.enqueue(7);
    EXPECT_EQ(fila.front(), 7);
    EXPECT_EQ(fila.back(), 7);
    EXPECT_EQ(fila.dequeue(), 7);
    EXPECT_TRUE(fila.isEmpty());
    EXPECT_THROW(fila.dequeue(), EmptyQueueException);
}

TEST(QueueTest, Q08_FrontAndBackAllowModification){
    Queue<int> fila;
    fila.enqueue(1);
    fila.enqueue(2);
    fila.front() = 50;
    fila.back() = 60;
    const Queue<int>& constante = fila;
    EXPECT_EQ(constante.front(), 50);
    EXPECT_EQ(constante.back(), 60);
}

TEST(QueueTest, Q09_ClearEmptiesQueue){
    Queue<int> fila;
    fila.clear();
    EXPECT_TRUE(fila.isEmpty());
    fila.enqueue(1);
    fila.enqueue(2);
    fila.clear();
    EXPECT_TRUE(fila.isEmpty());
    EXPECT_EQ(fila.size(), 0u);
    fila.enqueue(3);
    EXPECT_EQ(fila.front(), 3);
}

TEST(QueueTest, Q10_IterationFrontToBack){
    Queue<int> fila;
    fila.enqueue(1);
    fila.enqueue(2);
    fila.enqueue(3);
    std::vector<int> recorrido;
    for (const auto& elemento : fila){
        recorrido.push_back(elemento);
    }
    EXPECT_EQ(recorrido, (std::vector<int>{1, 2, 3}));
    EXPECT_EQ(fila.size(), 3u);
}

TEST(QueueTest, Q11_PrintFormat){
    Queue<int> fila;
    std::ostringstream vacia;
    vacia << fila;
    EXPECT_EQ(vacia.str(), "[]");

    fila.enqueue(7);
    std::ostringstream uno;
    uno << fila;
    EXPECT_EQ(uno.str(), "[7]");

    fila.enqueue(8);
    fila.enqueue(9);
    std::ostringstream varios;
    varios << fila;
    EXPECT_EQ(varios.str(), "[7, 8, 9]");
}

TEST(QueueTest, Q12_StringsCopyKeepsOriginal){
    Queue<std::string> fila;
    std::string original = "hola";
    fila.enqueue(original);
    fila.enqueue(std::string("mundo"));
    EXPECT_EQ(original, "hola");
    EXPECT_EQ(fila.dequeue(), "hola");
    EXPECT_EQ(fila.dequeue(), "mundo");
}

TEST(QueueTest, Q13_MoveOnlyUniquePtr){
    Queue<std::unique_ptr<int>> fila;
    fila.enqueue(std::make_unique<int>(1));
    auto puntero = std::make_unique<int>(2);
    fila.enqueue(std::move(puntero));
    EXPECT_EQ(puntero, nullptr);
    EXPECT_EQ(*fila.front(), 1);
    EXPECT_EQ(*fila.back(), 2);
    EXPECT_EQ(*fila.dequeue(), 1);
    EXPECT_EQ(*fila.dequeue(), 2);
}

TEST(QueueTest, Q14_TenThousandElements){
    Queue<int> fila;
    for (int i = 0; i < 10000; ++i){
        fila.enqueue(i);
    }
    EXPECT_EQ(fila.size(), 10000u);
    EXPECT_EQ(fila.front(), 0);
    EXPECT_EQ(fila.back(), 9999);
    for (int i = 0; i < 10000; ++i){
        ASSERT_EQ(fila.dequeue(), i);
    }
    EXPECT_TRUE(fila.isEmpty());
}

TEST(QueueTest, Q15_InterleavedEnqueueDequeue){
    Queue<int> fila;
    fila.enqueue(1);
    fila.enqueue(2);
    EXPECT_EQ(fila.dequeue(), 1);
    fila.enqueue(3);
    EXPECT_EQ(fila.dequeue(), 2);
    EXPECT_EQ(fila.dequeue(), 3);
    EXPECT_TRUE(fila.isEmpty());
}

TEST(QueueTest, Q16_ExceptionIsStructureException){
    Queue<int> fila;
    EXPECT_THROW(fila.dequeue(), StructureException);
    EXPECT_THROW(fila.dequeue(), std::runtime_error);
    try{
        fila.dequeue();
        FAIL() << "dequeue() debio lanzar";
    } catch (const EmptyQueueException& e){
        EXPECT_STREQ(e.what(), "la fila esta vacia");
    }
}
