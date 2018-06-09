#include "stdafx.h"
#include "D2XConsts.h"
#include "Weapon.h"

Weapon::Weapon():
m_nCategory(WC_INVALID), m_nRange(0), m_nWsm(0)
{}

Weapon::Weapon(LPCTSTR lpszName)
{
	m_strName = lpszName;
	m_nCategory = CalcCategory();
	m_nRange = CalcRange();
	m_nWsm = CalcWsm();
}

int Weapon::CalcCategory() const
{
	// one-handed swing
	if (!m_strName.CompareNoCase("Hand Axe")
		|| !m_strName.CompareNoCase("Axe")
		|| !m_strName.CompareNoCase("Double Axe")
		|| !m_strName.CompareNoCase("Military Pick")
		|| !m_strName.CompareNoCase("War Axe")
		|| !m_strName.CompareNoCase("Club")
		|| !m_strName.CompareNoCase("Spiked Club")
		|| !m_strName.CompareNoCase("Mace")
		|| !m_strName.CompareNoCase("Morning Star")
		|| !m_strName.CompareNoCase("Flail")
		|| !m_strName.CompareNoCase("War Hammer")
		|| !m_strName.CompareNoCase("Scepter")
		|| !m_strName.CompareNoCase("Grand Scepter")
		|| !m_strName.CompareNoCase("War Scepter")
		|| !m_strName.CompareNoCase("Short Sword")
		|| !m_strName.CompareNoCase("Scimitar")
		|| !m_strName.CompareNoCase("Sabre")
		|| !m_strName.CompareNoCase("Falchion")
		|| !m_strName.CompareNoCase("Crystal Sword")
		|| !m_strName.CompareNoCase("Broad Sword")
		|| !m_strName.CompareNoCase("Long Sword")
		|| !m_strName.CompareNoCase("War Sword")
		|| !m_strName.CompareNoCase("Wand")
		|| !m_strName.CompareNoCase("Yew Wand")
		|| !m_strName.CompareNoCase("Bone Wand")
		|| !m_strName.CompareNoCase("Grim Wand")
		|| !m_strName.CompareNoCase("Eagle Orb")
		|| !m_strName.CompareNoCase("Sacred Globe")
		|| !m_strName.CompareNoCase("Smoked Sphere")
		|| !m_strName.CompareNoCase("Clasped Orb")
		|| !m_strName.CompareNoCase("Jared's Stone")
		|| !m_strName.CompareNoCase("Hatchet")
		|| !m_strName.CompareNoCase("Cleaver")
		|| !m_strName.CompareNoCase("Twin Axe")
		|| !m_strName.CompareNoCase("Crowbill")
		|| !m_strName.CompareNoCase("Naga")
		|| !m_strName.CompareNoCase("Cudgel")
		|| !m_strName.CompareNoCase("Barbed Club")
		|| !m_strName.CompareNoCase("Flanged Mace")
		|| !m_strName.CompareNoCase("Jagged Star")
		|| !m_strName.CompareNoCase("Knout")
		|| !m_strName.CompareNoCase("Battle Hammer")
		|| !m_strName.CompareNoCase("Rune Scepter")
		|| !m_strName.CompareNoCase("Holy Water Sprinkler")
		|| !m_strName.CompareNoCase("Divine Scepter")
		|| !m_strName.CompareNoCase("Gladius")
		|| !m_strName.CompareNoCase("Cutlass")
		|| !m_strName.CompareNoCase("Shamshir")
		|| !m_strName.CompareNoCase("Tulwar")
		|| !m_strName.CompareNoCase("Dimensional Blade")
		|| !m_strName.CompareNoCase("Battle Sword")
		|| !m_strName.CompareNoCase("Rune Sword")
		|| !m_strName.CompareNoCase("Ancient Sword")
		|| !m_strName.CompareNoCase("Burnt Wand")
		|| !m_strName.CompareNoCase("Petrified Wand")
		|| !m_strName.CompareNoCase("Tomb Wand")
		|| !m_strName.CompareNoCase("Grave Wand")
		|| !m_strName.CompareNoCase("Glowing Orb")
		|| !m_strName.CompareNoCase("Crystalline Globe")
		|| !m_strName.CompareNoCase("Cloudy Sphere")
		|| !m_strName.CompareNoCase("Sparkling Ball")
		|| !m_strName.CompareNoCase("Swirling Crystal")
		|| !m_strName.CompareNoCase("Tomahawk")
		|| !m_strName.CompareNoCase("Small Crescent")
		|| !m_strName.CompareNoCase("Ettin Axe")
		|| !m_strName.CompareNoCase("War Spike")
		|| !m_strName.CompareNoCase("Berserker Axe")
		|| !m_strName.CompareNoCase("Truncheon")
		|| !m_strName.CompareNoCase("Tyrant Club")
		|| !m_strName.CompareNoCase("Reinforced Mace")
		|| !m_strName.CompareNoCase("Devil Star")
		|| !m_strName.CompareNoCase("Scourge")
		|| !m_strName.CompareNoCase("Legendary Mallet")
		|| !m_strName.CompareNoCase("Mighty Scepter")
		|| !m_strName.CompareNoCase("Seraph Rod")
		|| !m_strName.CompareNoCase("Caduceus")
		|| !m_strName.CompareNoCase("Falcata")
		|| !m_strName.CompareNoCase("Ataghan")
		|| !m_strName.CompareNoCase("Elegant Blade")
		|| !m_strName.CompareNoCase("Hydra Edge")
		|| !m_strName.CompareNoCase("Phase Blade")
		|| !m_strName.CompareNoCase("Conquest Sword")
		|| !m_strName.CompareNoCase("Cryptic Sword")
		|| !m_strName.CompareNoCase("Mythical Sword")
		|| !m_strName.CompareNoCase("Polished Wand")
		|| !m_strName.CompareNoCase("Ghost Wand")
		|| !m_strName.CompareNoCase("Lich Wand")
		|| !m_strName.CompareNoCase("Unearthed Wand")
		|| !m_strName.CompareNoCase("Heavenly Stone")
		|| !m_strName.CompareNoCase("Eldritch Orb")
		|| !m_strName.CompareNoCase("Demon Heart")
		|| !m_strName.CompareNoCase("Vortex Orb")
		|| !m_strName.CompareNoCase("Dimensional Shard")
		|| !m_strName.CompareNoCase("Throwing Axe")
		|| !m_strName.CompareNoCase("Balanced Axe")
		|| !m_strName.CompareNoCase("Francisca")
		|| !m_strName.CompareNoCase("Hurlbat")
		|| !m_strName.CompareNoCase("Flying Axe")
		|| !m_strName.CompareNoCase("Winged Axe"))
	{
		return WC_1HSWING;
	}

	// two-handed sword
	if (!m_strName.CompareNoCase("Two-handed Sword")
		|| !m_strName.CompareNoCase("Claymore")
		|| !m_strName.CompareNoCase("Giant Sword")
		|| !m_strName.CompareNoCase("Bastard Sword")
		|| !m_strName.CompareNoCase("Flamberge")
		|| !m_strName.CompareNoCase("Great Sword")
		|| !m_strName.CompareNoCase("Espandon")
		|| !m_strName.CompareNoCase("Dacian Falx")
		|| !m_strName.CompareNoCase("Tusk Sword")
		|| !m_strName.CompareNoCase("Gothic Sword")
		|| !m_strName.CompareNoCase("Zweihander")
		|| !m_strName.CompareNoCase("Executioner Sword")
		|| !m_strName.CompareNoCase("Legend Sword")
		|| !m_strName.CompareNoCase("Highland Blade")
		|| !m_strName.CompareNoCase("Balrog Blade")
		|| !m_strName.CompareNoCase("Champion Sword")
		|| !m_strName.CompareNoCase("Colossus Sword")
		|| !m_strName.CompareNoCase("Colossus Blade"))
	{
		return WC_2HSWORD;
	}

	// one-handed thrust
	if (!m_strName.CompareNoCase("Dagger")
		|| !m_strName.CompareNoCase("Dirk")
		|| !m_strName.CompareNoCase("Kris")
		|| !m_strName.CompareNoCase("Blade")
		|| !m_strName.CompareNoCase("Javelin")
		|| !m_strName.CompareNoCase("Pilum")
		|| !m_strName.CompareNoCase("Short Spear")
		|| !m_strName.CompareNoCase("Glaive")
		|| !m_strName.CompareNoCase("Throwing Spear")
		|| !m_strName.CompareNoCase("Poignard")
		|| !m_strName.CompareNoCase("Rondel")
		|| !m_strName.CompareNoCase("Cinquedeas")
		|| !m_strName.CompareNoCase("Stilleto")
		|| !m_strName.CompareNoCase("Bone Knife")
		|| !m_strName.CompareNoCase("Mithril Point")
		|| !m_strName.CompareNoCase("Fanged Knife")
		|| !m_strName.CompareNoCase("Legend Spike")
		|| !m_strName.CompareNoCase("Throwing Knife")
		|| !m_strName.CompareNoCase("Balanced Knife")
		|| !m_strName.CompareNoCase("Battle Dart")
		|| !m_strName.CompareNoCase("War Dart")
		|| !m_strName.CompareNoCase("Flying Knife")
		|| !m_strName.CompareNoCase("Winged Knife")
		|| !m_strName.CompareNoCase("Hyperion Javelin")
		|| !m_strName.CompareNoCase("Stygian Pilum")
		|| !m_strName.CompareNoCase("Balrog Spear")
		|| !m_strName.CompareNoCase("Ghost Glaive")
		|| !m_strName.CompareNoCase("Winged Harpoon")
		|| !m_strName.CompareNoCase("War Javelin")
		|| !m_strName.CompareNoCase("Great Pilum")
		|| !m_strName.CompareNoCase("Simbilan")
		|| !m_strName.CompareNoCase("Spiculum")
		|| !m_strName.CompareNoCase("Harpoon")
		|| !m_strName.CompareNoCase("Maiden Javelin")
		|| !m_strName.CompareNoCase("Ceremonial Javelin")
		|| !m_strName.CompareNoCase("Matriarchal Javelin"))
	{
		return WC_1HTHRUST;
	}

	// two-handed thrust
	if (!m_strName.CompareNoCase("Spear")
		|| !m_strName.CompareNoCase("Trident")
		|| !m_strName.CompareNoCase("Brandistock")
		|| !m_strName.CompareNoCase("Spetum")
		|| !m_strName.CompareNoCase("Pike")
		|| !m_strName.CompareNoCase("Maiden Spear")
		|| !m_strName.CompareNoCase("Maiden Pike")
		|| !m_strName.CompareNoCase("War Spear")
		|| !m_strName.CompareNoCase("Fuscina")
		|| !m_strName.CompareNoCase("War Fork")
		|| !m_strName.CompareNoCase("Yari")
		|| !m_strName.CompareNoCase("Lance")
		|| !m_strName.CompareNoCase("Ceremonial Spear")
		|| !m_strName.CompareNoCase("Ceremonial Pike")
		|| !m_strName.CompareNoCase("Hyperion Spear")
		|| !m_strName.CompareNoCase("Stygian Pike")
		|| !m_strName.CompareNoCase("Mancatcher")
		|| !m_strName.CompareNoCase("Ghost Spear")
		|| !m_strName.CompareNoCase("War Pike")
		|| !m_strName.CompareNoCase("Matriarchal Spear")
		|| !m_strName.CompareNoCase("Matriarchal Pike"))
	{
		return WC_2HTHRUST;
	}

	// two-handed other
	if (!m_strName.CompareNoCase("Large Axe")
		|| !m_strName.CompareNoCase("Broad Axe")
		|| !m_strName.CompareNoCase("Battle Axe")
		|| !m_strName.CompareNoCase("Great Axe")
		|| !m_strName.CompareNoCase("Giant Axe")
		|| !m_strName.CompareNoCase("Maul")
		|| !m_strName.CompareNoCase("Great Maul")
		|| !m_strName.CompareNoCase("Bardiche")
		|| !m_strName.CompareNoCase("Voulge")
		|| !m_strName.CompareNoCase("Scythe")
		|| !m_strName.CompareNoCase("Poleaxe")
		|| !m_strName.CompareNoCase("Halberd")
		|| !m_strName.CompareNoCase("War Scythe")
		|| !m_strName.CompareNoCase("Short Staff")
		|| !m_strName.CompareNoCase("Long Staff")
		|| !m_strName.CompareNoCase("Gnarled Staff")
		|| !m_strName.CompareNoCase("Battle Staff")
		|| !m_strName.CompareNoCase("War Staff")
		|| !m_strName.CompareNoCase("Military Axe")
		|| !m_strName.CompareNoCase("Bearded Axe")
		|| !m_strName.CompareNoCase("Tabar")
		|| !m_strName.CompareNoCase("Gothic Axe")
		|| !m_strName.CompareNoCase("Ancient Axe")
		|| !m_strName.CompareNoCase("War Club")
		|| !m_strName.CompareNoCase("Martel de Fer")
		|| !m_strName.CompareNoCase("Lochaber Axe")
		|| !m_strName.CompareNoCase("Bill")
		|| !m_strName.CompareNoCase("Battle Scythe")
		|| !m_strName.CompareNoCase("Partizan")
		|| !m_strName.CompareNoCase("Bec-De-Corbin")
		|| !m_strName.CompareNoCase("Grim Scythe")
		|| !m_strName.CompareNoCase("Jo Staff")
		|| !m_strName.CompareNoCase("Quarter Staff")
		|| !m_strName.CompareNoCase("Cedar Staff")
		|| !m_strName.CompareNoCase("Gothic Staff")
		|| !m_strName.CompareNoCase("Rune Staff")
		|| !m_strName.CompareNoCase("Feral Axe")
		|| !m_strName.CompareNoCase("Silver Edged Axe")
		|| !m_strName.CompareNoCase("Decapitator")
		|| !m_strName.CompareNoCase("Champion Axe")
		|| !m_strName.CompareNoCase("Glorious Axe")
		|| !m_strName.CompareNoCase("Ogre Maul")
		|| !m_strName.CompareNoCase("Thunder Maul")
		|| !m_strName.CompareNoCase("Ogre Axe")
		|| !m_strName.CompareNoCase("Colossus Voulge")
		|| !m_strName.CompareNoCase("Thresher")
		|| !m_strName.CompareNoCase("Cryptic Axe")
		|| !m_strName.CompareNoCase("Great Poleaxe")
		|| !m_strName.CompareNoCase("Giant Thresher")
		|| !m_strName.CompareNoCase("Walking Stick")
		|| !m_strName.CompareNoCase("Stalagmite")
		|| !m_strName.CompareNoCase("Elder Staff")
		|| !m_strName.CompareNoCase("Shillelah")
		|| !m_strName.CompareNoCase("Archon Staff"))
	{
		return WC_2HOTHER;
	}

	// bow
	if (!m_strName.CompareNoCase("Short Bow")
		|| !m_strName.CompareNoCase("Hunter's Bow")
		|| !m_strName.CompareNoCase("Long Bow")
		|| !m_strName.CompareNoCase("Composite Bow")
		|| !m_strName.CompareNoCase("Short Battle Bow")
		|| !m_strName.CompareNoCase("Long Battle Bow")
		|| !m_strName.CompareNoCase("Short War Bow")
		|| !m_strName.CompareNoCase("Long War Bow")
		|| !m_strName.CompareNoCase("Stag Bow")
		|| !m_strName.CompareNoCase("Reflex Bow")
		|| !m_strName.CompareNoCase("Edge Bow")
		|| !m_strName.CompareNoCase("Razor Bow")
		|| !m_strName.CompareNoCase("Cedar Bow")
		|| !m_strName.CompareNoCase("Double Bow")
		|| !m_strName.CompareNoCase("Short Siege Bow")
		|| !m_strName.CompareNoCase("Large Siege Bow")
		|| !m_strName.CompareNoCase("Rune Bow")
		|| !m_strName.CompareNoCase("Gothic Bow")
		|| !m_strName.CompareNoCase("Ashwood Bow")
		|| !m_strName.CompareNoCase("Ceremonial Bow")
		|| !m_strName.CompareNoCase("Spider Bow")
		|| !m_strName.CompareNoCase("Blade Bow")
		|| !m_strName.CompareNoCase("Shadow Bow")
		|| !m_strName.CompareNoCase("Great Bow")
		|| !m_strName.CompareNoCase("Diamond Bow")
		|| !m_strName.CompareNoCase("Crusader Bow")
		|| !m_strName.CompareNoCase("Ward Bow ")
		|| !m_strName.CompareNoCase("Hydra Bow")
		|| !m_strName.CompareNoCase("Matriarchal Bow")
		|| !m_strName.CompareNoCase("Grand Matron Bow"))
	{
		return WC_BOW;
	}

	// crossbow
	if (!m_strName.CompareNoCase("Light Crossbow")
		|| !m_strName.CompareNoCase("Crossbow")
		|| !m_strName.CompareNoCase("Heavy Crossbow")
		|| !m_strName.CompareNoCase("Repeating Crossbow")
		|| !m_strName.CompareNoCase("Arbalest")
		|| !m_strName.CompareNoCase("Siege Crossbow")
		|| !m_strName.CompareNoCase("Ballista")
		|| !m_strName.CompareNoCase("Chu-Ko-Nu")
		|| !m_strName.CompareNoCase("Pellet Bow")
		|| !m_strName.CompareNoCase("Gorgon Crossbow")
		|| !m_strName.CompareNoCase("Colossus Crossbow")
		|| !m_strName.CompareNoCase("Demon Crossbow"))
	{
		return WC_CROSSBOW;
	}

	// claw
	if (!m_strName.CompareNoCase("Katar")
		|| !m_strName.CompareNoCase("Wrist Blade")
		|| !m_strName.CompareNoCase("Hatchet Hands")
		|| !m_strName.CompareNoCase("Cestus")
		|| !m_strName.CompareNoCase("Claws")
		|| !m_strName.CompareNoCase("Blade Talons")
		|| !m_strName.CompareNoCase("Scissors Katar")
		|| !m_strName.CompareNoCase("Quhab")
		|| !m_strName.CompareNoCase("Wrist Spike")
		|| !m_strName.CompareNoCase("Fascia")
		|| !m_strName.CompareNoCase("Hand Scythe")
		|| !m_strName.CompareNoCase("Greater Claws")
		|| !m_strName.CompareNoCase("Greater Talons")
		|| !m_strName.CompareNoCase("Scissors Quhab")
		|| !m_strName.CompareNoCase("Suwayyah")
		|| !m_strName.CompareNoCase("Wrist Sword")
		|| !m_strName.CompareNoCase("War Fist")
		|| !m_strName.CompareNoCase("Battle Cestus")
		|| !m_strName.CompareNoCase("Feral Claws")
		|| !m_strName.CompareNoCase("Runic Talons")
		|| !m_strName.CompareNoCase("Scissors Suwayyah"))
	{
		return WC_CLAW;
	}

	// exceptions
	return WC_INVALID;
}

