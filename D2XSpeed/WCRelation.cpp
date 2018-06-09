#include "stdafx.h"
#include "D2XConsts.h"
#include "WCRelation.h"
#include "MathExt.h"

WCRelation::WCRelation():
m_fBase(DATA_INVALID), m_nAttackForm(AF_REGULAR),
m_nCharClass(CC_INVALID), m_nNWIas(0), m_nFana(0), m_nBos(0),
m_nFrenzy(0), m_nWIas(0), m_nWerewolf(MIN_WEREWOLF)
{}

WCRelation::WCRelation(LPCTSTR lpszCharClass, LPCTSTR lpszWeaponName):
m_nAttackForm(AF_REGULAR), m_nNWIas(0), m_nFana(0), m_nBos(0),
m_nFrenzy(0), m_nWIas(0), m_nCharClass(CC_INVALID),
m_fBase(DATA_INVALID), m_nWerewolf(MIN_WEREWOLF)
{
	m_weapon = Weapon(lpszWeaponName);
	SetCharClass(lpszCharClass);	// CalcBase() is called
}

float WCRelation::CalcBase() const
{
	float fBase = DATA_INVALID;

	switch (m_nAttackForm)
	{
	case AF_ZEAL:
		fBase = ZEAL_BASE;
		break;

	case AF_SMITE:
		fBase = SMITE_BASE;
		break;

	case AF_STRAFE:
		fBase = STRAFE_BASE;
		break;

	case AF_WEREBEAR:
		fBase = WEREBEAR_BASE;
		break;

	case AF_WEREWOLF:
		fBase = WEREWOLF_BASE;
		break;

	case AF_FURY:
		fBase = FURY_BASE;
		break;

	case AF_LIGHTNING:
		fBase = LIGHTNING_BASE;
		break;

	case AF_CAST:
		{
		switch (m_nCharClass)
			{
			case CC_AMAZON:
				fBase = BS_AMA_CAST;
				break;
			case CC_ASSASSIN:
				fBase = BS_ASN_CAST;
				break;
			case CC_BARBARIAN:
				fBase = BS_BAR_CAST;
				break;
			case CC_PALADIN:
				fBase = BS_PAL_CAST;
				break;
			case CC_NECROMANCER:
				fBase = BS_NEC_CAST;
				break;
			case CC_DRUID:
				fBase = BS_DRU_CAST;
				break;
			case CC_SORCERESS:
				fBase = BS_SOR_CAST;
				break;
			default:
					break;
			} // switch (m_nCharClass)
		} // case AF_CAST
		break;

	case AF_THROW: // Throwing base for each classes
		{
			switch (m_nCharClass)
			{
			case CC_AMAZON:
				fBase = BS_AMA_THROW;
				break;
			case CC_ASSASSIN:
				fBase = BS_ASN_THROW;
				break;
			case CC_BARBARIAN:
				fBase = BS_BAR_THROW;
				break;
			case CC_PALADIN:
				fBase = BS_PAL_THROW;
				break;
			case CC_NECROMANCER:
				fBase = BS_NEC_THROW;
				break;
			case CC_DRUID:
				fBase = BS_DRU_THROW;
				break;
			case CC_SORCERESS:
				fBase = BS_SOR_THROW;
				break;
			default:
				break;
			}
		}
		break;

	case AF_REGULAR: // base values for normal attack
		{
			switch (m_weapon.GetCategory())
			{
			case WC_1HSWING:
				switch (m_nCharClass)
				{
				case CC_AMAZON:
					fBase = BS_AMA_1HSWING;
					break;
				case CC_ASSASSIN:
					fBase = BS_ASN_1HSWING;
					break;
				case CC_BARBARIAN:
					fBase = BS_BAR_1HSWING;
					break;
				case CC_PALADIN:
					fBase = BS_PAL_1HSWING;
					break;
				case CC_NECROMANCER:
					fBase = BS_NEC_1HSWING;
					break;
				case CC_DRUID:
					fBase = BS_DRU_1HSWING;
					break;
				case CC_SORCERESS:
					fBase = BS_SOR_1HSWING;
					break;
				default:
					break;
				} // switch (m_nCharClass)
				break;

			case WC_2HSWORD:
				switch (m_nCharClass)
				{
				case CC_AMAZON:
					fBase = BS_AMA_2HSWORD;
					break;
				case CC_ASSASSIN:
					fBase = BS_ASN_2HSWORD;
					break;
				case CC_BARBARIAN:
					fBase = BS_BAR_2HSWORD;
					break;
				case CC_PALADIN:
					fBase = BS_PAL_2HSWORD;
					break;
				case CC_NECROMANCER:
					fBase = BS_NEC_2HSWORD;
					break;
				case CC_DRUID:
					fBase = BS_DRU_2HSWORD;
					break;
				case CC_SORCERESS:
					fBase = BS_SOR_2HSWORD;
					break;
				default:
					break;
				} // switch (m_nCharClass)
				break;

			case WC_1HTHRUST:
				switch (m_nCharClass)
				{
				case CC_AMAZON:
					fBase = BS_AMA_1HTHRUST;
					break;
				case CC_ASSASSIN:
					fBase = BS_ASN_1HTHRUST;
					break;
			case CC_BARBARIAN:
					fBase = BS_BAR_1HTHRUST;
					break;
				case CC_PALADIN:
					fBase = BS_PAL_1HTHRUST;
					break;
				case CC_NECROMANCER:
					fBase = BS_NEC_1HTHRUST;
					break;
				case CC_DRUID:
					fBase = BS_DRU_1HTHRUST;
					break;
				case CC_SORCERESS:
					fBase = BS_SOR_1HTHRUST;
					break;
				default:
					break;
				} // switch (m_nCharClass)
				break;

			case WC_2HTHRUST:
				switch (m_nCharClass)
				{
				case CC_AMAZON:
					fBase = BS_AMA_2HTHRUST;
					break;
				case CC_ASSASSIN:
					fBase = BS_ASN_2HTHRUST;
					break;
				case CC_BARBARIAN:
					fBase = BS_BAR_2HTHRUST;
					break;
				case CC_PALADIN:
					fBase = BS_PAL_2HTHRUST;
					break;
				case CC_NECROMANCER:
					fBase = BS_NEC_2HTHRUST;
					break;
				case CC_DRUID:
					fBase = BS_DRU_2HTHRUST;
					break;
				case CC_SORCERESS:
					fBase = BS_SOR_2HTHRUST;
					break;
				default:
					break;
				} // switch (m_nCharClass)
				break;

			case WC_2HOTHER:
				switch (m_nCharClass)
				{
				case CC_AMAZON:
					fBase = BS_AMA_2HOTHER;
					break;
				case CC_ASSASSIN:
					fBase = BS_ASN_2HOTHER;
					break;
				case CC_BARBARIAN:
					fBase = BS_BAR_2HOTHER;
					break;
				case CC_PALADIN:
					fBase = BS_PAL_2HOTHER;
					break;
				case CC_NECROMANCER:
					fBase = BS_NEC_2HOTHER;
					break;
				case CC_DRUID:
					fBase = BS_DRU_2HOTHER;
					break;
				case CC_SORCERESS:
					fBase = BS_SOR_2HOTHER;
					break;
				default:
					break;
				} // switch (m_nCharClass)
				break;

			case WC_BOW:
				switch (m_nCharClass)
				{
				case CC_AMAZON:
					fBase = BS_AMA_BOW;
					break;
				case CC_ASSASSIN:
					fBase = BS_ASN_BOW;
					break;
				case CC_BARBARIAN:
					fBase = BS_BAR_BOW;
					break;
				case CC_PALADIN:
					fBase = BS_PAL_BOW;
					break;
				case CC_NECROMANCER:
					fBase = BS_NEC_BOW;
					break;
				case CC_DRUID:
					fBase = BS_DRU_BOW;
					break;
				case CC_SORCERESS:
					fBase = BS_SOR_BOW;
					break;
				default:
					break;
				} // switch (m_nCharClass)
				break;

			case WC_CROSSBOW:
				switch (m_nCharClass)
				{
				case CC_AMAZON:
					fBase = BS_AMA_CROSSBOW;
					break;
				case CC_ASSASSIN:
					fBase = BS_ASN_CROSSBOW;
					break;
				case CC_BARBARIAN:
					fBase = BS_BAR_CROSSBOW;
					break;
				case CC_PALADIN:
					fBase = BS_PAL_CROSSBOW;
					break;
				case CC_NECROMANCER:
					fBase = BS_NEC_CROSSBOW;
					break;
				case CC_DRUID:
					fBase = BS_DRU_CROSSBOW;
					break;
				case CC_SORCERESS:
					fBase = BS_SOR_CROSSBOW;
					break;
				default:
					break;
				} // switch (m_nCharClass)
				break;

			case WC_CLAW:
				switch (m_nCharClass)
				{
				case CC_AMAZON:
					fBase = BS_AMA_CLAW;
					break;
				case CC_ASSASSIN:
					fBase = BS_ASN_CLAW;
					break;
				case CC_BARBARIAN:
					fBase = BS_BAR_CLAW;
					break;
				case CC_PALADIN:
					fBase = BS_PAL_CLAW;
					break;
				case CC_NECROMANCER:
					fBase = BS_NEC_CLAW;
					break;
				case CC_DRUID:
					fBase = BS_DRU_CLAW;
					break;
				case CC_SORCERESS:
					fBase = BS_SOR_CLAW;
					break;
				default:
					break;
				} // switch (m_nCharClass)
				break;

			default: // other weapon categories
				break;
			} // switch (m_weapon.GetCategory())
		} // case AF_REGULAR:
		break;

		default: // other attack forms
			break;

	} // switch (m_nAttackForm)

	return fBase;
}

