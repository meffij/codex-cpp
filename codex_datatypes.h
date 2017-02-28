// codex_datatypes.h
#include <vector>

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

enum class EffectType {
  Triggered,
  Layer,
};

struct Effect {
  EffectType type;
  Layer layer;
};

struct CardData {
  int card_id;
  int cost;
  CardType type;
  int ATK;
  int HP;
  Spec spec;
  Color color;
  std::vector<Effect> printed_effects;
};

// eventually build more detailed CardInfo with api to request it from card_id
//  would contain strings with name, text, flavor text, etc
