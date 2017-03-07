#include "codex.h"
#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <algorithm>

inline constexpr bool isActivatedAbility(Ability a) {
  switch (a) {
    default: return false; break;
  }
};

optional<CardInstance> Deck::draw(int which) {
  if (topKnown) {
    CardInstance temp = d[0];
    topKnown = false;
    d.erase(d.begin());
    return temp;
  }
  if (which < d.size()) {
    CardInstance temp = d[which];
    d.erase(d.begin() + which);
    return temp;
  }
  return optional<CardInstance>();
};

Deck starterDeck() {
  Deck d;
  d.add(CardInstance(CodexCardData::TimelyMessenger, 0, Player::Player1));
  d.add(CardInstance(CodexCardData::Tenderfoot, 1, Player::Player1));
  d.add(CardInstance(CodexCardData::OlderBrother, 2, Player::Player1));
  d.add(CardInstance(CodexCardData::BrickThief, 3, Player::Player1));
  d.add(CardInstance(CodexCardData::HelpfulTurtle, 4, Player::Player1));
  d.add(CardInstance(CodexCardData::GranfalloonFlagbearer, 5, Player::Player1));
  d.add(CardInstance(CodexCardData::FruitNinja, 6, Player::Player1));
  d.add(CardInstance(CodexCardData::Spark, 7, Player::Player1));
  d.add(CardInstance(CodexCardData::Bloom, 8, Player::Player1));
  d.add(CardInstance(CodexCardData::Wither, 9, Player::Player1));
  return d;
};

GameData GameData::StarterTest() {
  GameData g {};
  g.players[0];
  return g;
};

int main() {
#ifdef RUN_UNIT_TESTS
  return UnitTest::RunAllTests();
#else
  return 0;
#endif
}

#ifdef RUN_UNIT_TESTS
TEST(sanity) {
  CHECK(1 + 1 == 2);
}

TEST(q) {
  CHECK_EQUAL(1, 1);
}

TEST(deck) {
  Deck d = starterDeck();
  auto b = d.begin();
  auto e = d.end();
  vector<int> cuids;
  std::for_each(b, e, [&cuids](auto card){ cuids.push_back(card.getCUID()); });
  for(int i = 0; i < 10; i++) {
    CHECK_EQUAL(cuids[i], i);
  };
};

#endif
