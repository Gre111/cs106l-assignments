#include "spellcheck.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>

template <typename Iterator, typename UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred);

Corpus tokenize(std::string &source) {
  /* TODO: Implement this method */
  std::vector<std::string::iterator> v = find_all(
      source.begin(), source.end(), [](char c) { return std::isspace(c); });

  std::set<Token> tokens;
  std::transform(v.begin(), v.end() - 1, v.begin() + 1, std::inserter(tokens, tokens.end()),
      [&source](auto it1, auto it2) { return Token{source, it1, it2}; });
  std::erase_if(tokens, [](const Token& token){return token.content.empty();});
  return tokens;
}

std::set<Misspelling> spellcheck(const Corpus &source,
                                 const Dictionary &dictionary) {
  /* TODO: Implement this method */
  namespace rv = std::ranges::views;
  // step 1:
  auto view = source 
      | rv::filter([&dictionary](const Token& token)
      {return !dictionary.contains(token.content);})
      // step 2:
      | rv::transform([&dictionary](const Token& token)
      {auto view = dictionary | rv::filter([&token](const std::string& word){return levenshtein(word, token.content) == 1;}); 
        std::set<std::string> suggestions(view.begin(), view.end());
        return Misspelling{token, suggestions};})
      // step 3:
      | rv::filter([](const Misspelling& misspelling){return !misspelling.suggestions.empty();});

  return std::set<Misspelling>(view.begin(), view.end());
};

/* Helper methods */

#include "utils.cpp"