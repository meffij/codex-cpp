// codex_datatypes.h
#pragma once
#include "codex_effect.h"

enum class Player : bool {
  Player1,
  Player2,
};

enum class Color : uint8_t {
  Neutral,
  Red,
  Green,
};

enum class CardType : uint8_t {
  Unit,
  Spell,
  Building,
  Upgrade,
};

enum class EntityType : uint8_t {
  Unit,
  Hero,
  Building,
  Upgrade,
};

enum class TechLevel : uint8_t {
  Tech0,
  Tech1,
  Tech2,
  Tech3,
};

enum class Spec : uint8_t {
  None,
  Bashing,
  Finesse,
  // more
};
enum class Subtype : uint8_t {
  Buff,
  Burn,
  CuteAnimal,
  Debuff,
  Drunkard,
  Flagbearer,
  Legendary,
  Mercenary,
  Ninja,
  Ultimate,
  Virtuoso,
};

template <class T>
class EnumManager {
  vector<T> v;
  public:
  EnumManager(std::initializer_list<T> i) : v(i) {};
  EnumManager() : v() {};
  bool contains(T i) {
    auto result = std::find(std::begin(v), std::end(v), i);
    if (result != std::end(v)) {
      return true;
    } else {
      return false;
    }
  };

  bool contains(std::function<bool(T)> f) {
    auto result = std::find_if(std::begin(v), std::end(v), f);
    if (result != std::end(v)) {
      return true;
    } else {
      return false;
    }
  };
    
};

using EffectManager = EnumManager<Effect>;
using SubtypeManager = EnumManager<Subtype>;

// these should all be static data in codex_card_data.h
struct CardData {
  const char* name;
  const uint32_t card_id;
  const uint32_t cost;
  const CardType type;
  const uint32_t ATK;
  const uint32_t HP;
  const Spec spec;
  const Color color;
  const TechLevel techlevel;
  const EffectManager effects;
  const SubtypeManager subtypes;
};

class CardInstance {
  const CardData* cd;
  CUID cuid;;
  Player owner;
public:
  CardInstance(const CardData* cdd, const CUID id, const Player o) :
    cd(cdd), cuid(id), owner(o) {};
  CardInstance(const CardData& cdd, const CUID cui, const Player o) :
    cd(&cdd), cuid(cui), owner(o) {};
  /*
  CardInstance(const CardInstance& other) :
    cd(other.cd),
    card_unique_id(other.card_unique_id),
    owner(other.owner) {};
  */
  const CardData& getCardData() { return *cd; };
  CUID CUID() { return cuid; };
  Player getOwner() { return owner; };
  uint32_t getCost() { return cd->cost; };
  std::string name() { return cd->name; };
};

struct HeroData {
  const uint32_t cost = 2;
  const Spec spec;
  const uint32_t ATK;
  const uint32_t HP;
  const uint32_t midband_level;
  const uint32_t midband_ATK;
  const uint32_t midband_HP;
  const uint32_t maxband_level;
  const uint32_t maxband_ATK;
  const uint32_t maxband_HP;
  const EffectManager effects;
  const EffectManager midband_effects;
  const EffectManager maxband_effects;
};

class HeroCardInstance {
  const HeroData* hd;
  Player owner;
  CUID card_unique_id;
public:
  const HeroData* getHeroData() const { return hd; };
  Player getOwner() const { return owner; };
  CUID getCUID() const { return card_unique_id; };
};

struct UnitEntityData {
  optional<CardInstance> data;
  uint32_t ATK;
  uint32_t HP;
  SubtypeManager subtypes;
  uint32_t damage;
};

struct HeroEntityData {
  HeroCardInstance data;
  uint32_t ATK;
  uint32_t HP;
  uint32_t damage;
  uint32_t level;
};

struct BuildingEntityData {
  uint32_t HP;
  uint32_t damage;
};

struct UpgradeEntityData {};

using EntityData = variant<
                UnitEntityData, 
                HeroEntityData,
                BuildingEntityData,
                UpgradeEntityData
>;

struct Entity {
  Timestamp timestamp;
  Player controller;
  EffectManager effects;
  EntityData data;
  bool exhausted;
};

struct TechBuilding {
  uint32_t health;
  // 0 or lower means ded
  bool hasBeenConstructedBefore;
  bool isBeingConstructed;
};

enum class Addon : uint8_t {
  Tower,
  Surplus,
  HerosHall,
  TechLab,
};

using Hand = vector<CardInstance>;
using Discard = vector<CardInstance>;
using Codex = vector<CardInstance>;

class Deck {
  vector<CardInstance> d;
  bool topKnown;
  public:
  optional<CardInstance> draw(uint32_t which);
  void add(CardInstance c) {
    d.push_back(c);
  };
  Deck() = default;
  Deck(vector<CardInstance> cards) {
    d = cards;
    topKnown = false;
  };
  auto begin() { return d.cbegin(); };
  auto end() { return d.cend(); };
};

struct HeroSlot {
  CUID cuid;
  uint32_t turnsUntilPlayable;
};

struct PlayerData {
  uint32_t baseHealth;
  std::array<TechBuilding, 3> techBuildings;
  std::array<HeroSlot, 3> heroSlots;
  Hand hand;
  Deck deck;
  Discard discard;
  Codex codex;
  uint32_t gold;
  Spec tech2Spec;
  EffectManager effects;
  uint32_t workers;
  optional<Addon> addon;
  uint32_t addonHealth;
  bool addonBuiltThisTurn;
};