int Weapon::CalcRange() const
{
	return 0;
}

int Weapon::CalcWsm() const
{
	// 20
	if (!m_strName.CompareNoCase("Glaive")
		|| !m_strName.CompareNoCase("War Hammer")
		|| !m_strName.CompareNoCase("Great Maul")
		|| !m_strName.CompareNoCase("Pike")
		|| !m_strName.CompareNoCase("War Staff")
		|| !m_strName.CompareNoCase("Falchion")
		|| !m_strName.CompareNoCase("Spiculum")
		|| !m_strName.CompareNoCase("Battle Hammer")
		|| !m_strName.CompareNoCase("Martel de Fer")
		|| !m_strName.CompareNoCase("Lance")
		|| !m_strName.CompareNoCase("Tulwar")
		|| !m_strName.CompareNoCase("Ceremonial Pike")
		|| !m_strName.CompareNoCase("Ghost Glaive")
		|| !m_strName.CompareNoCase("Legendary Mallet")
		|| !m_strName.CompareNoCase("Thunder Maul")
		|| !m_strName.CompareNoCase("War Pike")
		|| !m_strName.CompareNoCase("Archon Staff")
		|| !m_strName.CompareNoCase("Matriarchal Pike"))
	{
		return 20;
	}
	
	// 10
	if (!m_strName.CompareNoCase("Axe")
		|| !m_strName.CompareNoCase("Large Siege Bow")
		|| !m_strName.CompareNoCase("Gothic Bow")
		|| !m_strName.CompareNoCase("Double Axe")
		|| !m_strName.CompareNoCase("Battle Axe")
		|| !m_strName.CompareNoCase("Giant Axe")
		|| !m_strName.CompareNoCase("Long Battle Bow")
		|| !m_strName.CompareNoCase("Long War Bow")
		|| !m_strName.CompareNoCase("Heavy Crossbow")
		|| !m_strName.CompareNoCase("Short Spear")
		|| !m_strName.CompareNoCase("Morning Star")
		|| !m_strName.CompareNoCase("Maul")
		|| !m_strName.CompareNoCase("Bardiche")
		|| !m_strName.CompareNoCase("Poleaxe")
		|| !m_strName.CompareNoCase("Grand Scepter")
		|| !m_strName.CompareNoCase("Gnarled Staff")
		|| !m_strName.CompareNoCase("Claymore")
		|| !m_strName.CompareNoCase("Bastard Sword")
		|| !m_strName.CompareNoCase("Great Sword")
		|| !m_strName.CompareNoCase("Yew Wand")
		|| !m_strName.CompareNoCase("Reflex Bow")
		|| !m_strName.CompareNoCase("Maiden Pike")
		|| !m_strName.CompareNoCase("Hatchet Hands")
		|| !m_strName.CompareNoCase("Jared's Stone")
		|| !m_strName.CompareNoCase("Cleaver")
		|| !m_strName.CompareNoCase("Twin Axe")
		|| !m_strName.CompareNoCase("Tabar")
		|| !m_strName.CompareNoCase("Ancient Axe")
		|| !m_strName.CompareNoCase("Ballista")
		|| !m_strName.CompareNoCase("Simbilan")
		|| !m_strName.CompareNoCase("Jagged Star")
		|| !m_strName.CompareNoCase("War Club")
		|| !m_strName.CompareNoCase("Lochaber Axe")
		|| !m_strName.CompareNoCase("Partizan")
		|| !m_strName.CompareNoCase("Holy Water Sprinkler")
		|| !m_strName.CompareNoCase("Cedar Staff")
		|| !m_strName.CompareNoCase("Dacian Falx")
		|| !m_strName.CompareNoCase("Gothic Sword")
		|| !m_strName.CompareNoCase("Executioner Sword")
		|| !m_strName.CompareNoCase("Petrified Wand")
		|| !m_strName.CompareNoCase("Ceremonial Bow")
		|| !m_strName.CompareNoCase("Fascia")
		|| !m_strName.CompareNoCase("Swirling Crystal")
		|| !m_strName.CompareNoCase("Small Crescent")
		|| !m_strName.CompareNoCase("Ettin Axe")
		|| !m_strName.CompareNoCase("Decapitator")
		|| !m_strName.CompareNoCase("Glorious Axe")
		|| !m_strName.CompareNoCase("Crusader Bow")
		|| !m_strName.CompareNoCase("Hydra Bow")
		|| !m_strName.CompareNoCase("Colossus Crossbow")
		|| !m_strName.CompareNoCase("Balrog Spear")
		|| !m_strName.CompareNoCase("Devil Star")
		|| !m_strName.CompareNoCase("Ogre Maul")
		|| !m_strName.CompareNoCase("Ogre Axe")
		|| !m_strName.CompareNoCase("Cryptic Axe")
		|| !m_strName.CompareNoCase("Seraph Rod")
		|| !m_strName.CompareNoCase("Elder Staff")
		|| !m_strName.CompareNoCase("Hydra Edge")
		|| !m_strName.CompareNoCase("Colossus Sword")
		|| !m_strName.CompareNoCase("Ghost Wand")
		|| !m_strName.CompareNoCase("Grand Matron Bow")
		|| !m_strName.CompareNoCase("War Fist")
		|| !m_strName.CompareNoCase("Dimensional Shard")
		|| !m_strName.CompareNoCase("Throwing Axe")
		|| !m_strName.CompareNoCase("Francisca")
		|| !m_strName.CompareNoCase("Flying Axe"))
	{
		return 10;
	}

	// 5
	if (!m_strName.CompareNoCase("Short Bow")
		|| !m_strName.CompareNoCase("Edge Bow")
		|| !m_strName.CompareNoCase("Spider Bow")
		|| !m_strName.CompareNoCase("Colossus Blade"))
	{
		return 5;
	}

	// 0
	if (!m_strName.CompareNoCase("Hand Axe")
		|| !m_strName.CompareNoCase("War Axe")
		|| !m_strName.CompareNoCase("Broad Axe")
		|| !m_strName.CompareNoCase("Long Bow")
		|| !m_strName.CompareNoCase("Short Battle Bow")
		|| !m_strName.CompareNoCase("Short War Bow")
		|| !m_strName.CompareNoCase("Crossbow")
		|| !m_strName.CompareNoCase("Dirk")
		|| !m_strName.CompareNoCase("Pilum")
		|| !m_strName.CompareNoCase("Spiked Club")
		|| !m_strName.CompareNoCase("Mace")
		|| !m_strName.CompareNoCase("Voulge")
		|| !m_strName.CompareNoCase("Halberd")
		|| !m_strName.CompareNoCase("Scepter")
		|| !m_strName.CompareNoCase("Trident")
		|| !m_strName.CompareNoCase("Spetum")
		|| !m_strName.CompareNoCase("Long Staff")
		|| !m_strName.CompareNoCase("Battle Staff")
		|| !m_strName.CompareNoCase("Short Sword")
		|| !m_strName.CompareNoCase("Crystal Sword")
		|| !m_strName.CompareNoCase("Broad Sword")
		|| !m_strName.CompareNoCase("War Sword")
		|| !m_strName.CompareNoCase("Two-handed Sword")
		|| !m_strName.CompareNoCase("Giant Sword")
		|| !m_strName.CompareNoCase("Wand")
		|| !m_strName.CompareNoCase("Grim Wand")
		|| !m_strName.CompareNoCase("Stag Bow")
		|| !m_strName.CompareNoCase("Maiden Spear")
		|| !m_strName.CompareNoCase("Wrist Blade")
		|| !m_strName.CompareNoCase("Cestus")
		|| !m_strName.CompareNoCase("Smoked Sphere")
		|| !m_strName.CompareNoCase("Clasped Orb")
		|| !m_strName.CompareNoCase("Hatchet")
		|| !m_strName.CompareNoCase("Naga")
		|| !m_strName.CompareNoCase("Bearded Axe")
		|| !m_strName.CompareNoCase("Cedar Bow")
		|| !m_strName.CompareNoCase("Short Siege Bow")
		|| !m_strName.CompareNoCase("Rune Bow")
		|| !m_strName.CompareNoCase("Siege Crossbow")
		|| !m_strName.CompareNoCase("Rondel")
		|| !m_strName.CompareNoCase("Great Pilum")
		|| !m_strName.CompareNoCase("Barbed Club")
		|| !m_strName.CompareNoCase("Flanged Mace")
		|| !m_strName.CompareNoCase("Bill")
		|| !m_strName.CompareNoCase("Bec-De-Corbin")
		|| !m_strName.CompareNoCase("Rune Scepter")
		|| !m_strName.CompareNoCase("Fuscina")
		|| !m_strName.CompareNoCase("Yari")
		|| !m_strName.CompareNoCase("Quarter Staff")
		|| !m_strName.CompareNoCase("Gothic Staff")
		|| !m_strName.CompareNoCase("Rune Staff")
		|| !m_strName.CompareNoCase("Gladius")
		|| !m_strName.CompareNoCase("Dimensional Blade")
		|| !m_strName.CompareNoCase("Battle Sword")
		|| !m_strName.CompareNoCase("Ancient Sword")
		|| !m_strName.CompareNoCase("Espandon")
		|| !m_strName.CompareNoCase("Tusk Sword")
		|| !m_strName.CompareNoCase("Burnt Wand")
		|| !m_strName.CompareNoCase("Grave Wand")
		|| !m_strName.CompareNoCase("Ashwood Bow")
		|| !m_strName.CompareNoCase("Ceremonial Spear")
		|| !m_strName.CompareNoCase("Quhab")
		|| !m_strName.CompareNoCase("Scissors Quhab")
		|| !m_strName.CompareNoCase("Cloudy Sphere")
		|| !m_strName.CompareNoCase("Sparkling Ball")
		|| !m_strName.CompareNoCase("Tomahawk")
		|| !m_strName.CompareNoCase("Berserker Axe")
		|| !m_strName.CompareNoCase("Silver Edged Axe")
		|| !m_strName.CompareNoCase("Shadow Bow")
		|| !m_strName.CompareNoCase("Diamond Bow")
		|| !m_strName.CompareNoCase("Ward Bow ")
		|| !m_strName.CompareNoCase("Gorgon Crossbow")
		|| !m_strName.CompareNoCase("Mithril Point")
		|| !m_strName.CompareNoCase("Stygian Pilum")
		|| !m_strName.CompareNoCase("Tyrant Club")
		|| !m_strName.CompareNoCase("Reinforced Mace")
		|| !m_strName.CompareNoCase("Colossus Voulge")
		|| !m_strName.CompareNoCase("Great Poleaxe")
		|| !m_strName.CompareNoCase("Mighty Scepter")
		|| !m_strName.CompareNoCase("Stygian Pike")
		|| !m_strName.CompareNoCase("Ghost Spear")
		|| !m_strName.CompareNoCase("Stalagmite")
		|| !m_strName.CompareNoCase("Shillelah")
		|| !m_strName.CompareNoCase("Falcata")
		|| !m_strName.CompareNoCase("Conquest Sword")
		|| !m_strName.CompareNoCase("Mythical Sword")
		|| !m_strName.CompareNoCase("Balrog Blade")
		|| !m_strName.CompareNoCase("Polished Wand")
		|| !m_strName.CompareNoCase("Unearthed Wand")
		|| !m_strName.CompareNoCase("Matriarchal Bow")
		|| !m_strName.CompareNoCase("Matriarchal Spear")
		|| !m_strName.CompareNoCase("Suwayyah")
		|| !m_strName.CompareNoCase("Scissors Suwayyah")
		|| !m_strName.CompareNoCase("Demon Heart")
		|| !m_strName.CompareNoCase("Vortex Orb")
		|| !m_strName.CompareNoCase("Throwing Knife")
		|| !m_strName.CompareNoCase("Battle Dart")
		|| !m_strName.CompareNoCase("Flying Knife")
		|| !m_strName.CompareNoCase("Casting"))
	{
		return 0;
	}

	// -5
	if (!m_strName.CompareNoCase("Highland Blade"))
	{
		return -5;
	}

	// -10
	if (!m_strName.CompareNoCase("Military Pick")
		|| !m_strName.CompareNoCase("Large Axe")
		|| !m_strName.CompareNoCase("Great Axe")
		|| !m_strName.CompareNoCase("Hunter's Bow")
		|| !m_strName.CompareNoCase("Composite Bow")
		|| !m_strName.CompareNoCase("Light Crossbow")
		|| !m_strName.CompareNoCase("Blade")
		|| !m_strName.CompareNoCase("Javelin")
		|| !m_strName.CompareNoCase("Throwing Spear")
		|| !m_strName.CompareNoCase("Club")
		|| !m_strName.CompareNoCase("Flail")
		|| !m_strName.CompareNoCase("Scythe")
		|| !m_strName.CompareNoCase("War Scythe")
		|| !m_strName.CompareNoCase("War Scepter")
		|| !m_strName.CompareNoCase("Spear")
		|| !m_strName.CompareNoCase("Short Staff")
		|| !m_strName.CompareNoCase("Sabre")
		|| !m_strName.CompareNoCase("Long Sword")
		|| !m_strName.CompareNoCase("Flamberge")
		|| !m_strName.CompareNoCase("Maiden Javelin")
		|| !m_strName.CompareNoCase("Katar")
		|| !m_strName.CompareNoCase("Claws")
		|| !m_strName.CompareNoCase("Scissors Katar")
		|| !m_strName.CompareNoCase("Eagle Orb")
		|| !m_strName.CompareNoCase("Sacred Globe")
		|| !m_strName.CompareNoCase("Crowbill")
		|| !m_strName.CompareNoCase("Military Axe")
		|| !m_strName.CompareNoCase("Gothic Axe")
		|| !m_strName.CompareNoCase("Razor Bow")
		|| !m_strName.CompareNoCase("Double Bow")
		|| !m_strName.CompareNoCase("Arbalest")
		|| !m_strName.CompareNoCase("Stilleto")
		|| !m_strName.CompareNoCase("War Javelin")
		|| !m_strName.CompareNoCase("Harpoon")
		|| !m_strName.CompareNoCase("Cudgel")
		|| !m_strName.CompareNoCase("Knout")
		|| !m_strName.CompareNoCase("Battle Scythe")
		|| !m_strName.CompareNoCase("Grim Scythe")
		|| !m_strName.CompareNoCase("Divine Scepter")
		|| !m_strName.CompareNoCase("War Spear")
		|| !m_strName.CompareNoCase("Jo Staff")
		|| !m_strName.CompareNoCase("Shamshir")
		|| !m_strName.CompareNoCase("Rune Sword")
		|| !m_strName.CompareNoCase("Zweihander")
		|| !m_strName.CompareNoCase("Ceremonial Javelin")
		|| !m_strName.CompareNoCase("Wrist Spike")
		|| !m_strName.CompareNoCase("Hand Scythe")
		|| !m_strName.CompareNoCase("Glowing Orb")
		|| !m_strName.CompareNoCase("Crystalline Globe")
		|| !m_strName.CompareNoCase("War Spike")
		|| !m_strName.CompareNoCase("Champion Axe")
		|| !m_strName.CompareNoCase("Blade Bow")
		|| !m_strName.CompareNoCase("Great Bow")
		|| !m_strName.CompareNoCase("Pellet Bow")
		|| !m_strName.CompareNoCase("Legend Spike")
		|| !m_strName.CompareNoCase("Hyperion Javelin")
		|| !m_strName.CompareNoCase("Winged Harpoon")
		|| !m_strName.CompareNoCase("Truncheon")
		|| !m_strName.CompareNoCase("Scourge")
		|| !m_strName.CompareNoCase("Thresher")
		|| !m_strName.CompareNoCase("Giant Thresher")
		|| !m_strName.CompareNoCase("Caduceus")
		|| !m_strName.CompareNoCase("Hyperion Spear")
		|| !m_strName.CompareNoCase("Walking Stick")
		|| !m_strName.CompareNoCase("Elegant Blade")
		|| !m_strName.CompareNoCase("Cryptic Sword")
		|| !m_strName.CompareNoCase("Champion Sword")
		|| !m_strName.CompareNoCase("Matriarchal Javelin")
		|| !m_strName.CompareNoCase("Wrist Sword")
		|| !m_strName.CompareNoCase("Battle Cestus")
		|| !m_strName.CompareNoCase("Heavenly Stone")
		|| !m_strName.CompareNoCase("Eldritch Orb")
		|| !m_strName.CompareNoCase("Balanced Axe")
		|| !m_strName.CompareNoCase("Hurlbat")
		|| !m_strName.CompareNoCase("Winged Axe"))
	{
		return -10;
	}

	// -15
	if (!m_strName.CompareNoCase("Feral Axe")
		|| !m_strName.CompareNoCase("Legend Sword"))
	{
		return -15;
	}

	// -20
	if (!m_strName.CompareNoCase("Dagger")
		|| !m_strName.CompareNoCase("Kris")
		|| !m_strName.CompareNoCase("Brandistock")
		|| !m_strName.CompareNoCase("Scimitar")
		|| !m_strName.CompareNoCase("Bone Wand")
		|| !m_strName.CompareNoCase("Blade Talons")
		|| !m_strName.CompareNoCase("Poignard")
		|| !m_strName.CompareNoCase("Cinquedeas")
		|| !m_strName.CompareNoCase("War Fork")
		|| !m_strName.CompareNoCase("Tomb Wand")
		|| !m_strName.CompareNoCase("Greater Claws")
		|| !m_strName.CompareNoCase("Bone Knife")
		|| !m_strName.CompareNoCase("Fanged Knife")
		|| !m_strName.CompareNoCase("Mancatcher")
		|| !m_strName.CompareNoCase("Ataghan")
		|| !m_strName.CompareNoCase("Lich Wand")
		|| !m_strName.CompareNoCase("Feral Claws")
		|| !m_strName.CompareNoCase("Balanced Knife")
		|| !m_strName.CompareNoCase("War Dart")
		|| !m_strName.CompareNoCase("Winged Knife"))
	{
		return -20;
	}

	// -30
	if (!m_strName.CompareNoCase("Cutlass")
		|| !m_strName.CompareNoCase("Greater Talons")
		|| !m_strName.CompareNoCase("Phase Blade")
		|| !m_strName.CompareNoCase("Runic Talons"))
	{
		return -30;
	}

	// -40
	if (!m_strName.CompareNoCase("Repeating Crossbow"))
	{
		return -40;
	}

	// -60
	if (!m_strName.CompareNoCase("Chu-Ko-Nu")
		|| !m_strName.CompareNoCase("Demon Crossbow"))
	{
		return -60;
	}

	return DATA_INVALID;
}


int Weapon::GetCategory() const
{
	return m_nCategory;
}

CString Weapon::GetName() const
{
	return m_strName;
}

int Weapon::GetRange() const
{
	return m_nRange;
}

int Weapon::GetWsm() const
{
	return m_nWsm;
}

BOOL Weapon::operator==(const Weapon& rhs) const
{
	return !(m_strName.CompareNoCase(rhs.m_strName));
}
