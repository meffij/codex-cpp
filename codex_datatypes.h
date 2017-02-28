// codex_datatypes.h

enum class Player {
  Player1,
  Player2,
};

enum class Color {
  Neutral,
  Red,
  Green,
};

enum class CardType {
  Unit,
  Spell,
  Building,
  Upgrade,
};

enum class EntityType {
  Unit,
  Hero,
  Building,
  Upgrade,
};

enum class TechLevel {
  Tech0,
  Tech1,
  Tech2,
  Tech3,
};

enum class Spec {
  None,
  Bashing,
  Finesse,
  // more
};

// layer notes
// this is how the "derived state" is arrived at
// see http://forums.sirlingames.com/t/rules-questions-thread/146/216
enum class Layer {
  None,
  // effects within layers always apply in timestamp order, there are
  // no sublayers that are not enumerated here
  PrintedValues,
  // values printed on the card, or for tokens, on effect that made them
  // Flipped Dancer token uses back of token card's values
  // Heroes have 'printed' the ATK/HP of their current band and the abilities
  //  up to and including their current band
  // Whether entity is a Token or not is set at this layer
  ChaosMirrorOrCopy,
  // later effects in this layer see the results of applying earlier
  //  effects in this layer
  ValueSetting,
  // i.e. Faerie Dragon
  ValueIndependentAbility,
  // Scavenger, Technician, and Lookout do not grant abilities
  ValueModifying,
  // i.e. runes, buff/debuff effects, cards in play
  // order does not matter
  ValueClamping,
  // clamp negative values to 0
  // Pestering Haunt's ability clamps its ATK to at most 1
  ValueDependentAbility,
  // Bluecoat Musketeer
  // Behind the Ferns
  
  // Apply SBA after this is over
  SBATriggeredAbility,
};

struct EffectDataNone {};

enum class Ability {
  // keyword abilities
  Haste,
  Readiness,
  Overpower,
  Sparkshot,
  Stealth,
  Flying,
  AntiAir,
  Invisible,
  SwiftStrike,
  // card specific abilities
  BrickThiefAbility,
};

enum class CardSpecificAbility {
  BrickThiefAbility,
};

typedef CardSpecificAbility CSAbility;

enum class AbilityParam {
  Resist,
  Healing,
  Obliterate,
  Frenzy,
};

struct EffectParam {
  AbilityParam ability;
  int parameter;
};

typedef const variant<
  EffectDataNone,
  Ability,
  EffectParam
> EffectData;

class Effect {
  const int timestamp;
  EffectData data;

public:
  Effect(Ability k, int t = 0) : timestamp(t),
    data(EffectData {k}) {};
  Effect(AbilityParam k, int p, int t = 0) : 
    timestamp(t), data(EffectParam{k, p}) {};
};

enum class Subtype {
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

typedef EnumManager<Effect> EffectManager;
typedef EnumManager<Subtype> SubtypeManager;

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

struct CardInstance {
  const CardData& cd;
  const int card_unique_id;
  const Player owner;
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
struct HeroCardInstance {

};

struct UnitEntityData {
  const optional<CardInstance> data;
  int ATK;
  int HP;
  SubtypeManager subtypes;
};

struct HeroEntityData {
  const HeroCardInstance data;
  int ATK;
  int HP;
};


typedef variant<UnitEntityData> EntityData;

struct Entity {
  int timestamp;
  EntityType ty;
  bool wasControlledAtStartOfTurn;
  EffectManager effects;
  EntityData data;
};
