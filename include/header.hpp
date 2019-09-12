// Copyright 2019 IliaNekleenov <neiluha@mail.ru>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <any>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class Json {
  std::any data;

 public:
  Json();

  explicit Json(const std::string& s);

  bool is_array() const;

  bool is_object() const;

  std::any& operator[](const std::string& key);

  std::any& operator[](int index);

  static Json parse(const std::string& s);

  static Json parseFile(const std::string& path_to_file);

  static Json market(const std::string& path_to_file);
};

#endif  // INCLUDE_HEADER_HPP_
