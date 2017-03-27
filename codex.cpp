#include "codex.h"
#include "codex_effect.h"
#include "codex_datatypes.h"
#include "codex_card_data.h"
#include "make_visitor.hpp"
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

optional<CardInstance> Deck::draw(uint32_t which) {
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
  return {};
};

Deck starterDeck(Player p, std::function<uint32_t()> inc) {
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

ProcessResult GameData::calculateDerivedState() {
  class layer1visitor : boost::static_visitor<> {
    /*
    template <typename T>
    void operator()(T& t) { return; };
    
    template <>
    void operator()(UnitEntityData& d) { return; };
    */
  };
  return NoError{};
}

ProcessResult GameData::processSBA() {
    // if 2 legendary things with same name, discard one
    // tokens not in the future or in play are trashed
    // heroes in the discard go to the command zone and go on cooldown
    // players with no base lose the game
    // effects that have expired stop applying -- ???
    // cancel +1/+1 and -1/-1 runes
    // deathtouch or lethal damage kills buildings/units/heroes,
    //  hero death bonus levels trigger
    // abilities that trigger on a condition trigger (i.e. Hardened Mox)
  return NoError{};
}

ProcessResult GameData::processEffectQueue() {
  if (q.empty()) {
    return NoError{};
  } else {
    return UnimplementedError {};
  }
}

class mainphase_action_visitor 
  : public boost::static_visitor<ProcessResult> {
public:
  GameData* gd;
  mainphase_action_visitor(GameData* gdd) : gd(gdd) {};

  ProcessResult operator()(EndOfCurrentActions& e) {
    return NoError{};
  };
  ProcessResult operator()(ForfeitAction& f) {
    if (f.p == +Player::Player1) {
      gd->winner = +Player::Player2;
      return NoError{};
    } else {
      gd->winner = +Player::Player1;
      return NoError{};
    }
  };
  ProcessResult operator()(DrawCardIndexAction& d) {
    PlayerData* pd = gd->playerData(d.player);
    optional<CardInstance> c = pd->deck.draw(d.which);
    if (c) {
      // move CardInstance to hand
      pd->hand.push_back(*c);
      return gd->processActions();
    } else {
      return DrawCardIndexCardNotFound {};
    }
  };
  ProcessResult operator()(DrawCardCUIDAction& d) {
    return UnimplementedError{};
  };
  ProcessResult operator()(MakeWorker& m) {
    PlayerData* p = gd->playerData(gd->activePlayer);
    Hand h = p->hand;
    auto c = std::find_if(h.begin(), h.end(), 
        [m](CardInstance c){ return c.CUID() == m.cuid; });
    if (c == h.end()) {
      return MakeWorkerCardNotFound {};
    }
    if (p->gold < 1) {
      return MakeWorkerNotEnoughGold {};
    }
    p->gold -= 1;
    p->workers++;
    h.erase(c);
    return gd->processActions();
  };

  ProcessResult playCard(CardInstance c) {
    PlayerData* p = gd->playerData(gd->activePlayer);
    Hand h = p->hand;
    if (p->gold < c.getCost()) {
      return PlayCardFromHandNotEnoughGold {};
    }
    p->gold -= c.getCost();
    // create new entity
    Entity e {
      gd->tsGen.next(),
      gd->activePlayer,
      {},
      UnitEntityData {
        c, 0, 0, {}, 0
      },
      false
    };
    return NoError {};
  };

  ProcessResult operator()(PlayCardFromHand& a) {
    PlayerData* p = gd->playerData(gd->activePlayer);
    Hand h = p->hand;
    auto ci = std::find_if(h.begin(), h.end(), 
        [a](CardInstance c){ return c.CUID() == a.cuid; });
    if (ci == h.end()) {
      return PlayCardFromHandCardNotFound {};
    }
    CardInstance c = *ci;
    return playCard(c);
  };
  ProcessResult operator()(PlayCardFromHandWithName& a) {
    PlayerData* p = gd->playerData(gd->activePlayer);
    Hand h = p->hand;
    auto ci = std::find_if(h.begin(), h.end(), 
        [a](CardInstance c){ return c.name() == a.name; });
    if (ci == h.end()) {
      return PlayCardFromHandCardNotFound {};
    }
    CardInstance c = *ci;
    return playCard(c);
  };
  ProcessResult operator()(EndMainPhase& e) {
    gd->currentPhase = Phase::DiscardDraw;
    return gd->processActions();
  };
  ProcessResult operator()(TechCard& t) {
    return IncorrectTimingError {};
  };
  ProcessResult operator()(ChooseTarget& t) {
    return IncorrectTimingError {};
  };
  /*
  template <typename T>
  ProcessResult operator()(T& a) {
    return UnimplementedError {};
  };
  */
};

/*
class unimplemented_action_visitor 
  : public boost::static_visitor<ProcessResult> {
public:
  template <typename T>
  ProcessResult operator()(T& a) {
    return UnimplementedError {};
  };
};
*/

auto test_visitor = make_visitor (
    [](auto e){ return; }
    );

/*
class draw_visitor : public boost::static_visitor<ProcessResult> {
public:
  GameData* gd;
  draw_visitor(GameData* gdd) : gd(gdd) {};
  ProcessResult operator()(DrawCardIndexAction& d) {
    PlayerData* pd = gd->playerData(d.player);
    optional<CardInstance> c = pd->deck.draw(d.which);
    if (c) {
      // move CardInstance to hand
      pd->hand.push_back(*c);
      return gd->processActions();
    } else {
      return DrawCardIndexCardNotFound {};
    }
  };
  ProcessResult operator()(DrawCardCUIDAction& d) {
    PlayerData* pd = gd->playerData(d.player);
    Hand h = pd->hand;
    Deck deck = pd->deck;
    auto c = std::find_if(deck.begin(), deck.end(), [d](const CardInstance& c)
        { return c.CUID() == d.cuid; });
    if (c == deck.end()) {
      return DrawCardCUIDNotFound {};
    }
    deck.draw(c->CUID());


    return NoError {};
  };
  template <typename T>
  ProcessResult operator()(T& t) { return DrawCardIncorrectAction {}; };
};
*/

ProcessResult GameData::drawCard() {
  Action a = am.nextAction();
  return boost::apply_visitor( make_visitor<ProcessResult>(
    [this](DrawCardIndexAction& d) -> ProcessResult {
      PlayerData* pd = playerData(d.player);
      optional<CardInstance> c = pd->deck.draw(d.which);
      if (c) {
        // move CardInstance to hand
        pd->hand.push_back(*c);
        return processActions();
      } else {
        return DrawCardIndexCardNotFound {};
      }
    },
    [this](DrawCardCUIDAction& d) -> ProcessResult {
      PlayerData* pd = playerData(d.player);
      Hand h = pd->hand;
      Deck deck = pd->deck;
      auto c = std::find_if(deck.begin(), deck.end(), [d](const CardInstance& c)
          { return c.CUID() == d.cuid; });
      if (c == deck.end()) {
        return DrawCardCUIDNotFound {};
      }
      deck.draw(c->CUID());


      return NoError {};
    },
    [this](auto e) -> ProcessResult { return IncorrectTimingError {}; }
  ), a);
};

ProcessResult GameData::processActions() {
  actionCallback(this);
  if (!q.empty()) {
    // process action queue
    return UnimplementedError {};
  }
  if (currentPhase != Phase::Main) {
    // process stuff till we get there
    if (currentPhase == Phase::Tech) {
      if (turn > 2) {
        // process tech action
      }
      currentPhase = Phase::Ready;
      return processActions();
    }
    if (currentPhase == Phase::Ready) {
      // untap stuff
      currentPhase = Phase::Upkeep;
      return processActions();
    }
    if (currentPhase == Phase::Upkeep) {
      // trigger upkeep stuff
      auto pd = playerData(activePlayer);
      pd->gold += pd->workers;
      currentPhase = Phase::Main;
      return processActions();
    }
    if (currentPhase == Phase::DiscardDraw) {
      // do discard-draw
      Hand h = playerData(activePlayer)->hand;
      std::for_each(h.begin(), h.end(), [this](CardInstance c){ 
          playerData(activePlayer)->discard.push_back(c);
      });
      int s = h.size();
      s += 2;
      if (s > 5) { s = 5; }
      h.clear();
      // draw s cards
      currentPhase = Phase::End;
      return processActions();
    }
    if (currentPhase == Phase::End) {
      // do end step stuff
      currentPhase = Phase::Tech;
      if (activePlayer == +Player::Player1) {
        activePlayer = Player::Player2;
      } else {
        activePlayer = Player::Player1;
      }
      return processActions();
    }
  }
  mainphase_action_visitor av = mainphase_action_visitor(this);
  auto na = am.nextAction();
  return boost::apply_visitor(av, na);
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
  // draw p1 cards and p2 cards 
  // g.printDeck(Player::Player1);
  // g.printDeck(Player::Player2);
  g.processActions();
  return g;
};

void GameData::setupSingleSpec(Spec spec, Player player) {
  PlayerData* p = playerData(player);
  p->baseHealth = 20;
  CUID heroCUID = cuidgen.next().asCUID();
  p->heroSlots[0].cuid = heroCUID;
  p->heroSlots[0].turnsUntilPlayable = 0;
  if (spec == +Spec::Bashing || spec == +Spec::Finesse) {
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
  vector<CUID> cuids;
  std::for_each(b, e, [&cuids](auto card){ cuids.push_back(card.CUID()); });
  for(int i = 0; i < 10; i++) {
    CHECK_EQUAL(cuids[i], i + 1);
  };
};

TEST(forfeit) {
  vector<Action> av { ForfeitAction {Player::Player1} };
  ActionManager am { av };
  GameData g { am };
  g.processActions();
  CHECK(g.hasWinner());
  CHECK(*(g.getWinner()) == +Player::Player2);
};

TEST(starter) {
  ActionManager am {};
  GameData g = GameData::SingleSpecGame(am, Spec::Bashing, Spec::Finesse);
  CHECK(g.getCurrentPhase() == Phase::Main);
  CHECK(g.playerWorkers(Player::Player1) == 4);
  CHECK(g.playerWorkers(Player::Player2) == 5);

  // end turn
  vector<Action> av { EndMainPhase {} };
  am = ActionManager(av);
  g.setActionManager(am);
  g.processActions();
  CHECK(g.getCurrentPhase() == Phase::Main);
  CHECK(g.getActivePlayer() == +Player::Player2);
  // make worker
  av = { DrawCardIndexAction { g.getActivePlayer(), 0 } };
  am = ActionManager(av);
  g.setActionManager(am);
  auto res = g.processActions();
  CHECK_EQUAL(0, res.which());
  CHECK(g.getActivePlayer() == +Player::Player2);
  av = { MakeWorker { 13 } };
  g.setActionManager(av);
  res = g.processActions();
  CHECK_EQUAL(6, g.playerWorkers(Player::Player2));
  CHECK_EQUAL(0, res.which());
  CHECK_EQUAL(g.getActivePlayer(), +Player::Player2);
  // play unit
  // play hero
  // play spell
  // attack base
  // attack patroller
  // build add-on
  // destroy add-on, base takes 2
  // destroy hero, opposing hero gains 2 levels
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
  boost::get<DrawCardIndexAction>(am2.nextAction());
  boost::get<DrawCardCUIDAction>(am2.nextAction());
  boost::get<EndOfCurrentActions>(am2.nextAction());
};

#endif

// TODOS
// change from int to uint32_t cause emscripten
// basically don't use anything that isn't explicitly sized