int WCRelation::GetCharClass() const
{
	return m_nCharClass;
}

Weapon WCRelation::GetWeapon() const
{
	return m_weapon;
}

float WCRelation::GetBase() const
{
	return m_fBase;
}

int WCRelation::SetCharClass(LPCTSTR lpszCharClass)
{
	int nOldCharClass = m_nCharClass;

	if (!IsSameCharClass(lpszCharClass))
	{
		CString strCharClass(lpszCharClass);
		
		if (!strCharClass.CompareNoCase("Amazon"))
			m_nCharClass = CC_AMAZON;
		else if (!strCharClass.CompareNoCase("Assassin"))
			m_nCharClass = CC_ASSASSIN;
		else if (!strCharClass.CompareNoCase("Barbarian"))
			m_nCharClass = CC_BARBARIAN;
		else if (!strCharClass.CompareNoCase("Druid"))
			m_nCharClass = CC_DRUID;
		else if (!strCharClass.CompareNoCase("Paladin"))
			m_nCharClass = CC_PALADIN;
		else if (!strCharClass.CompareNoCase("Necromancer"))
			m_nCharClass = CC_NECROMANCER;
		else if (!strCharClass.CompareNoCase("Sorceress"))
			m_nCharClass = CC_SORCERESS;
		else
			m_nCharClass = CC_INVALID;

		// altering chars causes attack form change
		CheckAttackForm();
		m_fBase = CalcBase();
	}

	return nOldCharClass;
}

Weapon WCRelation::SetWeapon(LPCTSTR lpszWeaponName)
{
	Weapon oldWeapon = m_weapon;

	if (!IsSameWeapon(lpszWeaponName))
	{
		m_weapon = Weapon(lpszWeaponName);
		
		// altering weapon does not necessarily change attack form,
		// need to check
		CheckAttackForm();
		m_fBase = CalcBase();
	}

	return oldWeapon;
}

int WCRelation::SetAttackForm(LPCTSTR lpszAttackForm)
{
	int nOldAttackForm = m_nAttackForm;
	CString strAttackForm(lpszAttackForm);
	
	if (!strAttackForm.CompareNoCase("Zeal"))
		m_nAttackForm = AF_ZEAL;
	else if (!strAttackForm.CompareNoCase("Cast"))
		m_nAttackForm = AF_CAST;
	else if (!strAttackForm.CompareNoCase("Throw"))
		m_nAttackForm = AF_THROW;
	else if (!strAttackForm.CompareNoCase("Strafe"))
		m_nAttackForm = AF_STRAFE;
	else if (!strAttackForm.CompareNoCase("Smite"))
		m_nAttackForm = AF_SMITE;
	else if (!strAttackForm.CompareNoCase("Werewolf"))
		m_nAttackForm = AF_WEREWOLF;
	else if (!strAttackForm.CompareNoCase("Werebear"))
		m_nAttackForm = AF_WEREBEAR;
	else if (!strAttackForm.CompareNoCase("Fury"))
		m_nAttackForm = AF_FURY;
	else if (!strAttackForm.CompareNoCase("Lightning/Chain Lightning"))
		m_nAttackForm = AF_LIGHTNING;
	else
		m_nAttackForm = AF_REGULAR;

	if (nOldAttackForm != m_nAttackForm)
	{
		// attack form changed
		m_fBase = CalcBase();
	}

	return nOldAttackForm;
}

int WCRelation::SetWIas(int nWIas)
{
	int nOldWIas = m_nWIas;
	m_nWIas = nWIas;
	return nOldWIas;
}

int WCRelation::SetNWIas(int nNWIas)
{
	int nOldNWIas = m_nNWIas;
	m_nNWIas = nNWIas;
	return nOldNWIas;
}

BOOL WCRelation::Zeal_able() const
{
	return (m_nCharClass == CC_PALADIN
		&& (m_weapon.GetCategory() == WC_1HSWING
		|| m_weapon.GetCategory() == WC_2HSWORD
		|| m_weapon.GetCategory() == WC_1HTHRUST
		|| m_weapon.GetCategory() == WC_2HTHRUST
		|| m_weapon.GetCategory() == WC_2HOTHER));
}

BOOL WCRelation::Smite_able() const
{
	return (m_nCharClass == CC_PALADIN
		&& (m_weapon.GetCategory() == WC_1HSWING
		|| m_weapon.GetCategory() == WC_1HTHRUST));
}

BOOL WCRelation::Strafe_able() const
{
	return (m_nCharClass == CC_AMAZON
		&& (m_weapon.GetCategory() == WC_BOW || m_weapon.GetCategory() == WC_CROSSBOW));
}

BOOL WCRelation::Were_able() const
{
	return (m_nCharClass == CC_DRUID
		&& m_weapon.GetCategory() != WC_INVALID
		&& m_weapon.GetCategory() != WC_CLAW
		&& m_weapon.GetCategory() != WC_BOW
		&& m_weapon.GetCategory() != WC_CROSSBOW
		&& m_weapon.GetCategory() != WC_THROW);
}

