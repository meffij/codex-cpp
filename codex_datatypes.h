// codex_datatypes.h

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
  Mercenary,
  Ninja,
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
  const int card_id;
  const int cost;
  const CardType type;
  const int ATK;
  const int HP;
  const Spec spec;
  const Color color;
  const TechLevel techlevel;
  const EffectManager effects;
  const SubtypeManager subtypes;
};

class CardInstance {
  const CardData* cd;
  int card_unique_id;
  Player owner;
public:
  CardInstance(const CardData* cdd, const int cui, const Player o) :
    cd(cdd), card_unique_id(cui), owner(o) {};
  CardInstance(const CardData& cdd, const int cui, const Player o) :
    cd(&cdd), card_unique_id(cui), owner(o) {};
  /*
  CardInstance(const CardInstance& other) :
    cd(other.cd),
    card_unique_id(other.card_unique_id),
    owner(other.owner) {};
  */
  const CardData* getCardData() { return cd; };
  int getCUID() { return card_unique_id; };
  Player getOwner() { return owner; };
};

struct HeroData {
  const int cost = 2;
  const int ATK;
  const int HP;
  const int midband_level;
  const int midband_ATK;
  const int midband_HP;
  const int maxband_level;
  const int maxband_ATK;
  const int maxband_HP;
  EffectManager effects;
  EffectManager midband_effects;
  EffectManager maxband_effects;
};

class HeroCardInstance {
  const HeroData* hd;
  Player owner;
  int card_unique_id;
public:
  const HeroData* getHeroData() const { return hd; };
  Player getOwner() const { return owner; };
  int getCUID() const { return card_unique_id; };
};

struct UnitEntityData {
  const optional<CardInstance> data;
  int ATK;
  int HP;
  SubtypeManager subtypes;
  int damage;
};

struct HeroEntityData {
  const HeroCardInstance data;
  int ATK;
  int HP;
  int damage;
  int level;
};

struct BuildingEntityData {
  int HP;
  int damage;
};

using EntityData = variant<
                UnitEntityData, 
                HeroEntityData,
                BuildingEntityData
>;

struct Entity {
  Timestamp timestamp;
  EntityType ty;
  Player controller;
  EffectManager effects;
  EntityData data;
};

struct TechBuilding {
  int health;
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
  optional<CardInstance> draw(int which);
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
  int cuid;
  int turnsUntilPlayable;
};

struct PlayerData {
  int baseHealth;
  std::array<TechBuilding, 3> techBuildings;
  std::array<HeroSlot, 3> heroSlots;
  Hand hand;
  Deck deck;
  Discard discard;
  Codex codex;
  int gold;
  Spec tech2Spec;
  EffectManager effects;
  int workers;
  optional<Addon> addon;
  int addonHealth;
  bool addonBuiltThisTurn;
};

class EntityManager {
  vector<Entity> e;
};

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

class GameData {
  std::array<PlayerData, 2> players;
  Player activePlayer;
  Phase currentPhase;
  EntityManager entities;
  optional<Player> winner;
  EffectQueue q;
public:
  GameData StarterTest();  
};
