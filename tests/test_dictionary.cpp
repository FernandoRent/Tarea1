/**
 *  @file test_dictionary.cpp
 *  @brief Pruebas de Dictionary. El ID de cada prueba corresponde a TEST_CASES.md
 */
#include "structures/Dictionary.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using structures::Dictionary;
using structures::KeyNotFoundException;
using structures::StructureException;

using Pares = std::vector<std::pair<std::string, int>>;

TEST(DictionaryTest, D01_NewDictionaryIsEmpty){
    Dictionary<std::string, int> dic;
    EXPECT_TRUE(dic.isEmpty());
    EXPECT_EQ(dic.size(), 0u);
    EXPECT_FALSE(dic.contains("x"));
}

TEST(DictionaryTest, D02_PutThenGet){
    Dictionary<std::string, int> dic;
    dic.put("uno", 1);
    dic.put("dos", 2);
    EXPECT_EQ(dic.get("uno"), 1);
    EXPECT_EQ(dic.get("dos"), 2);
    EXPECT_EQ(dic.size(), 2u);
}

TEST(DictionaryTest, D03_RepeatedKeyUpdatesWithoutGrowing){
    Dictionary<std::string, int> dic;
    dic.put("x", 1);
    dic.put("x", 2);
    dic.put("x", 3);
    EXPECT_EQ(dic.size(), 1u);
    EXPECT_EQ(dic.get("x"), 3);
}

TEST(DictionaryTest, D04_UpdateKeepsInsertionPosition){
    Dictionary<std::string, int> dic;
    dic.put("z", 1);
    dic.put("a", 2);
    dic.put("m", 3);
    dic.put("a", 20);
    EXPECT_EQ(dic.keys(), (std::vector<std::string>{"z", "a", "m"}));
    EXPECT_EQ(dic.values(), (std::vector<int>{1, 20, 3}));
}

TEST(DictionaryTest, D05_GetMissingKeyThrows){
    Dictionary<std::string, int> dic;
    dic.put("x", 1);
    const Dictionary<std::string, int>& constante = dic;
    EXPECT_THROW(dic.get("y"), KeyNotFoundException);
    EXPECT_THROW(constante.get("y"), KeyNotFoundException);
}

TEST(DictionaryTest, D06_GetAllowsModifyingValue){
    Dictionary<std::string, int> dic;
    dic.put("x", 1);
    dic.get("x") = 100;
    EXPECT_EQ(dic.get("x"), 100);
}

TEST(DictionaryTest, D07_GetOrReturnsDefaultWithoutInserting){
    Dictionary<std::string, int> dic;
    dic.put("x", 1);
    EXPECT_EQ(dic.getOr("x", -1), 1);
    EXPECT_EQ(dic.getOr("y", -1), -1);
    EXPECT_FALSE(dic.contains("y"));
    EXPECT_EQ(dic.size(), 1u);
}

TEST(DictionaryTest, D08_ContainsReflectsPutAndRemove){
    Dictionary<std::string, int> dic;
    EXPECT_FALSE(dic.contains("x"));
    dic.put("x", 1);
    EXPECT_TRUE(dic.contains("x"));
    dic.remove("x");
    EXPECT_FALSE(dic.contains("x"));
}

TEST(DictionaryTest, D09_RemoveReturnsWhetherKeyExisted){
    Dictionary<std::string, int> dic;
    dic.put("a", 1);
    dic.put("b", 2);
    dic.put("c", 3);
    EXPECT_TRUE(dic.remove("b"));
    EXPECT_FALSE(dic.remove("b"));
    EXPECT_FALSE(dic.remove("nunca"));
    EXPECT_EQ(dic.size(), 2u);
    EXPECT_EQ(dic.keys(), (std::vector<std::string>{"a", "c"}));
    EXPECT_THROW(dic.get("b"), KeyNotFoundException);
}

TEST(DictionaryTest, D10_RemoveThenReinsertGoesToEnd){
    Dictionary<std::string, int> dic;
    dic.put("a", 1);
    dic.put("b", 2);
    dic.put("c", 3);
    dic.remove("a");
    dic.put("a", 10);
    EXPECT_EQ(dic.keys(), (std::vector<std::string>{"b", "c", "a"}));
    EXPECT_EQ(dic.get("a"), 10);
}

TEST(DictionaryTest, D11_BracketInsertsDefaultValue){
    Dictionary<std::string, int> dic;
    EXPECT_EQ(dic["nuevo"], 0);
    EXPECT_TRUE(dic.contains("nuevo"));
    dic["contador"] += 5;
    dic["contador"] += 5;
    EXPECT_EQ(dic.get("contador"), 10);
    EXPECT_EQ(dic.keys(), (std::vector<std::string>{"nuevo", "contador"}));
}

TEST(DictionaryTest, D12_BracketOnExistingKeyDoesNotInsert){
    Dictionary<std::string, int> dic;
    dic.put("x", 1);
    dic["x"] = 7;
    EXPECT_EQ(dic.size(), 1u);
    EXPECT_EQ(dic.get("x"), 7);
}

TEST(DictionaryTest, D13_KeysValuesItemsInInsertionOrder){
    Dictionary<std::string, int> dic;
    dic.put("z", 26);
    dic.put("a", 1);
    dic.put("m", 13);
    EXPECT_EQ(dic.keys(), (std::vector<std::string>{"z", "a", "m"}));
    EXPECT_EQ(dic.values(), (std::vector<int>{26, 1, 13}));
    EXPECT_EQ(dic.items(), (Pares{{"z", 26}, {"a", 1}, {"m", 13}}));
}