BOOL WCRelation::Throw_able() const
{
	CString strWeaponName = m_weapon.GetName();
	return (m_nCharClass != CC_INVALID
		&& (!strWeaponName.CompareNoCase("Throwing Knife")
		|| !strWeaponName.CompareNoCase("Balanced Knife")
		|| !strWeaponName.CompareNoCase("Throwing Axe")
		|| !strWeaponName.CompareNoCase("Balanced Axe")
		|| !strWeaponName.CompareNoCase("Battle Dart")
		|| !strWeaponName.CompareNoCase("War Dart")
		|| !strWeaponName.CompareNoCase("Francisca")
		|| !strWeaponName.CompareNoCase("Hurlbat")
		|| !strWeaponName.CompareNoCase("Flying Knife")
		|| !strWeaponName.CompareNoCase("Winged Knife")
		|| !strWeaponName.CompareNoCase("Flying Axe")
		|| !strWeaponName.CompareNoCase("Winged Axe")
		|| !strWeaponName.CompareNoCase("Maiden Javelin")
		|| !strWeaponName.CompareNoCase("Ceremonial Javelin")
		|| !strWeaponName.CompareNoCase("Matriarchal Javelin")
		|| !strWeaponName.CompareNoCase("Javelin")
		|| !strWeaponName.CompareNoCase("Pilum")
		|| !strWeaponName.CompareNoCase("Short Spear")
		|| !strWeaponName.CompareNoCase("Glaive")
		|| !strWeaponName.CompareNoCase("Throwing Spear")
		|| !strWeaponName.CompareNoCase("War Javelin")
		|| !strWeaponName.CompareNoCase("Great Pilum")
		|| !strWeaponName.CompareNoCase("Simbilan")
		|| !strWeaponName.CompareNoCase("Spiculum")
		|| !strWeaponName.CompareNoCase("Harpoon")
		|| !strWeaponName.CompareNoCase("Hyperion Javelin")
		|| !strWeaponName.CompareNoCase("Stygian Pilum")
		|| !strWeaponName.CompareNoCase("Balrog Spear")
		|| !strWeaponName.CompareNoCase("Ghost Glaive")
		|| !strWeaponName.CompareNoCase("Winged Harpoon")));
}

BOOL WCRelation::Frenzy_able() const
{
	return (m_nCharClass == CC_BARBARIAN);
}

BOOL WCRelation::Bos_able() const
{
	return (m_nCharClass == CC_ASSASSIN);
}

BOOL WCRelation::Fury_able() const
{
	return Were_able();
}

int WCRelation::DimiIas(int nIas)
{
	return RoundDown((float)nIas / (1 + (float)nIas / 120));
}

int WCRelation::RevIas(int nDimiIas)
{
	return RoundUp(120 * (float)nDimiIas / (120 - (float)nDimiIas));
}

int WCRelation::EIas() const
{
	int	nWsm, nSIas;
	if (m_nAttackForm == AF_CAST)
	{
		nWsm = 0;
		nSIas = 0;
	}
	else
	{
		nWsm = m_weapon.GetWsm();
        nSIas = GetSIas();
	}

	int nEIas = DimiIas(m_nWIas + m_nNWIas) + nSIas - nWsm;
	return (nEIas < MAX_EIAS? nEIas: MAX_EIAS);
}

int WCRelation::Speed(int nEIasVal) const
{
	int nSpeed = DATA_INVALID;
	int nCountRate;
	float fWere;

	switch (m_nAttackForm)
	{
	case AF_ZEAL:
		nCountRate = RoundDown((100 + (float)nEIasVal) / 100 * 256);
		nSpeed = RoundUp((ZEAL_BASE) * 256 / (float)nCountRate);
		nSpeed = nSpeed > MIN_MELEEX ? nSpeed : MIN_MELEEX;
		break;

	case AF_SMITE:
		nCountRate = RoundDown((100 + (float)nEIasVal) / 100 * 256);
		nSpeed = RoundUp((m_fBase + 1) * 256 / (float)nCountRate) - 1;
		break;

	case AF_FURY:
		break;

	case AF_STRAFE:
		//nSpeed = RoundUp(m_fBase / ((100 + float(EIas()) / 100)));
		break;

	case AF_WEREBEAR:
		fWere = WEREBEAR_BASE / (1 + ((float)m_nWIas 
			- float(m_weapon.GetWsm())) / 100) / (m_fBase + 1);
		nCountRate = RoundDown(RoundDown((100 + float(EIas())) / 100 * 256)/fWere);
		nSpeed = RoundUp((m_fBase + 1) * 256 / (float)nCountRate) - 1;
		break;

	case AF_WEREWOLF:
		fWere = WEREWOLF_BASE / (1 + ((float)m_nWIas 
			- float(m_weapon.GetWsm())) / 100) / (m_fBase + 1);
		nCountRate = RoundDown(RoundDown((100 + float(EIas())) / 100 * 256)/fWere);
		nSpeed = RoundUp((m_fBase + 1) * 256 / (float)nCountRate) - 1;
		break;

	default: // other attack
		nCountRate = RoundDown((100 + (float)nEIasVal) / 100 * 256);
		nSpeed = RoundUp((m_fBase + 1) * 256 / (float)nCountRate) - 1;
		nSpeed = nSpeed > MIN_MELEE ? nSpeed : MIN_MELEE;
	}

	return nSpeed;
}

int WCRelation::Speed() const
{
	return Speed(EIas());
}

int WCRelation::GetTable(IasUnit* tableArray) const
{
	int nCount = 0;
	int	nWsm, nSIas;
	if (m_nAttackForm == AF_CAST)
	{
		nWsm = 0;
		nSIas = 0;
	}
	else
	{
		nWsm = m_weapon.GetWsm();
        nSIas = GetSIas();
	}

	int	nEIas = nSIas - nWsm;
	nEIas = (nEIas < MAX_EIAS? nEIas: MAX_EIAS);

	int	nCurrentDias = 0;
	int	nCurrentSpeed = Speed(nEIas);
	int	nSpeed;
	
	tableArray[nCount++].Set(0, nCurrentSpeed);

	while (nEIas < MAX_EIAS)
	{
		nCurrentDias++;
		nEIas++;

		nSpeed = Speed(nEIas);

		if (nSpeed < nCurrentSpeed)
		{
			tableArray[nCount++].Set(RevIas(nCurrentDias), nSpeed);
			nCurrentSpeed = nSpeed;
		}
	}

	return nCount;
}

int WCRelation::GetWIas() const
{
	return m_nWIas;
}

int WCRelation::GetNWIas() const
{
	return m_nNWIas;
}

int WCRelation::GetIas() const
{
	return m_nWIas + m_nNWIas;
}

int WCRelation::GetFana() const
{
	return m_nFana;
}

int WCRelation::GetBos() const
{
	return m_nBos;
}

int WCRelation::GetFrenzy() const
{
	return m_nFrenzy;
}

int WCRelation::GetSIas() const
{
	int nSIas = m_nFana + m_nBos + m_nFrenzy;
	//if (m_nAttackForm == AF_WEREWOLF || m_nAttackForm == AF_FURY)
		//nSIas += m_nWerewolf;
	return nSIas;
}

int WCRelation::SetFana(int nFana)
{
	int nOldFana = m_nFana;
	m_nFana = nFana;
	return nOldFana;
}

int WCRelation::SetBos(int nBos)
{
	int nOldBos = m_nBos;
	m_nBos = nBos;
	return nOldBos;
}

int WCRelation::SetFrenzy(int nFrenzy)
{
	int nOldFrenzy = m_nFrenzy;
	m_nFrenzy = nFrenzy;
	return nOldFrenzy;
}

BOOL WCRelation::IsIasDistinct() const
{
	return (m_nAttackForm == AF_WEREBEAR
		|| m_nAttackForm == AF_WEREWOLF);
}

int WCRelation::GetWerewolf() const
{
	return m_nWerewolf;
}

