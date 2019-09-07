// Copyright 2019 IliaNekleenov <neiluha@mail.ru>

#include <gtest/gtest.h>

#include <header.hpp>

TEST(JSON_FORMAT, init) {
  std::string json = "";
  std::ifstream fin("..//txt//Json.txt");
  if (!fin) std::cout << "not found";
  getline(fin, json, '*');
  fin.close();
  std::cout << json;
  Json object = Json::parse(json);
  EXPECT_EQ(std::any_cast<std::string>(object["lastname"]), "Ivanov");
  EXPECT_EQ(std::any_cast<bool>(object["islegal"]), false);
  EXPECT_EQ(std::any_cast<int>(object["age"]), 25);
  EXPECT_TRUE(object.is_object());
  EXPECT_FALSE(object.is_array());

  auto marks = std::any_cast<Json>(object["marks"]);
  EXPECT_EQ(std::any_cast<int>(marks[0]), 4);
  EXPECT_EQ(std::any_cast<int>(marks[1]), 5);
  EXPECT_FALSE(marks.is_object());
  EXPECT_TRUE(marks.is_array());

  auto address = std::any_cast<Json>(object["address"]);
  EXPECT_EQ(std::any_cast<std::string>(address["city"]), "Moscow");
  EXPECT_EQ(std::any_cast<std::string>(address["street"]), "Vozdvijenka");
  EXPECT_TRUE(address.is_object());
  EXPECT_FALSE(address.is_array());
}

TEST(JSON_FORMAT, trade) {
  auto trade_obj = Json::market("..//txt//Market.txt");
  EXPECT_EQ(
      std::any_cast<std::string>(std::any_cast<Json>(trade_obj[0])["ticker"]),
      "Si-9.15");
  EXPECT_EQ(std::any_cast<int>(std::any_cast<Json>(trade_obj[1])["id"]),
            100027);
  EXPECT_EQ(std::any_cast<std::string>(
                std::any_cast<Json>(trade_obj[2])["description"]),
            "Futures contract for GAZPROM shares");
  EXPECT_FALSE(trade_obj.is_object());
  EXPECT_TRUE(trade_obj.is_array());
}
