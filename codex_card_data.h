// codex_card_data.h
#pragma once
#include "codex.h"
#include "codex_effect.h"
#include "codex_datatypes.h"
namespace CodexCardData {
  const CardData TimelyMessenger {
    "Timely Messenger",
    0, 1, CardType::Unit, 1, 1, Spec::None, Color::Neutral, TechLevel::Tech0,
      { Ability::Haste }, 
      { Subtype::Mercenary }
  };

  const CardData Tenderfoot {
    "Tenderfoot",
    1, 1, CardType::Unit, 1, 2, Spec::None, Color::Neutral, TechLevel::Tech0,
      {}, { Subtype::Virtuoso } 
  };

  const CardData OlderBrother {
    "Older Brother",
    2, 2, CardType::Unit, 2, 2, Spec::None, Color::Neutral, TechLevel::Tech0,
      {}, { Subtype::Drunkard } 
  };

  const CardData BrickThief {
    "Brick Thief",
    3, 2, CardType::Unit, 2, 1, Spec::None, Color::Neutral, TechLevel::Tech0,
      { Ability::BrickThiefAbility, Ability::Resist }, 
      { Subtype::Mercenary } 
  };

  const CardData HelpfulTurtle {
    "Helpful Turtle",
    4, 2, CardType::Unit, 1, 2, Spec::None, Color::Neutral, TechLevel::Tech0,
      { Ability::Healing }, 
      { Subtype::CuteAnimal } 
  };

  const CardData GranfalloonFlagbearer {
    "Granfalloon Flagbearer",
    5, 3, CardType::Unit, 2, 2, Spec::None, Color::Neutral, TechLevel::Tech0,
      { Ability::FlagbearerAbility }, 
      { Subtype::Flagbearer } 
  };

  const CardData FruitNinja {
    "Fruit Ninja",
    6, 3, CardType::Unit, 2, 2, Spec::None, Color::Neutral, TechLevel::Tech0,
      { Ability::Frenzy }, 
      { Subtype::Ninja } 
  };

  const CardData Spark {
    "Spark",
    7, 1, CardType::Spell, 0, 0, Spec::None, Color::Neutral, TechLevel::Tech0,
      { Ability::Deal1DamageToPatroller },
      { Subtype::Burn } 
  };

  const CardData Bloom {
    "Bloom",
    8, 2, CardType::Spell, 0, 0, Spec::None, Color::Neutral, TechLevel::Tech0,
      { Ability::BloomEffect },
      { Subtype::Buff } 
  };

  const CardData Wither {
    "Wither",
    9, 2, CardType::Spell, 0, 0, Spec::None, Color::Neutral, TechLevel::Tech0,
      { Ability::WitherEffect },
      { Subtype::Debuff } 
  };
  const HeroData Troq {
    2, Spec::Bashing, 2, 3, 5, 3, 4, 8, 4, 5, 
      {},
      { Ability::TroqMidband },
      { Ability::Readiness }
  };
  const HeroData River {
    2, Spec::Finesse, 2, 3, 3, 2, 4, 5, 3, 4,
      {},
      { Ability::RiverMidband },
      { Ability::RiverMaxband },
  };

}