int WCRelation::SetWerewolf(int nWerewolf)
{
	int nOldWerewolf = m_nWerewolf;
	m_nWerewolf = nWerewolf;
	return nOldWerewolf;
}

int WCRelation::GetWeaponNameList(char* nameArray[],
								  LPCTSTR lpszFilter /*="All"*/)
{
	CString strFilter(lpszFilter);
	int nCount = 0;

	if (!strFilter.CompareNoCase(TF_1HSWORD))
	{
		nameArray[nCount++] = "Short Sword";
		nameArray[nCount++] = "Scimitar";
		nameArray[nCount++] = "Sabre";
		nameArray[nCount++] = "Falchion";
		nameArray[nCount++] = "Crystal Sword";
		nameArray[nCount++] = "Broad Sword";
		nameArray[nCount++] = "Long Sword";
		nameArray[nCount++] = "War Sword";
		nameArray[nCount++] = "Gladius";
		nameArray[nCount++] = "Cutlass";
		nameArray[nCount++] = "Shamshir";
		nameArray[nCount++] = "Tulwar";
		nameArray[nCount++] = "Dimensional Blade";
		nameArray[nCount++] = "Battle Sword";
		nameArray[nCount++] = "Rune Sword";
		nameArray[nCount++] = "Ancient Sword";
		nameArray[nCount++] = "Falcata";
		nameArray[nCount++] = "Ataghan";
		nameArray[nCount++] = "Elegant Blade";
		nameArray[nCount++] = "Hydra Edge";
		nameArray[nCount++] = "Phase Blade";
		nameArray[nCount++] = "Conquest Sword";
		nameArray[nCount++] = "Cryptic Sword";
		nameArray[nCount++] = "Mythical Sword";
	}
	else if (!strFilter.CompareNoCase(TF_2HSWORD))
	{
		nameArray[nCount++] = "Two-handed Sword";
		nameArray[nCount++] = "Claymore";
		nameArray[nCount++] = "Giant Sword";
		nameArray[nCount++] = "Bastard Sword";
		nameArray[nCount++] = "Flamberge";
		nameArray[nCount++] = "Great Sword";
		nameArray[nCount++] = "Espandon";
		nameArray[nCount++] = "Dacian Falx";
		nameArray[nCount++] = "Tusk Sword";
		nameArray[nCount++] = "Gothic Sword";
		nameArray[nCount++] = "Zweihander";
		nameArray[nCount++] = "Executioner Sword";
		nameArray[nCount++] = "Legend Sword";
		nameArray[nCount++] = "Highland Blade";
		nameArray[nCount++] = "Balrog Blade";
		nameArray[nCount++] = "Champion Sword";
		nameArray[nCount++] = "Colossus Sword";
		nameArray[nCount++] = "Colossus Blade";
	}
	else if (!strFilter.CompareNoCase(TF_1HMACE))
	{
		nameArray[nCount++] = "Club";
		nameArray[nCount++] = "Spiked Club";
		nameArray[nCount++] = "Mace";
		nameArray[nCount++] = "Morning Star";
		nameArray[nCount++] = "Flail";
		nameArray[nCount++] = "War Hammer";
		nameArray[nCount++] = "Cudgel";
		nameArray[nCount++] = "Barbed Club";
		nameArray[nCount++] = "Flanged Mace";
		nameArray[nCount++] = "Jagged Star";
		nameArray[nCount++] = "Knout";
		nameArray[nCount++] = "Battle Hammer";
		nameArray[nCount++] = "Truncheon";
		nameArray[nCount++] = "Tyrant Club";
		nameArray[nCount++] = "Reinforced Mace";
		nameArray[nCount++] = "Devil Star";
		nameArray[nCount++] = "Scourge";
		nameArray[nCount++] = "Legendary Mallet";
	}
	else if (!strFilter.CompareNoCase(TF_2HMACE))
	{
		nameArray[nCount++] = "Maul";
		nameArray[nCount++] = "Great Maul";
		nameArray[nCount++] = "War Club";
		nameArray[nCount++] = "Martel de Fer";
		nameArray[nCount++] = "Ogre Maul";
		nameArray[nCount++] = "Thunder Maul";
	}
	else if (!strFilter.CompareNoCase(TF_1HAXE))
	{
		nameArray[nCount++] = "Hand Axe";
		nameArray[nCount++] = "Axe";
		nameArray[nCount++] = "Double Axe";
		nameArray[nCount++] = "Military Pick";
		nameArray[nCount++] = "War Axe";
		nameArray[nCount++] = "Hatchet";
		nameArray[nCount++] = "Cleaver";
		nameArray[nCount++] = "Twin Axe";
		nameArray[nCount++] = "Crowbill";
		nameArray[nCount++] = "Naga";
		nameArray[nCount++] = "Tomahawk";
		nameArray[nCount++] = "Small Crescent";
		nameArray[nCount++] = "Ettin Axe";
		nameArray[nCount++] = "War Spike";
		nameArray[nCount++] = "Berserker Axe";
	}
	else if (!strFilter.CompareNoCase(TF_2HAXE))
	{
		nameArray[nCount++] = "Large Axe";
		nameArray[nCount++] = "Broad Axe";
		nameArray[nCount++] = "Battle Axe";
		nameArray[nCount++] = "Great Axe";
		nameArray[nCount++] = "Giant Axe";
		nameArray[nCount++] = "Military Axe";
		nameArray[nCount++] = "Bearded Axe";
		nameArray[nCount++] = "Tabar";
		nameArray[nCount++] = "Gothic Axe";
		nameArray[nCount++] = "Ancient Axe";
		nameArray[nCount++] = "Feral Axe";
		nameArray[nCount++] = "Silver Edged Axe";
		nameArray[nCount++] = "Decapitator";
		nameArray[nCount++] = "Champion Axe";
		nameArray[nCount++] = "Glorious Axe";
	}
	else if (!strFilter.CompareNoCase(TF_WAND))
	{
		nameArray[nCount++] = "Wand";
		nameArray[nCount++] = "Yew Wand";
		nameArray[nCount++] = "Bone Wand";
		nameArray[nCount++] = "Grim Wand";
		nameArray[nCount++] = "Burnt Wand";
		nameArray[nCount++] = "Petrified Wand";
		nameArray[nCount++] = "Tomb Wand";
		nameArray[nCount++] = "Grave Wand";
		nameArray[nCount++] = "Polished Wand";
		nameArray[nCount++] = "Ghost Wand";
		nameArray[nCount++] = "Lich Wand";
		nameArray[nCount++] = "Unearthed Wand";
	}
	else if (!strFilter.CompareNoCase(TF_SCEPTER))
	{
		nameArray[nCount++] = "Scepter";
		nameArray[nCount++] = "Grand Scepter";
		nameArray[nCount++] = "War Scepter";
		nameArray[nCount++] = "Rune Scepter";
		nameArray[nCount++] = "Holy Water Sprinkler";
		nameArray[nCount++] = "Divine Scepter";
		nameArray[nCount++] = "Mighty Scepter";
		nameArray[nCount++] = "Seraph Rod";
		nameArray[nCount++] = "Caduceus";
	}
	else if (!strFilter.CompareNoCase(TF_DAGGER))
	{
		nameArray[nCount++] = "Dagger";
		nameArray[nCount++] = "Dirk";
		nameArray[nCount++] = "Kris";
		nameArray[nCount++] = "Blade";
		nameArray[nCount++] = "Poignard";
		nameArray[nCount++] = "Rondel";
		nameArray[nCount++] = "Cinquedeas";
		nameArray[nCount++] = "Stilleto";
		nameArray[nCount++] = "Bone Knife";
		nameArray[nCount++] = "Mithril Point";
		nameArray[nCount++] = "Fanged Knife";
		nameArray[nCount++] = "Legend Spike";
	}
	else if (!strFilter.CompareNoCase(TF_JAVELIN))
	{
		nameArray[nCount++] = "Javelin";
		nameArray[nCount++] = "Pilum";
		nameArray[nCount++] = "Short Spear";
		nameArray[nCount++] = "Glaive";
		nameArray[nCount++] = "Throwing Spear";
		nameArray[nCount++] = "Maiden Javelin";
		nameArray[nCount++] = "War Javelin";
		nameArray[nCount++] = "Great Pilum";
		nameArray[nCount++] = "Simbilan";
		nameArray[nCount++] = "Spiculum";
		nameArray[nCount++] = "Harpoon";
		nameArray[nCount++] = "Ceremonial Javelin";
		nameArray[nCount++] = "Hyperion Javelin";
		nameArray[nCount++] = "Stygian Pilum";
		nameArray[nCount++] = "Balrog Spear";
		nameArray[nCount++] = "Ghost Glaive";
		nameArray[nCount++] = "Winged Harpoon";
		nameArray[nCount++] = "Matriarchal Javelin";
	}
	else if (!strFilter.CompareNoCase(TF_SPEAR))
	{
		nameArray[nCount++] = "Spear";
		nameArray[nCount++] = "Trident";
		nameArray[nCount++] = "Brandistock";
		nameArray[nCount++] = "Spetum";
		nameArray[nCount++] = "Pike";
		nameArray[nCount++] = "Maiden Spear";
		nameArray[nCount++] = "Maiden Pike";
		nameArray[nCount++] = "War Spear";
		nameArray[nCount++] = "Fuscina";
		nameArray[nCount++] = "War Fork";
		nameArray[nCount++] = "Yari";
		nameArray[nCount++] = "Lance";
		nameArray[nCount++] = "Ceremonial Spear";
		nameArray[nCount++] = "Ceremonial Pike";
		nameArray[nCount++] = "Hyperion Spear";
		nameArray[nCount++] = "Stygian Pike";
		nameArray[nCount++] = "Mancatcher";
		nameArray[nCount++] = "Ghost Spear";
		nameArray[nCount++] = "War Pike";
		nameArray[nCount++] = "Matriarchal Spear";
		nameArray[nCount++] = "Matriarchal Pike";
	}
	else if (!strFilter.CompareNoCase(TF_POLEARM))
	{
		nameArray[nCount++] = "Bardiche";
		nameArray[nCount++] = "Voulge";
		nameArray[nCount++] = "Scythe";
		nameArray[nCount++] = "Poleaxe";
		nameArray[nCount++] = "Halberd";
		nameArray[nCount++] = "War Scythe";
		nameArray[nCount++] = "Lochaber Axe";
		nameArray[nCount++] = "Bill";
		nameArray[nCount++] = "Battle Scythe";
		nameArray[nCount++] = "Partizan";
		nameArray[nCount++] = "Bec-De-Corbin";
		nameArray[nCount++] = "Grim Scythe";
		nameArray[nCount++] = "Ogre Axe";
		nameArray[nCount++] = "Colossus Voulge";
		nameArray[nCount++] = "Thresher";
		nameArray[nCount++] = "Cryptic Axe";
		nameArray[nCount++] = "Great Poleaxe";
		nameArray[nCount++] = "Giant Thresher";
	}
	else if (!strFilter.CompareNoCase(TF_STAFF))
	{
		nameArray[nCount++] = "Short Staff";
		nameArray[nCount++] = "Long Staff";
		nameArray[nCount++] = "Gnarled Staff";
		nameArray[nCount++] = "Battle Staff";
		nameArray[nCount++] = "War Staff";
		nameArray[nCount++] = "Jo Staff";
		nameArray[nCount++] = "Quarter Staff";
		nameArray[nCount++] = "Cedar Staff";
		nameArray[nCount++] = "Gothic Staff";
		nameArray[nCount++] = "Rune Staff";
		nameArray[nCount++] = "Walking Stick";
		nameArray[nCount++] = "Stalagmite";
		nameArray[nCount++] = "Elder Staff";
		nameArray[nCount++] = "Shillelah";
		nameArray[nCount++] = "Archon Staff";
	}
	else if (!strFilter.CompareNoCase(TF_BOW))
	{
		nameArray[nCount++] = "Short Bow";
		nameArray[nCount++] = "Hunter's Bow";
		nameArray[nCount++] = "Long Bow";
		nameArray[nCount++] = "Composite Bow";
		nameArray[nCount++] = "Short Battle Bow";
		nameArray[nCount++] = "Long Battle Bow";
		nameArray[nCount++] = "Short War Bow";
		nameArray[nCount++] = "Long War Bow";
		nameArray[nCount++] = "Stag Bow";
		nameArray[nCount++] = "Reflex Bow";
		nameArray[nCount++] = "Edge Bow";
		nameArray[nCount++] = "Razor Bow";
		nameArray[nCount++] = "Cedar Bow";
		nameArray[nCount++] = "Double Bow";
		nameArray[nCount++] = "Short Siege Bow";
		nameArray[nCount++] = "Large Siege Bow";
		nameArray[nCount++] = "Rune Bow";
		nameArray[nCount++] = "Gothic Bow";
		nameArray[nCount++] = "Ashwood Bow";
		nameArray[nCount++] = "Ceremonial Bow";
		nameArray[nCount++] = "Spider Bow";
		nameArray[nCount++] = "Blade Bow";
		nameArray[nCount++] = "Shadow Bow";
		nameArray[nCount++] = "Great Bow";
		nameArray[nCount++] = "Diamond Bow";
		nameArray[nCount++] = "Crusader Bow";
		nameArray[nCount++] = "Ward Bow ";
		nameArray[nCount++] = "Hydra Bow";
		nameArray[nCount++] = "Matriarchal Bow";
		nameArray[nCount++] = "Grand Matron Bow";
	}
	else if (!strFilter.CompareNoCase(TF_CROSSBOW))
	{
		nameArray[nCount++] = "Light Crossbow";
		nameArray[nCount++] = "Crossbow";
		nameArray[nCount++] = "Heavy Crossbow";
		nameArray[nCount++] = "Repeating Crossbow";
		nameArray[nCount++] = "Arbalest";
		nameArray[nCount++] = "Siege Crossbow";
		nameArray[nCount++] = "Ballista";
		nameArray[nCount++] = "Chu-Ko-Nu";
		nameArray[nCount++] = "Pellet Bow";
		nameArray[nCount++] = "Gorgon Crossbow";
		nameArray[nCount++] = "Colossus Crossbow";
		nameArray[nCount++] = "Demon Crossbow";
	}
	else if (!strFilter.CompareNoCase(TF_ORB))
	{
		nameArray[nCount++] = "Eagle Orb";
		nameArray[nCount++] = "Sacred Globe";
		nameArray[nCount++] = "Smoked Sphere";
		nameArray[nCount++] = "Clasped Orb";
		nameArray[nCount++] = "Jared's Stone";
		nameArray[nCount++] = "Glowing Orb";
		nameArray[nCount++] = "Crystalline Globe";
		nameArray[nCount++] = "Cloudy Sphere";
		nameArray[nCount++] = "Sparkling Ball";
		nameArray[nCount++] = "Swirling Crystal";
		nameArray[nCount++] = "Heavenly Stone";
		nameArray[nCount++] = "Eldritch Orb";
		nameArray[nCount++] = "Demon Heart";
		nameArray[nCount++] = "Vortex Orb";
		nameArray[nCount++] = "Dimensional Shard";
	}
	else if (!strFilter.CompareNoCase(TF_CLAW))
	{
		nameArray[nCount++] = "Katar";
		nameArray[nCount++] = "Wrist Blade";
		nameArray[nCount++] = "Hatchet Hands";
		nameArray[nCount++] = "Cestus";
		nameArray[nCount++] = "Claws";
		nameArray[nCount++] = "Blade Talons";
		nameArray[nCount++] = "Scissors Katar";
		nameArray[nCount++] = "Quhab";
		nameArray[nCount++] = "Wrist Spike";
		nameArray[nCount++] = "Fascia";
		nameArray[nCount++] = "Hand Scythe";
		nameArray[nCount++] = "Greater Claws";
		nameArray[nCount++] = "Greater Talons";
		nameArray[nCount++] = "Scissors Quhab";
		nameArray[nCount++] = "Suwayyah";
		nameArray[nCount++] = "Wrist Sword";
		nameArray[nCount++] = "War Fist";
		nameArray[nCount++] = "Battle Cestus";
		nameArray[nCount++] = "Feral Claws";
		nameArray[nCount++] = "Runic Talons";
		nameArray[nCount++] = "Scissors Suwayyah";
	}
	else if (!strFilter.CompareNoCase(TF_THROW))
	{
		nameArray[nCount++] = "Throwing Knife";
		nameArray[nCount++] = "Balanced Knife";
		nameArray[nCount++] = "Battle Dart";
		nameArray[nCount++] = "War Dart";
		nameArray[nCount++] = "Flying Knife";
		nameArray[nCount++] = "Winged Knife";
		nameArray[nCount++] = "Throwing Axe";
		nameArray[nCount++] = "Balanced Axe";
		nameArray[nCount++] = "Francisca";
		nameArray[nCount++] = "Hurlbat";
		nameArray[nCount++] = "Flying Axe";
		nameArray[nCount++] = "Winged Axe";
	}
	else
	{
		// list all
		nameArray[nCount++] = "Short Sword";
		nameArray[nCount++] = "Scimitar";
		nameArray[nCount++] = "Sabre";
		nameArray[nCount++] = "Falchion";
		nameArray[nCount++] = "Crystal Sword";
		nameArray[nCount++] = "Broad Sword";
		nameArray[nCount++] = "Long Sword";
		nameArray[nCount++] = "War Sword";
		nameArray[nCount++] = "Gladius";
		nameArray[nCount++] = "Cutlass";
		nameArray[nCount++] = "Shamshir";
		nameArray[nCount++] = "Tulwar";
		nameArray[nCount++] = "Dimensional Blade";
		nameArray[nCount++] = "Battle Sword";
		nameArray[nCount++] = "Rune Sword";
		nameArray[nCount++] = "Ancient Sword";
		nameArray[nCount++] = "Falcata";
		nameArray[nCount++] = "Ataghan";
		nameArray[nCount++] = "Elegant Blade";
		nameArray[nCount++] = "Hydra Edge";
		nameArray[nCount++] = "Phase Blade";
		nameArray[nCount++] = "Conquest Sword";
		nameArray[nCount++] = "Cryptic Sword";
		nameArray[nCount++] = "Mythical Sword";

		nameArray[nCount++] = "Two-handed Sword";
		nameArray[nCount++] = "Claymore";
		nameArray[nCount++] = "Giant Sword";
		nameArray[nCount++] = "Bastard Sword";
		nameArray[nCount++] = "Flamberge";
		nameArray[nCount++] = "Great Sword";
		nameArray[nCount++] = "Espandon";
		nameArray[nCount++] = "Dacian Falx";
		nameArray[nCount++] = "Tusk Sword";
		nameArray[nCount++] = "Gothic Sword";
		nameArray[nCount++] = "Zweihander";
		nameArray[nCount++] = "Executioner Sword";
		nameArray[nCount++] = "Legend Sword";
		nameArray[nCount++] = "Highland Blade";
		nameArray[nCount++] = "Balrog Blade";
		nameArray[nCount++] = "Champion Sword";
		nameArray[nCount++] = "Colossus Sword";
		nameArray[nCount++] = "Colossus Blade";

		nameArray[nCount++] = "Club";
		nameArray[nCount++] = "Spiked Club";
		nameArray[nCount++] = "Mace";
		nameArray[nCount++] = "Morning Star";
		nameArray[nCount++] = "Flail";
		nameArray[nCount++] = "War Hammer";
		nameArray[nCount++] = "Cudgel";
		nameArray[nCount++] = "Barbed Club";
		nameArray[nCount++] = "Flanged Mace";
		nameArray[nCount++] = "Jagged Star";
		nameArray[nCount++] = "Knout";
		nameArray[nCount++] = "Battle Hammer";
		nameArray[nCount++] = "Truncheon";
		nameArray[nCount++] = "Tyrant Club";
		nameArray[nCount++] = "Reinforced Mace";
		nameArray[nCount++] = "Devil Star";
		nameArray[nCount++] = "Scourge";
		nameArray[nCount++] = "Legendary Mallet";

		nameArray[nCount++] = "Maul";
		nameArray[nCount++] = "Great Maul";
		nameArray[nCount++] = "War Club";
		nameArray[nCount++] = "Martel de Fer";
		nameArray[nCount++] = "Ogre Maul";
		nameArray[nCount++] = "Thunder Maul";

		nameArray[nCount++] = "Hand Axe";
		nameArray[nCount++] = "Axe";
		nameArray[nCount++] = "Double Axe";
		nameArray[nCount++] = "Military Pick";
		nameArray[nCount++] = "War Axe";
		nameArray[nCount++] = "Hatchet";
		nameArray[nCount++] = "Cleaver";
		nameArray[nCount++] = "Twin Axe";
		nameArray[nCount++] = "Crowbill";
		nameArray[nCount++] = "Naga";
		nameArray[nCount++] = "Tomahawk";
		nameArray[nCount++] = "Small Crescent";
		nameArray[nCount++] = "Ettin Axe";
		nameArray[nCount++] = "War Spike";
		nameArray[nCount++] = "Berserker Axe";
		nameArray[nCount++] = "Throwing Axe";
		nameArray[nCount++] = "Balanced Axe";
		nameArray[nCount++] = "Francisca";
		nameArray[nCount++] = "Hurlbat";
		nameArray[nCount++] = "Flying Axe";
		nameArray[nCount++] = "Winged Axe";

		nameArray[nCount++] = "Large Axe";
		nameArray[nCount++] = "Broad Axe";
		nameArray[nCount++] = "Battle Axe";
		nameArray[nCount++] = "Great Axe";
		nameArray[nCount++] = "Giant Axe";
		nameArray[nCount++] = "Military Axe";
		nameArray[nCount++] = "Bearded Axe";
		nameArray[nCount++] = "Tabar";
		nameArray[nCount++] = "Gothic Axe";
		nameArray[nCount++] = "Ancient Axe";
		nameArray[nCount++] = "Feral Axe";
		nameArray[nCount++] = "Silver Edged Axe";
		nameArray[nCount++] = "Decapitator";
		nameArray[nCount++] = "Champion Axe";
		nameArray[nCount++] = "Glorious Axe";

		nameArray[nCount++] = "Wand";
		nameArray[nCount++] = "Yew Wand";
		nameArray[nCount++] = "Bone Wand";
		nameArray[nCount++] = "Grim Wand";
		nameArray[nCount++] = "Burnt Wand";
		nameArray[nCount++] = "Petrified Wand";
		nameArray[nCount++] = "Tomb Wand";
		nameArray[nCount++] = "Grave Wand";
		nameArray[nCount++] = "Polished Wand";
		nameArray[nCount++] = "Ghost Wand";
		nameArray[nCount++] = "Lich Wand";
		nameArray[nCount++] = "Unearthed Wand";

		nameArray[nCount++] = "Scepter";
		nameArray[nCount++] = "Grand Scepter";
		nameArray[nCount++] = "War Scepter";
		nameArray[nCount++] = "Rune Scepter";
		nameArray[nCount++] = "Holy Water Sprinkler";
		nameArray[nCount++] = "Divine Scepter";
		nameArray[nCount++] = "Mighty Scepter";
		nameArray[nCount++] = "Seraph Rod";
		nameArray[nCount++] = "Caduceus";

		nameArray[nCount++] = "Dagger";
		nameArray[nCount++] = "Dirk";
		nameArray[nCount++] = "Kris";
		nameArray[nCount++] = "Blade";
		nameArray[nCount++] = "Poignard";
		nameArray[nCount++] = "Rondel";
		nameArray[nCount++] = "Cinquedeas";
		nameArray[nCount++] = "Stilleto";
		nameArray[nCount++] = "Bone Knife";
		nameArray[nCount++] = "Mithril Point";
		nameArray[nCount++] = "Fanged Knife";
		nameArray[nCount++] = "Legend Spike";
		nameArray[nCount++] = "Throwing Knife";
		nameArray[nCount++] = "Balanced Knife";
		nameArray[nCount++] = "Battle Dart";
		nameArray[nCount++] = "War Dart";
		nameArray[nCount++] = "Flying Knife";
		nameArray[nCount++] = "Winged Knife";

		nameArray[nCount++] = "Javelin";
		nameArray[nCount++] = "Pilum";
		nameArray[nCount++] = "Short Spear";
		nameArray[nCount++] = "Glaive";
		nameArray[nCount++] = "Throwing Spear";
		nameArray[nCount++] = "Maiden Javelin";
		nameArray[nCount++] = "War Javelin";
		nameArray[nCount++] = "Great Pilum";
		nameArray[nCount++] = "Simbilan";
		nameArray[nCount++] = "Spiculum";
		nameArray[nCount++] = "Harpoon";
		nameArray[nCount++] = "Ceremonial Javelin";
		nameArray[nCount++] = "Hyperion Javelin";
		nameArray[nCount++] = "Stygian Pilum";
		nameArray[nCount++] = "Balrog Spear";
		nameArray[nCount++] = "Ghost Glaive";
		nameArray[nCount++] = "Winged Harpoon";
		nameArray[nCount++] = "Matriarchal Javelin";

		nameArray[nCount++] = "Spear";
		nameArray[nCount++] = "Trident";
		nameArray[nCount++] = "Brandistock";
		nameArray[nCount++] = "Spetum";
		nameArray[nCount++] = "Pike";
		nameArray[nCount++] = "Maiden Spear";
		nameArray[nCount++] = "Maiden Pike";
		nameArray[nCount++] = "War Spear";
		nameArray[nCount++] = "Fuscina";
		nameArray[nCount++] = "War Fork";
		nameArray[nCount++] = "Yari";
		nameArray[nCount++] = "Lance";
		nameArray[nCount++] = "Ceremonial Spear";
		nameArray[nCount++] = "Ceremonial Pike";
		nameArray[nCount++] = "Hyperion Spear";
		nameArray[nCount++] = "Stygian Pike";
		nameArray[nCount++] = "Mancatcher";
		nameArray[nCount++] = "Ghost Spear";
		nameArray[nCount++] = "War Pike";
		nameArray[nCount++] = "Matriarchal Spear";
		nameArray[nCount++] = "Matriarchal Pike";

		nameArray[nCount++] = "Bardiche";
		nameArray[nCount++] = "Voulge";
		nameArray[nCount++] = "Scythe";
		nameArray[nCount++] = "Poleaxe";
		nameArray[nCount++] = "Halberd";
		nameArray[nCount++] = "War Scythe";
		nameArray[nCount++] = "Lochaber Axe";
		nameArray[nCount++] = "Bill";
		nameArray[nCount++] = "Battle Scythe";
		nameArray[nCount++] = "Partizan";
		nameArray[nCount++] = "Bec-De-Corbin";
		nameArray[nCount++] = "Grim Scythe";
		nameArray[nCount++] = "Ogre Axe";
		nameArray[nCount++] = "Colossus Voulge";
		nameArray[nCount++] = "Thresher";
		nameArray[nCount++] = "Cryptic Axe";
		nameArray[nCount++] = "Great Poleaxe";
		nameArray[nCount++] = "Giant Thresher";

		nameArray[nCount++] = "Short Staff";
		nameArray[nCount++] = "Long Staff";
		nameArray[nCount++] = "Gnarled Staff";
		nameArray[nCount++] = "Battle Staff";
		nameArray[nCount++] = "War Staff";
		nameArray[nCount++] = "Jo Staff";
		nameArray[nCount++] = "Quarter Staff";
		nameArray[nCount++] = "Cedar Staff";
		nameArray[nCount++] = "Gothic Staff";
		nameArray[nCount++] = "Rune Staff";
		nameArray[nCount++] = "Walking Stick";
		nameArray[nCount++] = "Stalagmite";
		nameArray[nCount++] = "Elder Staff";
		nameArray[nCount++] = "Shillelah";
		nameArray[nCount++] = "Archon Staff";

		nameArray[nCount++] = "Short Bow";
		nameArray[nCount++] = "Hunter's Bow";
		nameArray[nCount++] = "Long Bow";
		nameArray[nCount++] = "Composite Bow";
		nameArray[nCount++] = "Short Battle Bow";
		nameArray[nCount++] = "Long Battle Bow";
		nameArray[nCount++] = "Short War Bow";
		nameArray[nCount++] = "Long War Bow";
		nameArray[nCount++] = "Stag Bow";
		nameArray[nCount++] = "Reflex Bow";
		nameArray[nCount++] = "Edge Bow";
		nameArray[nCount++] = "Razor Bow";
		nameArray[nCount++] = "Cedar Bow";
		nameArray[nCount++] = "Double Bow";
		nameArray[nCount++] = "Short Siege Bow";
		nameArray[nCount++] = "Large Siege Bow";
		nameArray[nCount++] = "Rune Bow";
		nameArray[nCount++] = "Gothic Bow";
		nameArray[nCount++] = "Ashwood Bow";
		nameArray[nCount++] = "Ceremonial Bow";
		nameArray[nCount++] = "Spider Bow";
		nameArray[nCount++] = "Blade Bow";
		nameArray[nCount++] = "Shadow Bow";
		nameArray[nCount++] = "Great Bow";
		nameArray[nCount++] = "Diamond Bow";
		nameArray[nCount++] = "Crusader Bow";
		nameArray[nCount++] = "Ward Bow ";
		nameArray[nCount++] = "Hydra Bow";
		nameArray[nCount++] = "Matriarchal Bow";
		nameArray[nCount++] = "Grand Matron Bow";

		nameArray[nCount++] = "Light Crossbow";
		nameArray[nCount++] = "Crossbow";
		nameArray[nCount++] = "Heavy Crossbow";
		nameArray[nCount++] = "Repeating Crossbow";
		nameArray[nCount++] = "Arbalest";
		nameArray[nCount++] = "Siege Crossbow";
		nameArray[nCount++] = "Ballista";
		nameArray[nCount++] = "Chu-Ko-Nu";
		nameArray[nCount++] = "Pellet Bow";
		nameArray[nCount++] = "Gorgon Crossbow";
		nameArray[nCount++] = "Colossus Crossbow";
		nameArray[nCount++] = "Demon Crossbow";

		nameArray[nCount++] = "Eagle Orb";
		nameArray[nCount++] = "Sacred Globe";
		nameArray[nCount++] = "Smoked Sphere";
		nameArray[nCount++] = "Clasped Orb";
		nameArray[nCount++] = "Jared's Stone";
		nameArray[nCount++] = "Glowing Orb";
		nameArray[nCount++] = "Crystalline Globe";
		nameArray[nCount++] = "Cloudy Sphere";
		nameArray[nCount++] = "Sparkling Ball";
		nameArray[nCount++] = "Swirling Crystal";
		nameArray[nCount++] = "Heavenly Stone";
		nameArray[nCount++] = "Eldritch Orb";
		nameArray[nCount++] = "Demon Heart";
		nameArray[nCount++] = "Vortex Orb";
		nameArray[nCount++] = "Dimensional Shard";

		nameArray[nCount++] = "Katar";
		nameArray[nCount++] = "Wrist Blade";
		nameArray[nCount++] = "Hatchet Hands";
		nameArray[nCount++] = "Cestus";
		nameArray[nCount++] = "Claws";
		nameArray[nCount++] = "Blade Talons";
		nameArray[nCount++] = "Scissors Katar";
		nameArray[nCount++] = "Quhab";
		nameArray[nCount++] = "Wrist Spike";
		nameArray[nCount++] = "Fascia";
		nameArray[nCount++] = "Hand Scythe";
		nameArray[nCount++] = "Greater Claws";
		nameArray[nCount++] = "Greater Talons";
		nameArray[nCount++] = "Scissors Quhab";
		nameArray[nCount++] = "Suwayyah";
		nameArray[nCount++] = "Wrist Sword";
		nameArray[nCount++] = "War Fist";
		nameArray[nCount++] = "Battle Cestus";
		nameArray[nCount++] = "Feral Claws";
		nameArray[nCount++] = "Runic Talons";
		nameArray[nCount++] = "Scissors Suwayyah";

		nameArray[nCount++] = "Casting";
	}

	return nCount;
}

