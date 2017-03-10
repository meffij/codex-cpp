#include "codex.h"
#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <algorithm>

inline constexpr bool isActivatedAbility(Ability a) {
  switch (a) {
    default: return false; break;
  }
};

inline constexpr Color colorOfSpec(Spec s) {
  switch (s) {
    case Spec::Bashing:
    case Spec::Finesse:
      return Color::Neutral;
    default:
      return Color::Neutral;
  };
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

Deck starterDeck(Player p, std::function<int()> inc) {
  Deck d;
  d.add(CardInstance(CodexCardData::TimelyMessenger, inc(), p));
  d.add(CardInstance(CodexCardData::Tenderfoot, inc(), p));
  d.add(CardInstance(CodexCardData::OlderBrother, inc(), p));
  d.add(CardInstance(CodexCardData::BrickThief, inc(), p));
  d.add(CardInstance(CodexCardData::HelpfulTurtle, inc(), p));
  d.add(CardInstance(CodexCardData::GranfalloonFlagbearer, inc(), p));
  d.add(CardInstance(CodexCardData::FruitNinja, inc(), p));
  d.add(CardInstance(CodexCardData::Spark, inc(), p));
  d.add(CardInstance(CodexCardData::Bloom, inc(), p));
  d.add(CardInstance(CodexCardData::Wither, inc(), p));
  return d;
};

GameData GameData::SingleSpecGame(ActionManager am, Spec p1spec, Spec p2spec) {
  GameData g {am};
  g.turn = 1;
  g.activePlayer = Player::Player1;
  g.currentPhase = Phase::Tech;
  g.playerData(Player::Player1)->gold = 0;
  g.playerData(Player::Player1)->workers = 4;
  g.playerData(Player::Player2)->gold = 0;
  g.playerData(Player::Player2)->workers = 5;
  g.setupSingleSpec(p1spec, Player::Player1);
  g.setupSingleSpec(p2spec, Player::Player2);
  return g;
};

void GameData::setupSingleSpec(Spec spec, Player player) {
  PlayerData* p = playerData(player);
  p->baseHealth = 20;
  int heroCUID = cuidgen.next().asCUID();
  p->heroSlots[0].cuid = heroCUID;
  p->heroSlots[0].turnsUntilPlayable = 0;
  if (spec == Spec::Bashing || spec == Spec::Finesse) {
    Deck d = starterDeck(player, [this](){ return cuidgen.next().asCUID(); });
    p->deck = d;
  } else {
    return;
  }
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
  TimestampManager t;
  Deck d = starterDeck(Player::Player1, [&t](){ return t.next().asCUID(); });
  auto b = d.begin();
  auto e = d.end();
  vector<int> cuids;
  std::for_each(b, e, [&cuids](auto card){ cuids.push_back(card.CUID()); });
  for(int i = 0; i < 10; i++) {
    CHECK_EQUAL(cuids[i], i + 1);
  };
};

TEST(starter) {
  ActionManager am {};
  GameData g = GameData::SingleSpecGame(am, Spec::Bashing, Spec::Finesse);
  // Deck d = g.players[0].deck;
};

TEST(herodata) {
  CHECK_EQUAL(4, CodexCardData::Troq.midband_HP);
  CHECK_EQUAL(4, CodexCardData::River.midband_HP);
  CHECK_EQUAL(2, CodexCardData::River.midband_ATK);
};

TEST(actionManager) {
  ActionManager am {};
  Action a = am.nextAction();
  CHECK(a.which() == 0); // EndGameAction
  vector<Action> act {DrawCardIndexAction{Player::Player1, 2},
                    DrawCardCUIDAction{Player::Player2, 3}};
  ActionManager am2 { act };
  CHECK(am2.nextAction().which() == 1);
  CHECK(am2.nextAction().which() == 2);
  CHECK(am2.nextAction().which() == 0);
  Action a2 = boost::get<EndGameAction>(am2.nextAction());
};

#endif

// TODOS
// change from int to uint32_t cause emscripten
// basically don't use anything that isn't explicitly sized