using EntityManager = vector<Entity>;

enum class Phase : uint8_t {
  Tech,
  Ready,
  Upkeep,
  Main,
  DiscardDraw,
  End,
};

using EffectQueue = std::deque<Effect>;

class TimestampManager {
  uint32_t current = 0;
  public:
  Timestamp next() {
    current++;
    return Timestamp(current);
  };
};

struct EndOfCurrentActions {};
struct ForfeitAction {
  Player p;
};

struct DrawCardIndexAction {
  Player player;
  uint32_t which;
};

struct DrawCardCUIDAction {
  Player player;
  CUID cuid;
};

struct MakeWorker {
  CUID cuid;
};

struct PlayCardFromHand {
  CUID cuid;
};

struct PlayCardFromHandWithName {
  std::string name;
};

struct EndMainPhase {
  CUID squadleader = 0;
  CUID elite = 0;
  CUID scavenger = 0;
  CUID technician = 0;
  CUID lookout = 0;
};

struct TechCard {
  CUID cuid;
};

using Action = variant<
  EndOfCurrentActions,
  ForfeitAction,
  DrawCardIndexAction,
  DrawCardCUIDAction,
  MakeWorker,
  PlayCardFromHand,
  PlayCardFromHandWithName,
  EndMainPhase,
  TechCard
>;

template <typename T>
std::function<T()> make_iter_lambda(vector<T>& vec, T tdefault = T()) {
  uint32_t curr = 0;
  auto vecIter = [tdefault, curr, &vec]() mutable {
    if (curr >= vec.size()) {
      return tdefault;
    }
    T temp = vec[curr];
    curr++;
    return temp;
  };
  return vecIter;
};

class ActionManager {
  public:
  std::function<Action()> nextAction;
  ActionManager(std::function<Action()> na = 
      [](){ return Action(EndOfCurrentActions{}); }) : nextAction(na) {};
  ActionManager(vector<Action>& actions) : 
    ActionManager(make_iter_lambda<Action>
        (actions, Action(EndOfCurrentActions{}))) {};
};

struct NoError {};
struct EndGameActionGameAlreadyEnded {};
struct MakeWorkerCardNotFound {};
struct DrawCardIndexCardNotFound {};
struct MakeWorkerNotEnoughGold {};
struct UnimplementedError {};
struct IncorrectTimingError {};
struct PlayCardFromHandCardNotFound {};
struct PlayCardFromHandNotEnoughGold {};

using ProcessResult = boost::variant<
  NoError,
  UnimplementedError,
  EndGameActionGameAlreadyEnded,
  DrawCardIndexCardNotFound,
  MakeWorkerCardNotFound,
  MakeWorkerNotEnoughGold,
  PlayCardFromHandCardNotFound,
  PlayCardFromHandNotEnoughGold,
  IncorrectTimingError
>;

class GameData {
  friend class mainphase_action_visitor;
  std::array<PlayerData, 2> players;
  Player activePlayer;
  Phase currentPhase;
  EntityManager entities;
  uint32_t turn;
  optional<Player> winner;
  EffectQueue q;
  EffectQueue pendingSimultaneousEffects;
  TimestampManager cuidgen;
  TimestampManager tsGen;
  ActionManager am;
  PlayerData* playerData(Player p) {
    if (p == Player::Player1) {
      return &players[0];
    } else {
      return &players[1];
    }
  };
  optional<CardInstance> findInHand(Player p, 
      std::function<bool(CardInstance)> pred) {
    Hand h = playerData(p)->hand;
    auto o = std::find_if(h.begin(), h.end(), pred);
    if (o == h.end()) {
      return {};
    } else {
      return *o;
    }
  };

  void setupSingleSpec(Spec spec, Player player);
  void forEachEntity(std::function<void(Entity)> f) {
    std::for_each(entities.begin(), entities.end(), f);
  };
  void addSimultaneousEffect(Effect e) {
    pendingSimultaneousEffects.push_back(e);
  };
  void enqueueSimultaneousEffects() {
    int s = pendingSimultaneousEffects.size();
    if (s == 0) {
      return;
    }
    if (s == 1) {
      q.push_back(pendingSimultaneousEffects[0]);
      pendingSimultaneousEffects.clear();
      return;
    }
    else {
      // handle ordering of simultaneous effects
      return;
    }
  };
  ProcessResult calculateDerivedState();
  ProcessResult processSBA();
  ProcessResult processEffectQueue();
public:
  GameData(ActionManager amm = ActionManager {}) : am(amm) {};
  static GameData SingleSpecGame(ActionManager am, Spec p1spec, Spec p2spec);
  bool hasWinner() const { return static_cast<bool>(winner); };
  ProcessResult processActions();
  const vector<Entity> entityList() {
    return entities;
  };
  Phase getCurrentPhase() { return currentPhase; };
  const vector<Entity> playerEntities(Player p) {
    vector<Entity> vt;
    std::copy_if(entities.begin(), entities.end(), vt.begin(), 
        [p](Entity e){ return e.controller == p; });
    return vt;
  };
  uint32_t getTurn() { return turn; };
  Player getActivePlayer() { return activePlayer; };
  uint32_t playerGold(Player p) {
    return playerData(p)->gold;
  };
  uint32_t playerWorkers(Player p) {
    return playerData(p)->workers;
  };

};