int WCRelation::GetWeaponFilterList(char* nameArray[])
{
	int nCount = 0;
	nameArray[nCount++] = TF_ALL;
	nameArray[nCount++] = TF_1HSWORD;
	nameArray[nCount++] = TF_2HSWORD;
	nameArray[nCount++] = TF_1HMACE;
	nameArray[nCount++] = TF_2HMACE;
	nameArray[nCount++] = TF_1HAXE;
	nameArray[nCount++] = TF_2HAXE;
	nameArray[nCount++] = TF_WAND;
	nameArray[nCount++] = TF_SCEPTER;
	nameArray[nCount++] = TF_DAGGER;
	nameArray[nCount++] = TF_JAVELIN;
	nameArray[nCount++] = TF_SPEAR;
	nameArray[nCount++] = TF_POLEARM;
	nameArray[nCount++] = TF_STAFF;
	nameArray[nCount++] = TF_BOW;
	nameArray[nCount++] = TF_CROSSBOW;
	nameArray[nCount++] = TF_ORB;
	nameArray[nCount++] = TF_CLAW;
	nameArray[nCount++] = TF_THROW;
	return nCount;
}

int WCRelation::GetCharNameList(char* nameArray[])
{
	int nCount = 0;
	nameArray[nCount++] = "Amazon";
	nameArray[nCount++] = "Assassin";
	nameArray[nCount++] = "Barbarian";
	nameArray[nCount++] = "Druid";
	nameArray[nCount++] = "Necromancer";
	nameArray[nCount++] = "Paladin";
	nameArray[nCount++] = "Sorceress";
	return nCount;
}

