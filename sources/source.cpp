// Copyright 2019 IliaNekleenov <neiluha@mail.ru>

#include <header.hpp>

Json::Json() {}

Json::Json(const std::string& s) {
  size_t i = 0;
  while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
    i++;
  }
  if (s[i] == '{') {
    std::unordered_map<std::string, std::any> new_data;
    i++;
    while (s[i] != '}') {
      std::string key = "";
      std::any value;
      while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
        i++;
      }
      if (s[i] == '"') {
        i++;
        while (s[i] != '"') {
          key += s[i];
          i++;
        }
        i++;
      }
      while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
        i++;
      }
      if (s[i] == ':') {
        i++;
      }
      while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
        i++;
      }
      if (s[i] == '"') {
        i++;
        std::string str_val;
        while (s[i] != '"') {
          str_val += s[i];
          i++;
        }
        value = str_val;
        i++;
      } else if (s[i] == '[') {
        size_t count = 1, flag = 1;
        while (flag > 0) {
          if (s[i + count] == '[') {
            flag++;
          }
          if (s[i + count] == ']') {
            flag--;
          }
          count++;
        }
        value = Json(s.substr(i, count));
        i += count;
      } else if (s[i] == '{') {
        size_t count = 1, flag = 1;
        while (flag > 0) {
          if (s[i + count] == '{') {
            flag++;
          }
          if (s[i + count] == '}') {
            flag--;
          }
          count++;
        }
        value = Json(s.substr(i, count));
        i += count;
      }
      int left = 0;
      double right = 0;
      bool minus = false;
      if (s[i] == '-') {
        minus = true;
        i++;
      }
      if (s[i] >= '0' && s[i] <= '9') {
        while (s[i] >= '0' && s[i] <= '9') {
          left = 10 * left + s[i] - 48;
          i++;
        }
        if (s[i] == '.') {
          i++;
          double pw = 10;
          while (s[i] >= '0' && s[i] <= '9') {
            right += (s[i] - 48) / pw;
            i++;
            pw *= 10;
          }
          right += left;
          if (minus) {
            value = -right;
          } else {
            value = right;
          }
        }
        if (right == 0) {
          value = left;
        }
      } else {
        if (minus) {
          throw std::invalid_argument("not number after '-'");
        }
      }
      if (s[i] == 't') {
        if (s.substr(i, 4) == "true") {
          value = true;
          i += 4;
        } else {
          throw std::invalid_argument("wrong literal");
        }
      } else if (s[i] == 'f') {
        if (s.substr(i, 5) == "false") {
          value = false;
          i += 5;
        } else {
          throw std::invalid_argument("wrong literal");
        }
      } else if (s[i] == 'n') {
        if (s.substr(i, 4) == "null") {
          i += 4;
        } else {
          throw std::invalid_argument("wrong literal");
        }
      }
      new_data.insert(std::make_pair(key, value));
      while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
        i++;
      }
      if (s[i] == ',') {
        i++;
      } else if (s[i] == '}') {
        break;
      } else {
        throw std::invalid_argument("wrong syntax");
      }
      while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
        i++;
      }
    }
    data = new_data;
  } else if (s[i] == '[') {
    std::vector<std::any> arr;
    i++;
    while (true) {
      std::any item;
      int left = 0;
      double right = 0;
      bool minus = false;
      while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
        i++;
      }
      if (s[i] == '-') {
        minus = true;
        i++;
      }
      if (s[i] >= '0' && s[i] <= '9') {
        while (s[i] >= '0' && s[i] <= '9') {
          left = 10 * left + s[i] - 48;
          i++;
        }
        if (s[i] == '.') {
          i++;
          double pw = 10;
          while (s[i] >= '0' && s[i] <= '9') {
            right += (s[i] - 48) / pw;
            i++;
            pw *= 10;
          }
          right += left;
          if (minus) {
            item = -right;
          } else {
            item = right;
          }
        }
        if (right == 0) {
          item = left;
        }
      } else {
        if (minus) {
          throw std::invalid_argument("not number after '-'");
        }
      }
      if (s[i] == 't') {
        if (s.substr(i, 4) == "true") {
          item = true;
          i += 4;
        } else {
          throw std::invalid_argument("wrong literal");
        }
      } else if (s[i] == 'f') {
        if (s.substr(i, 5) == "false") {
          item = false;
          i += 5;
        } else {
          throw std::invalid_argument("wrong literal");
        }
      } else if (s[i] == 'n') {
        if (s.substr(i, 4) == "null") {
          i += 4;
        } else {
          throw std::invalid_argument("wrong literal");
        }
      } else if (s[i] == '"') {
        i++;
        std::string str_val = "";
        while (s[i] != '"') {
          str_val += s[i];
          i++;
        }
        item = str_val;
        i++;
      } else if (s[i] == '{') {
        size_t count = 1, flag = 1;
        while (flag > 0) {
          if (s[i + count] == '{') {
            flag++;
          }
          if (s[i + count] == '}') {
            flag--;
          }
          count++;
        }
        item = Json(s.substr(i, count));
        i += count;
      } else if (s[i] == '[') {
        size_t count = 1, flag = 1;
        while (flag > 0) {
          if (s[i + count] == '[') {
            flag++;
          }
          if (s[i + count] == ']') {
            flag--;
          }
          count++;
        }
        item = Json(s.substr(i, count));
        i += count;
      }
      arr.push_back(item);
      while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
        i++;
      }
      if (s[i] == ',') {
        i++;
        continue;
      } else if (s[i] == ']') {
        i++;
        break;
      } else {
        throw std::invalid_argument("wrong syntax");
      }
    }
    data = arr;
  } else {
    throw std::invalid_argument("wrong syntax");
  }
}

