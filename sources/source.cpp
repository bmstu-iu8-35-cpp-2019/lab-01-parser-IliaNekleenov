// Copyright 2019 IliaNekleenov <neiluha@mail.ru>

#include <header.hpp>

Json::Json() {}

std::string Json::parse_string(const std::string& s, size_t& i) {
  std::string str;
  i++;
  while (s[i] != '"') {
    str += s[i];
    i++;
  }
  i++;
  return str;
}

double Json::parse_number(const std::string& s, size_t& i) {
  int left = 0;
  double right = 0;
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
  }
  right += left;
  return right;
}

std::any Json::parse_special(const std::string& s, size_t& i) {
  std::any answer = NULL;
  if (s[i] == 't') {
    if (s.substr(i, 4) == "true") {
      answer = true;
      i += 4;
    } else {
      throw std::invalid_argument("wrong literal");
    }
  } else if (s[i] == 'f') {
    if (s.substr(i, 5) == "false") {
      answer = false;
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
  } else {
    throw std::invalid_argument("wrong value");
  }
  return answer;
}

void skip_empty(const std::string& s, size_t& i) {
  while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
    i++;
  }
}

size_t find_closing(const std::string& s, const size_t i, const char& opening,
                    const char& closing) {
  size_t count = 1, flag = 1;
  while (flag > 0) {
    if (s[i + count] == opening) {
      flag++;
    }
    if (s[i + count] == closing) {
      flag--;
    }
    count++;
  }
  return count;
}

std::unordered_map<std::string, std::any> Json::parse_object(
    const std::string& s) {
  std::unordered_map<std::string, std::any> new_data;
  size_t i = 0;
  i++;
  while (s[i] != '}') {
    std::string key = "";
    std::any value;
    skip_empty(s, i);
    if (s[i] == '"') {
      key = parse_string(s, i);
    }
    skip_empty(s, i);
    if (s[i] == ':') {
      i++;
    } else {
      throw std::invalid_argument("wrong syntax");
    }
    skip_empty(s, i);
    if (s[i] == '"') {
      value = parse_string(s, i);
    } else if (s[i] == '[') {
      size_t count = find_closing(s, i, '[', ']');
      value = Json(s.substr(i, count));
      i += count;
    } else if (s[i] == '{') {
      size_t count = find_closing(s, i, '{', '}');
      value = Json(s.substr(i, count));
      i += count;
    }
    bool minus = false;
    if (s[i] == '-') {
      minus = true;
      i++;
    }
    if (s[i] >= '0' && s[i] <= '9') {
      double num = parse_number(s, i);
      if (minus) {
        num = -num;
      }
      if (static_cast<double>(static_cast<int>(num)) - num == 0.) {
        value = static_cast<int>(num);
      } else {
        value = num;
      }
    } else {
      if (minus) {
        throw std::invalid_argument("not number after '-'");
      }
    }
    if (!value.has_value()) value = parse_special(s, i);
    new_data.insert(std::make_pair(key, value));
    skip_empty(s, i);
    if (s[i] == ',') {
      i++;
    } else if (s[i] == '}') {
      break;
    } else {
      throw std::invalid_argument("wrong syntax");
    }
    skip_empty(s, i);
  }
  return new_data;
}

std::vector<std::any> Json::parse_array(const std::string& s) {
  std::vector<std::any> arr;
  size_t i = 0;
  i++;
  while (true) {
    skip_empty(s, i);
    std::any item;
    if (s[i] == '"') {
      item = parse_string(s, i);
    } else if (s[i] == '{') {
      size_t count = find_closing(s, i, '{', '}');
      item = Json(s.substr(i, count));
      i += count;
    } else if (s[i] == '[') {
      size_t count = find_closing(s, i, '[', ']');
      item = Json(s.substr(i, count));
      i += count;
    }
    bool minus = false;
    if (s[i] == '-') {
      minus = true;
      i++;
    }
    if (s[i] >= '0' && s[i] <= '9') {
      double num = parse_number(s, i);
      if (minus) {
        num = -num;
      }
      if (static_cast<double>(static_cast<int>(num)) - num == 0.) {
        item = int(num);
      } else {
        item = num;
      }
    } else {
      if (minus) {
        throw std::invalid_argument("not number after '-'");
      }
    }
    if (!item.has_value()) item = parse_special(s, i);
    arr.push_back(item);
    skip_empty(s, i);
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
  return arr;
}

Json::Json(const std::string& s) {
  size_t i = 0;
  skip_empty(s, i);
  if (s[i] == '{') {
    data = parse_object(s);
  } else if (s[i] == '[') {
    data = parse_array(s);
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
    return std::any_cast<std::unordered_map<std::string, std::any>&>(data)[key];
  }
  throw std::invalid_argument("not a Json-object");
}

std::any& Json::operator[](int index) {
  if (is_array()) {
    return std::any_cast<std::vector<std::any>&>(data)[index];
  }
  throw std::invalid_argument("not a Json-array");
}

Json Json::parse(const std::string& s) { return Json(s); }

Json Json::parseFile(const std::string& path_to_file) {
  std::ifstream fin(path_to_file);
  std::string json = "";
  if (!fin) std::cout << "file not found";
  while (fin) {
    std::string str;
    getline(fin, str);
    json += str + '\n';
  }
  fin.close();
  return Json(json);
}

Json Json::market(const std::string& path_to_file) {
  Json first = Json::parseFile(path_to_file);
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
