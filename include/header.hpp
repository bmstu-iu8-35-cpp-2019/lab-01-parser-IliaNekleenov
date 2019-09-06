// Copyright 2019 IliaNekleenov <neiluha@mail.ru>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <any>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

class Json {
  std::any data;

 public:
  Json();
  // ����������� �� ������, ���������� Json-������.
  Json(const std::string& s);

  // ����� ���������� true, ���� ������ ��������� �������� � ���� JSON-������.
  // ����� false.
  bool is_array() const;
  // ����� ���������� true, ���� ������ ��������� �������� � ���� JSON-������.
  // ����� false.
  bool is_object() const;

  // ����� ���������� �������� �� ����� key, ���� ��������� ��������
  // JSON-��������. �������� ����� ����� ���� �� ��������� �����: Json,
  // std::string, double, bool ��� ���� ������. ���� ��������� ��������
  // JSON-��������, ������������ ����������.
  std::any& operator[](const std::string& key);

  // ����� ���������� �������� �� ������� index, ���� ��������� ��������
  // JSON-��������. �������� ����� ����� ���� �� ��������� �����: Json,
  // std::string, double, bool ��� ���� ������. ���� ��������� ��������
  // JSON-��������, ������������ ����������.
  std::any& operator[](int index);

  // ����� ���������� ������ ������ Json �� ������, ���������� Json-������.
  static Json parse(const std::string& s);

  // ����� ���������� ������� ������ Json �� �����, ����������� Json-������ �
  // ��������� �������.
  static Json parseFile(const std::string& path_to_file);

  static Json market(const std::string& s);
};

#endif  // INCLUDE_HEADER_HPP_