TEST(DictionaryTest, D14_SortedItemsOrdersByKey){
    Dictionary<std::string, int> dic;
    dic.put("z", 26);
    dic.put("a", 1);
    dic.put("m", 13);
    EXPECT_EQ(dic.sortedItems(), (Pares{{"a", 1}, {"m", 13}, {"z", 26}}));
    EXPECT_EQ(dic.keys(), (std::vector<std::string>{"z", "a", "m"}));
}

TEST(DictionaryTest, D15_IterationInInsertionOrderAndModifyValues){
    Dictionary<std::string, int> dic;
    dic.put("b", 2);
    dic.put("a", 1);
    std::vector<std::string> llaves;
    for (auto& item : dic){
        llaves.push_back(item.first);
        item.second *= 10;
    }
    EXPECT_EQ(llaves, (std::vector<std::string>{"b", "a"}));
    EXPECT_EQ(dic.get("b"), 20);
    EXPECT_EQ(dic.get("a"), 10);
}

TEST(DictionaryTest, D16_PrintFormat){
    Dictionary<std::string, int> dic;
    std::ostringstream vacio;
    vacio << dic;
    EXPECT_EQ(vacio.str(), "{}");

    dic.put("x", 1);
    std::ostringstream uno;
    uno << dic;
    EXPECT_EQ(uno.str(), "{x: 1}");

    dic.put("y", 2);
    std::ostringstream varios;
    varios << dic;
    EXPECT_EQ(varios.str(), "{x: 1, y: 2}");
}

TEST(DictionaryTest, D17_ClearEmptiesDictionary){
    Dictionary<std::string, int> dic;
    dic.put("a", 1);
    dic.put("b", 2);
    dic.clear();
    EXPECT_TRUE(dic.isEmpty());
    EXPECT_FALSE(dic.contains("a"));
    EXPECT_THROW(dic.get("a"), KeyNotFoundException);
    dic.put("c", 3);
    EXPECT_EQ(dic.keys(), (std::vector<std::string>{"c"}));
}

TEST(DictionaryTest, D18_SingleElement){
    Dictionary<int, std::string> dic;
    dic.put(1, "uno");
    EXPECT_EQ(dic.size(), 1u);
    EXPECT_EQ(dic.get(1), "uno");
    EXPECT_TRUE(dic.remove(1));
    EXPECT_TRUE(dic.isEmpty());
}

TEST(DictionaryTest, D19_CopyIsIndependent){
    Dictionary<std::string, int> original;
    original.put("a", 1);
    original.put("b", 2);
    Dictionary<std::string, int> copia(original);
    copia.put("a", 100);
    copia.remove("b");
    copia.put("c", 3);
    EXPECT_EQ(original.items(), (Pares{{"a", 1}, {"b", 2}}));
    EXPECT_EQ(copia.items(), (Pares{{"a", 100}, {"c", 3}}));
}

TEST(DictionaryTest, D20_CopyAssignmentIsIndependent){
    Dictionary<std::string, int> original;
    original.put("a", 1);
    Dictionary<std::string, int> destino;
    destino.put("viejo", 0);
    destino = original;
    destino.put("a", 50);
    EXPECT_EQ(original.get("a"), 1);
    EXPECT_EQ(destino.items(), (Pares{{"a", 50}}));
    Dictionary<std::string, int>& mismo = destino;
    destino = mismo;
    EXPECT_EQ(destino.items(), (Pares{{"a", 50}}));
}

TEST(DictionaryTest, D21_MoveLeavesSourceEmpty){
    Dictionary<std::string, int> fuente;
    fuente.put("a", 1);
    fuente.put("b", 2);
    Dictionary<std::string, int> movido(std::move(fuente));
    EXPECT_TRUE(fuente.isEmpty());
    EXPECT_EQ(movido.items(), (Pares{{"a", 1}, {"b", 2}}));
    movido.remove("a");
    EXPECT_EQ(movido.keys(), (std::vector<std::string>{"b"}));

    Dictionary<std::string, int> asignado;
    asignado.put("viejo", 0);
    asignado = std::move(movido);
    EXPECT_TRUE(movido.isEmpty());
    EXPECT_EQ(asignado.items(), (Pares{{"b", 2}}));
}

TEST(DictionaryTest, D22_MoveOnlyValues){
    Dictionary<int, std::unique_ptr<int>> dic;
    dic.put(1, std::make_unique<int>(10));
    auto puntero = std::make_unique<int>(20);
    dic.put(2, std::move(puntero));
    EXPECT_EQ(puntero, nullptr);
    dic.put(1, std::make_unique<int>(11));
    EXPECT_EQ(dic.size(), 2u);
    EXPECT_EQ(*dic.get(1), 11);
    EXPECT_EQ(*dic.get(2), 20);
}

TEST(DictionaryTest, D23_TenThousandElements){
    Dictionary<int, int> dic;
    for (int i = 0; i < 10000; ++i){
        dic.put(i, i * i);
    }
    EXPECT_EQ(dic.size(), 10000u);
    for (int i = 0; i < 10000; i += 2){
        ASSERT_TRUE(dic.remove(i));
    }
    EXPECT_EQ(dic.size(), 5000u);
    int esperado = 1;
    for (const auto& item : dic){
        ASSERT_EQ(item.first, esperado);
        ASSERT_EQ(item.second, esperado * esperado);
        esperado += 2;
    }
}

TEST(DictionaryTest, D24_ExceptionIsStructureException){
    Dictionary<std::string, int> dic;
    EXPECT_THROW(dic.get("x"), StructureException);
    EXPECT_THROW(dic.get("x"), std::runtime_error);
    try{
        dic.get("x");
        FAIL() << "get() debio lanzar";
    } catch (const KeyNotFoundException& e){
        EXPECT_STREQ(e.what(), "llave no encontrada");
    }
}