bool Json::is_array() const {
  return (data.type() == typeid(std::vector<std::any>));
}

bool Json::is_object() const {
  return (data.type() == typeid(std::unordered_map<std::string, std::any>));
}

std::any& Json::operator[](const std::string& key) {
  if (is_object()) {
    auto obj = std::any_cast<std::unordered_map<std::string, std::any>>(data);
    auto it = obj.begin();
    while (it != obj.end()) {
      if (it->first == key) {
        return it->second;
      }
      it++;
    }
    throw std::invalid_argument("invalid key");
  }
  throw std::invalid_argument("invalid key");
}

std::any& Json::operator[](int index) {
  if (is_array()) {
    auto obj = std::any_cast<std::vector<std::any>>(data);
    return obj[index];
  } else {
    throw std::invalid_argument("invalid key");
  }
}

Json Json::parse(const std::string& s) { return Json(s); }

Json Json::parseFile(const std::string& path_to_file) {
  std::ifstream fin(path_to_file);
  std::string json = "";
  while (fin) {
    std::string str;
    getline(fin, str);
    json += str;
  }
  fin.close();
  return Json(json);
}

Json Json::market(const std::string& path_to_file) {
  Json first(Json::parseFile(path_to_file));
  if (first.is_array()) {
    auto vec = std::any_cast<std::vector<std::any>>(first.data);
    if (vec.size() != 3) {
      throw std::invalid_argument("wrong format");
    }
    auto ticker = std::any_cast<Json>(vec[0]);
    auto id = std::any_cast<Json>(vec[1]);
    auto description = std::any_cast<Json>(vec[2]);
    auto v_ticker = std::any_cast<std::vector<std::any>>(ticker.data);
    auto v_id = std::any_cast<std::vector<std::any>>(id.data);
    auto v_description = std::any_cast<std::vector<std::any>>(description.data);
    size_t n = v_ticker.size();
    if (n == v_id.size() && n == v_description.size()) {
      Json res;
      res.data = std::vector<std::any>(n);
      for (size_t i = 0; i < n; i++) {
        std::unordered_map<std::string, std::any> item;
        item.insert(std::make_pair("ticker", v_ticker[i]));
        item.insert(std::make_pair("id", v_id[i]));
        item.insert(std::make_pair("description", v_description[i]));
        Json new_item;
        new_item.data = item;
        res[i] = new_item;
      }
      return res;
    } else {
      throw std::invalid_argument("wrong format");
    }
  } else {
    throw std::invalid_argument("wrong format");
  }
}
