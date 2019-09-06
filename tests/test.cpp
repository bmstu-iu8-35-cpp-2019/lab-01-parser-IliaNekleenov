// Copyright 2019 IliaNekleenov <neiluha@mail.ru>

#include <gtest/gtest.h>

#include <header.hpp>

std::string json = "";
std::ifstream fin("..//Json.txt");
getline(fin, json, '*');

Json object = Json::parse(json);
EXPECT_EQ(std::any_cast<std::string>(object["lastname"]), "Ivanov");
EXPECT_EQ(std::any_cast<bool>(object["islegal"]), false);
EXPECT_EQ(std::any_cast<int>(object["age"]), 25);

auto marks = std::any_cast<Json>(object["marks"]);
EXPECT_EQ(std::any_cast<int>(marks[0]), 4);
EXPECT_EQ(std::any_cast<int>(marks[1]), 5);

auto address = std::any_cast<Json>(object["address"]);
EXPECT_EQ(std::any_cast<std::string>(address["city"]), "Moscow");
EXPECT_EQ(std::any_cast<std::string>(address["street"]), "Vozdvijenka");
