// codex_card_data.h
namespace CodexCardData {
  const CardData TimelyMessenger {
    0, 1, CardType::Unit, 1, 1, Spec::None, Color::Neutral, TechLevel::Tech0,
      { Effect(Ability::Haste) }, {Subtype::Mercenary}
  };

  const CardData Tenderfoot {
    1, 1, CardType::Unit, 1, 2, Spec::None, Color::Neutral, TechLevel::Tech0,
      {}, {Subtype::Virtuoso} 
  };

  const CardData OlderBrother {
    2, 2, CardType::Unit, 2, 2, Spec::None, Color::Neutral, TechLevel::Tech0,
      {}, {Subtype::Drunkard} 
  };

  const CardData BrickThief {
    3, 2, CardType::Unit, 2, 1, Spec::None, Color::Neutral, TechLevel::Tech0,
      { Effect(Ability::BrickThiefAbility), 
        Effect(AbilityParam::Resist, 1) }, {Subtype::Mercenary} 
  };

  const CardData HelpfulTurtle {
    4, 2, CardType::Unit, 1, 2, Spec::None, Color::Neutral, TechLevel::Tech0,
      { Effect(AbilityParam::Healing, 1) }, {Subtype::CuteAnimal} 
  };
}
