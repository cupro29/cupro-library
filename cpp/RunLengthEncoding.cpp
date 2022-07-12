#include <string>
#include <utility>
#include <vector>

std::vector<std::pair<char, int> > run_length_encoding(std::string& s) {
  std::vector<std::pair<char, int> > res;
  for (int i = 0; i < s.size(); i++) {
    if (res.size() > 0 && res.back().first == s[i]) {
      res.back().second += 1;
    } else {
      res.emplace_back(s[i], 1);
    }
  }
  return res;
}
template <class T>
std::vector<std::pair<T, int> > run_length_encoding(std::vector<T>& s) {
  std::vector<std::pair<T, int> > res;
  for (int i = 0; i < s.size(); i++) {
    if (res.size() > 0 && res.back().first == s[i]) {
      res.back().second += 1;
    } else {
      res.emplace_back(s[i], 1);
    }
  }
  return res;
}