int WCRelation::GetAttackFormList(char* nameArray[])
{
	int nCount = 0;
	nameArray[nCount++] = "Normal";
	nameArray[nCount++] = "Cast";

	if (Throw_able())
	{
		nameArray[nCount++] = "Throw";
	}

	if (Zeal_able())
	{
		nameArray[nCount++] = "Zeal";
	}

	if (Smite_able())
	{
		nameArray[nCount++] = "Smite";
	}

	if (Strafe_able())
	{
		// temporarily disabled
		// nameArray[nCount++] = "Strafe";
	}

	if (Were_able())
	{
		// temporarily disabled
		//nameArray[nCount++] = "Werewolf";
		// nameArray[nCount++] = "Werebear";
	}

	if (Fury_able())
	{
		// temporarily disabled
		//nameArray[nCount++] = "Fury";
	}

	if (Lightning_able())
	{
		nameArray[nCount++] = "Lightning/Chain Lightning";
	}

	return nCount;
}

BOOL WCRelation::IsSameWeapon(LPCTSTR lpszWeaponName) const
{
	return (!m_weapon.GetName().CompareNoCase(lpszWeaponName));
}


BOOL WCRelation::IsSameCharClass(LPCTSTR lpszCharClass) const
{
	CString strCharClass(lpszCharClass);

	if (!strCharClass.CompareNoCase("Amazon"))
		return m_nCharClass == CC_AMAZON;
	else if (!strCharClass.CompareNoCase("Assassin"))
		return m_nCharClass == CC_ASSASSIN;
	else if (!strCharClass.CompareNoCase("Barbarian"))
		return m_nCharClass == CC_BARBARIAN;
	else if (!strCharClass.CompareNoCase("Druid"))
		return m_nCharClass == CC_DRUID;
	else if (!strCharClass.CompareNoCase("Necromancer"))
		return m_nCharClass == CC_NECROMANCER;
	else if (!strCharClass.CompareNoCase("Paladin"))
		return m_nCharClass == CC_PALADIN;
	else if (!strCharClass.CompareNoCase("Sorceress"))
		return m_nCharClass == CC_SORCERESS;
	else
		return m_nCharClass == CC_INVALID;
}

