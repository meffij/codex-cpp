// codex_effect.h
#pragma once
#include "codex.h"

class Timestamp {
  uint32_t t;
  public:
  Timestamp(uint32_t tt) : t(tt) {};
  Timestamp() : t(0) {};
  inline bool operator<(const Timestamp& tt) const { return t < tt.t; };
  explicit operator int() const { return t; };
  CUID asCUID() { return t; };
  bool operator==(const Timestamp& rhs) const { return t == rhs.t; }
};

// layer notes
// this is how the "derived state" is arrived at
// see http://forums.sirlingames.com/t/rules-questions-thread/146/216
enum class Layer : uint8_t {
  None,
  // effects within layers always apply in timestamp order, there are
  // no sublayers that are not enumerated here
  PrintedValue,
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
  // clamp negative values to 1
  // Pestering Haunt's ability clamps its ATK to at most 1
  ValueDependentAbility,
  // Bluecoat Musketeer
  // Behind the Ferns
  
  // Apply SBA after this is over
  SBATriggeredAbility,
};

struct EffectDataNone {};

enum class Ability : uint8_t {
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
  Channeling,
  // card specific abilities
  BrickThiefAbility,
  BloomEffect,
  WitherEffect,
  // hero abilities
  TroqMidband,
  RiverMidband,
  RiverMaxband,
  // multi-card abilities
  FlagbearerAbility,
  Deal1DamageToPatroller,
  // parameter abilities -- just make multiple copies
  Resist,
  Healing,
  Obliterate,
  Frenzy,
  // runes
  Plus1Plus1Rune,
  Minus1Minus1Rune,
  // game-rule "abilities"
  AttackedThisTurn,
  AttackAbility,
  ControlledFromStartOfTurn,
  HeroLevelUpAbility,
  // player and whole-game "abilities"
  HasReshuffledThisMainPhase,
};

// instead just make multiple copies of non-param abilities
/*
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

using EffectData = const variant<
  EffectDataNone,
  Ability,
  EffectParam
>;
*/

class Effect {
  // EffectData data;
  Ability ability;
  Timestamp timestamp;
  Timestamp source;
  Layer layer = Layer::None;

public:
  Effect(Ability a, int t = 0) : ability(a), timestamp(t) {};
  Effect(Ability a, Timestamp t, Timestamp s) 
    : ability(a), timestamp(t), source(s) {};
  bool operator==(const Effect& rhs) const {
    return ability == rhs.ability
      && timestamp == rhs.timestamp
      && source == rhs.source
      && layer == rhs.layer
      ;
  }
};

constexpr inline bool countsAsWrittenAbility(Ability a) {
  using A = Ability;
  switch (a) {
    case A::AttackedThisTurn:
    case A::AttackAbility:
    case A::ControlledFromStartOfTurn:
    case A::HeroLevelUpAbility:

      return false; break;

    default: 
      return true; break;
  };
};