BOOL WCRelation::CheckAttackForm()
{
	BOOL bChanged = FALSE;

	switch (m_nAttackForm)
		{
		case AF_CAST:
			break;

		case AF_ZEAL:
			if (!Zeal_able())
			{
				m_nAttackForm = AF_REGULAR;
				bChanged = TRUE;
			}
			break;

		case AF_THROW:
			if (!Throw_able())
			{
				m_nAttackForm = AF_REGULAR;
				bChanged = TRUE;
			}
			break;

		case AF_STRAFE:
			if (!Strafe_able())
			{
				m_nAttackForm = AF_REGULAR;
				bChanged = TRUE;
			}
			break;

		case AF_SMITE:
			if (!Smite_able())
			{
				m_nAttackForm = AF_REGULAR;
				bChanged = TRUE;
			}
			break;

		case AF_WEREBEAR:
		case AF_WEREWOLF:
			if (!Were_able())
			{
				m_nAttackForm = AF_REGULAR;
				bChanged = TRUE;
			}
			break;

		case AF_FURY:
			if (!Fury_able())
			{
				m_nAttackForm = AF_REGULAR;
				bChanged = TRUE;
			}
			break;

		case AF_LIGHTNING:
			if (!Lightning_able())
			{
				m_nAttackForm = AF_REGULAR;
				bChanged = TRUE;
			}
			break;

		default:
			m_nAttackForm = AF_REGULAR;
			break;
		}

	return bChanged;
}

BOOL WCRelation::Lightning_able() const
{
	return m_nCharClass == CC_SORCERESS;
}

int WCRelation::GetAttackForm(void) const
{
	return m_nAttackForm;
}