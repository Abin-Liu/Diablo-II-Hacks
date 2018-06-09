// D2ItemDesc.cpp: implementation of the CD2ItemDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D2ItemDesc.h"
#include "SpellDef.h"
#include "..\\External\\ProgressWnd.h"
#include "ItemDBDef.h"
#include "..\\external\\StringEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum { MOD_INVALID = 0, MOD_OTHER, MOD_FIRE, MOD_LIT, MOD_COLD, MOD_POI, MOD_STR, MOD_DEX, MOD_VITA, MOD_ENG, MOD_INDES, MOD_ALLSKILL, MOD_CLASSSKILL };

struct SKILLTREE
{
	int nID;
	char szDescEng[64];
	char szDescChs[64];
};

static const DWORD BLUNDS[] = { 24, 25, 26, 29, 31, 36, -1 };

static const SKILLTREE TREES[] = {
	{ 0, "Bow And Crossbow Skills (Amazon Only)", "弓与十字弓系技能 (仅限于亚马逊)" },
	{ 1, "Passive And Magic Skills (Amazon Only)", "被动与魔法系技能 (仅限于亚马逊)" },
	{ 2, "Javelin and Spear Skills (Amazon Only)", "标枪与长矛系技能 (仅限于亚马逊)" },

	{ 8, "Fire Skills (Sorceress Only)", "火系技能 (仅限于女巫)"},
	{ 9, "Lightning Skills (Sorceress Only)", "电系技能 (仅限于女巫)"},
	{ 10, "Cold Skills (Sorceress Only)", "冰系技能 (仅限于女巫)"},

	{ 16, "Curses Skills (Necromancer Only)", "诅咒系技能 (仅限于男巫)"},
	{ 17, "Poison And Bone Skills (Necromancer Only)", "毒骨系技能 (仅限于男巫)"},
	{ 18, "Summoning Skills (Necromancer Only)", "召唤系技能 (仅限于男巫)"},

	{ 24, "Combat Skills (Paladin Only)", "格斗系技能 (仅限于圣骑士)"},
	{ 25, "Offensive Auras (Paladin Only)", "攻击系光环 (仅限于圣骑士)"},
	{ 26, "Defensive Auras (Paladin Only)", "防御系光环 (仅限于圣骑士)"},

	{ 32, "Combat Skills (Barbarian Only)", "格斗系技能 (仅限于野蛮人)"},
	{ 33, "Masteries (Barbarian Only)", "掌握系技能 (仅限于野蛮人)"},
	{ 34, "Warcries (Barbarian Only)", "战嚎系技能 (仅限于野蛮人)"},

	{ 40, "Summoning Skills (Druid Only)", "召唤系技能 (仅限于德鲁伊)"},
	{ 41, "Shape Shifting Skills (Druid Only)", "变身系技能 (仅限于德鲁伊)"},
	{ 42, "Elemental Skills (Druid Only)", "元素系技能 (仅限于德鲁伊)"},

	{ 48, "Traps (Assassin Only)", "陷阱系技能 (仅限于刺客)"},
	{ 49, "Shadow Disciplines (Assassin Only)", "影子训练系技能 (仅限于刺客)"},
	{ 50, "Martial Arts (Assassin Only)", "武术系技能 (仅限于刺客)"},

	{ -1, "", "" }
};

static const LPCSTR INDEXITEMS[] = { "rin", "amu", "cm1", "cm2", "cm3", "jew", NULL };

static const LPCSTR QUESTITEMS[] = {
	
	// Act 1 Items
	"bks", // Scroll of Inifuss (pre-translated)
	"bkd", // Scroll of Inifuss (post-translated)
	"hdm", // Horadric Malus
	"leg", // Wert's Leg
	
	// Act 2 Items
	"ass", // Book of Skill
	"box", // Horadic cube
	"tr1", // Horadric Scroll
	"vip", // Viper Amulet
	"msf", // Shaft of Kings
	"hst", // Horadric Staff

	// Act 3 Items
	"j34", // A Jade Figurine
	"g34", // The Golden Bird
	"xyz", // Potion of Life
	"g33", // The Gidbinn
	"qey", // Khalim's Eye
	"qhr", // Khalim's Heart
	"qbr", // Khalim's Brain
	"qf1", // Khalim's Flail
	"qf2", // Khalim's Will
	"bbb", // Lam Esen's Tome
	"mss", // Mephisto's Soulstone

	// Act 4 Items
	"hfh", // Hell Forge Hammer

	// Act 5 Items
	"ice", // Malah's Potion
	"tr2", // Scroll of Resistance

	NULL  // End of story
};

BOOL FindTreeDesc(int nID, CString& str);

BOOL FindTreeDesc(int nID, CString& str)
{
	for (int i = 0; TREES[i].nID >= 0; i++)
	{
		if (TREES[i].nID == nID)
		{
			str = g_bLanguage ? TREES[i].szDescChs : TREES[i].szDescEng;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL operator==(const MODDETAIL& lhs, const MODDETAIL& rhs)
{
	return lhs.dwModID == rhs.dwModID;
}

BOOL operator>(const MODDETAIL& lhs, const MODDETAIL& rhs)
{
	return lhs.dwModID > rhs.dwModID;
}

BOOL CD2ItemData::FromDatabase(CADODatabase *pDatabase, CADORecordset *pRS, const CModLib* aModLib, CResPack* pPack)
{
	if (pDatabase == NULL || pRS == NULL || pRS->IsEof())
		return FALSE;

	pRS->GetFieldValue("iid", dwItemID);
	pRS->GetFieldValue("playerid", dwPlayerID);
	pRS->GetFieldValue("isockets", nSockets);
	pRS->GetFieldValue("iflags", dwFlags);
	pRS->GetFieldValue("istrreq", dwStrReq);
	pRS->GetFieldValue("typeid", dwTypeID);
	pRS->GetFieldValue("idexreq", dwDexReq);
	pRS->GetFieldValue("ilvlreq", dwLvlReq);
	pRS->GetFieldValue("ilevel", dwiLevel);
	pRS->GetFieldValue("imindmg", dwMinDmg);
	pRS->GetFieldValue("imaxdmg", dwMaxDmg);
	pRS->GetFieldValue("idefense", dwDef);
	pRS->GetFieldValue("iquantity", dwQty);
	pRS->GetFieldValue("imaxdura", dwMaxDura);
	pRS->GetFieldValue("imindura", dwMinDura);
	pRS->GetFieldValue("icode", sCode);
	pRS->GetFieldValue(g_bLanguage ? "qualitynamechs" : "qualitynameeng", sQualityName);
	pRS->GetFieldValue(g_bLanguage ? "typenamechs" : "typenameeng", sTypeName);
	pRS->GetFieldValue("storageid", dwStorageID);
	pRS->GetFieldValue("ix", x);
	pRS->GetFieldValue("iy", y);
	pRS->GetFieldValue("bcx", cx);
	pRS->GetFieldValue("bcy", cy);
	pRS->GetFieldValue("qualityid", dwQualityID);
	pRS->GetFieldValue("btwohand", bTwoHand);
	pRS->GetFieldValue("iartwork", nArtwork);
	pRS->GetFieldValue("itemtitle", sName);
	pRS->GetFieldValue(g_bLanguage ? "storagenamechs" : "storagenameeng", sLocation);

	sName.Replace('^', '\'');

	m_aTitle.RemoveAll();
	CStringEx se(sName);
	BOOL b = se.FindToken("|");
	while (b)
	{
		CStringEx sSeg;
		b = se.FindNextToken(sSeg);
		sSeg.Trim();
		m_aTitle.Add((LPCSTR)sSeg);
	}
	
	sName.Replace('|', ' ');
	bQuestItem = IsQuestItem(sCode);
	bBlund = IsBlund(dwTypeID);
	bEthereal = dwFlags & 0x00400000;
	bIdentified = dwFlags & 0x00000010;
	bRuneword = dwFlags & 0x04000000;	
	Format();	

	nModCount = 0;
	CString str;
	str.Format("select * from attr where itemid=%d", dwItemID);
	MODDETAIL md;
	CADORecordset rs(pDatabase);	
	rs.Open((LPCSTR)str);

	CString sMod;
	int nStr = -1, nDex = -1, nEng = -1, nVit = -1;
	int nFire = -1, nLit = -1, nCold = -1, nPoi = -1;
	int nVal = 0;

	CString sAllSkill;
	CString sClassSkill;
	CString sStr;
	CString sDex;
	CString sEng;
	CString sVit;
	CString sFire;
	CString sLit;
	CString sCold;
	CString sPoi;
	CString sIndes;

	for (;!rs.IsEof() && nModCount < 16; rs.MoveNext())
	{
		rs.GetFieldValue("modid", md.dwModID);
		int nIdx = aModLib->Find(md);
		if (nIdx == -1)
			continue;

		const MODDETAIL& lib = aModLib->GetAt(nIdx);
		::memcpy(&aMods[nModCount], &lib, sizeof(MODDETAIL));

		rs.GetFieldValue("val1", aMods[nModCount].aValues[0]);
		rs.GetFieldValue("val2", aMods[nModCount].aValues[1]);
		rs.GetFieldValue("val3", aMods[nModCount].aValues[2]);
		rs.GetFieldValue("val4", aMods[nModCount].aValues[3]);

		int nModType = GetModDesc(aMods[nModCount], sMod, nVal);
		if (nModType != MOD_INVALID && m_aMods.Find(sMod) == -1)
		{
			switch (nModType)
			{
			case MOD_STR:
				sStr = sMod;
				nStr = nVal;
				break;

			case MOD_DEX:
				sDex = sMod;
				nDex = nVal;
				break;

			case MOD_ENG:
				sEng = sMod;
				nEng = nVal;
				break;

			case MOD_VITA:
				sVit = sMod;
				nVit = nVal;
				break;

			case MOD_FIRE:
				sFire = sMod;
				nFire = nVal;
				break;

			case MOD_LIT:
				sLit = sMod;
				nLit = nVal;
				break;

			case MOD_COLD:
				sCold = sMod;
				nCold = nVal;
				break;

			case MOD_POI:
				sPoi = sMod;
				nPoi = nVal;
				break;

			case MOD_ALLSKILL:
				sAllSkill = sMod;
				break;

			case MOD_CLASSSKILL:
				sClassSkill = sMod;
				break;
		
			case MOD_INDES:
				sIndes = sMod;
				break;

			default:
				m_aMods.Add(sMod);
				break;
			}				
		}			
		
		nModCount++;
	}


	if (IsValSame(nFire, nLit, nCold, nPoi))
	{
		sMod.Format(g_bLanguage ? "所有抗性+%d%%" : "All Resistance +%d%%", nFire);
		InsertStr(sMod);
	}
	else
	{
		InsertStr(sPoi);
		InsertStr(sCold);
		InsertStr(sLit);
		InsertStr(sFire);
	}

	if (IsValSame(nStr, nDex, nEng, nVit))
	{
		sMod.Format(g_bLanguage ? "+%d所有属性" : "+%d to All Attributes", nStr);
		InsertStr(sMod);
	}
	else
	{
		InsertStr(sVit);
		InsertStr(sEng);
		InsertStr(sDex);
		InsertStr(sStr);
	}

	InsertStr(sClassSkill);
	InsertStr(sAllSkill);
	InsertStr(sIndes);

	if (!LoadImageResource(pPack))
		LoadUnknownResource(pPack);

	czImage = image.GetPictureSize();
		
	return TRUE;
}

void CALLBACK FreeD2ItemData(CD2ItemData*& pData)
{
	if (pData)
	{
		delete pData;
		pData = NULL;
	}
}

void CALLBACK FreeD2PlayerData(CD2PlayerData*& pData)
{
	if (pData)
	{
		delete pData;
		pData = NULL;
	}
}

BOOL CD2PlayerData::FromDatabase(CADODatabase *pDatabase, CADORecordset *pRS, const CModLib* aModLib, CResPack* pPack)
{
	if (pDatabase == NULL || pRS == NULL || pRS->IsEof())
		return FALSE;

	pRS->GetFieldValue("pid", dwPlayerID);
	pRS->GetFieldValue("pname", sName);
	pRS->GetFieldValue("classid", dwClassID);
	pRS->GetFieldValue("accountid", dwAccountID);
	pRS->GetFieldValue("phardcore", bHardcore);
	pRS->GetFieldValue(g_bLanguage ? "chsname" : "engname", sClassName);
	pRS->GetFieldValue("pinvgold", dwInvGold);
	pRS->GetFieldValue("pstashgold", dwStashGold);
	pRS->GetFieldValue("pgoldlimit", dwGoldLimit);
	pRS->GetFieldValue("pmercclass", dwMercClass);
	pRS->GetFieldValue("pmercname", sMercName);
	pRS->GetFieldValue("pweaponswitch", nActiveSwitch);

	m_aItemList.RemoveAll();

	CString str;
	str.Format("select * from [listitem] where playerid=%d order by iid", dwPlayerID);

	CADORecordset rs(pDatabase);
	rs.Open((LPCSTR)str);

	CProgressWnd dlg;	
	dlg.Create(AfxGetMainWnd(), "ItemDB");
	dlg.SetRange(0, max(1, rs.GetRecordCount()));
	dlg.SetPos(0);
	dlg.SetStep(1);

	CD2ItemData* p = NULL;
	CString sStatus;
	while (!rs.IsEof())
	{
		sStatus.Empty();
		if (p == NULL)
			p = new CD2ItemData;

		if (p->FromDatabase(pDatabase, &rs, aModLib, pPack))
		{
			m_aItemList.InsertRear(p);
			sStatus = p->m_aTitle.IsEmpty() ? p->sCode : p->m_aTitle[0];
			sStatus.TrimLeft();
			dlg.SetText(sStatus);
			p = NULL;
		}
		
		dlg.StepIt();
		rs.MoveNext();
	}

	return TRUE;
}

void CD2ItemData::Format()
{
	m_aHead.RemoveAll();
	m_aMods.RemoveAll();
	m_aFoot.RemoveAll();
	::memset(&czImage, 0, sizeof(SIZE));
	::memset(&czHover, 0, sizeof(SIZE));

	if (!bIdentified)
		return;

	CString sTemp;	

	if (bBlund)
		m_aFoot.Add(g_bLanguage ? "对不死系怪物增加50%攻击力" : "+50% Damage to Undead");

	if (nSockets)
	{
		sTemp.Format(g_bLanguage ? "有孔洞的 (%d)" : "Socketed (%d)", nSockets);
		m_aFoot.Add(sTemp);
	}

	if (bEthereal)
		m_aFoot.Add(g_bLanguage ? "无形的 (不可修复)" : "Ethereal (Cannot be Repaired)");	

	if (g_bLanguage)
		sTemp.Format("代码: \"%s\"  等级: %d", (LPCSTR)sCode, dwiLevel);
	else
		sTemp.Format("Code: \"%s\"   iLevel: %d", (LPCSTR)sCode, dwiLevel);
	
	m_aHead.Add(sTemp);	

	if (dwDef)
	{
		sTemp.Format(g_bLanguage ? "防御力: %d" : "Defense: %d", dwDef);
		m_aHead.Add(sTemp);
	}

	if (dwMinDmg && dwMaxDmg)
	{
		if (g_bLanguage)
			sTemp.Format("%s攻击力: %d到%d", bTwoHand ? "双手" : "单手", dwMinDmg, dwMaxDmg);
		else
			sTemp.Format("%s Damage: %d to %d", bTwoHand ? "Two-Hand" : "One-Hand", dwMinDmg, dwMaxDmg);
		m_aHead.Add(sTemp);
	}

	if (dwMaxDura > 0 && dwMaxDura < 32768)
	{
		sTemp.Format(g_bLanguage ? "牢固度: %d之%d" : "Durability: %d of %d", dwMinDura, dwMaxDura);
		m_aHead.Add(sTemp);
	}

	if (dwQty)
	{
		sTemp.Format(g_bLanguage ? "数量: %d" : "Quantity: %d", dwQty);
		m_aHead.Add(sTemp);
	}

	if (dwDexReq > 1)
	{
		sTemp.Format(g_bLanguage ? "要求敏捷: %d" : "Required Dexterity: %d", dwDexReq);
		m_aHead.Add(sTemp);
	}

	if (dwStrReq > 1)
	{
		sTemp.Format(g_bLanguage ? "要求强壮: %d" : "Required Strength: %d", dwStrReq);
		m_aHead.Add(sTemp);
	}

	if (dwLvlReq > 1)
	{
		sTemp.Format(g_bLanguage ? "要求等级: %d" : "Required Level: %d", dwLvlReq);
		m_aHead.Add(sTemp);
	}	
}

BOOL CD2ItemData::GetModDesc(const MODDETAIL& mod, CString& sResult, int& nVal)
{
	nVal = mod.aValues[0];
	sResult.Empty();

	if (mod.bBoolean)
	{
		sResult = mod.szFmt;
		return mod.dwModID == 152 ? MOD_INDES : MOD_OTHER;
	}

	if (mod.dwModID == 188)
	{
		// tree
		CString str;
		if (FindTreeDesc(mod.aValues[1], str))
		{
			sResult.Format(g_bLanguage ? "+%d级%s" : "+%d to %s", mod.aValues[0], (LPCSTR)str);
			return MOD_OTHER;
		}
	}

	if (mod.dwModID == 83)
	{
		char szClass[64] = "";
		if (D2GetClassName(mod.aValues[1], szClass, 63))
		{
			sResult.Format(g_bLanguage ? "+%d级%s技能等级" : "+%d to %s Skill Levels", mod.aValues[0], szClass);
			return MOD_CLASSSKILL;
		}		
	}

	if (mod.dwModID == 204)
	{
		D2SPELLINFO dsi = { 0 };
		if (D2GetSpellInfo(mod.aValues[1], &dsi))
		{
			CString sFmt = mod.szFmt;
			sFmt.Replace("<skill>", "%s");
			sResult.Format(sFmt, mod.aValues[0], dsi.szSpellName, mod.aValues[2], mod.aValues[3]);
			return MOD_OTHER;
		}
	}

	if (mod.dwModID == 107)
	{
		D2SPELLINFO dsi = { 0 };
		if (D2GetSpellInfo(mod.aValues[1], &dsi))
		{
			CString sFmt = mod.szFmt;
			sFmt.Replace("<skill>", "%s");
			sFmt.Replace("<class>", "%s");
			char szClass[64] = "";
			D2GetClassName(dsi.nClass, szClass, 63);

			sResult.Format(sFmt, mod.aValues[0], dsi.szSpellName, szClass);
			return MOD_OTHER;
		}
	}

	if (mod.dwModID >= 195 && mod.dwModID <= 201)
	{
		D2SPELLINFO dsi = { 0 };
		if (D2GetSpellInfo(mod.aValues[2], &dsi))
		{
			CString sFmt = mod.szFmt;
			sFmt.Replace("<skill>", "%s");
			sResult.Format(sFmt, mod.aValues[0], mod.aValues[1], dsi.szSpellName);
			return MOD_OTHER;
		}
	}

	if (mod.nValueCount == 1)
	{
		if (mod.bFloat)
		{
			double f = (double)mod.aValues[0];
			f /= 1000.0;
			CString s;
			s.Format("%f", f);
			s.TrimRight('0');
			s.TrimRight('.');
			CString sFmt(mod.szFmt);
			sFmt.Replace("%.3f", "%s");
			sResult.Format(sFmt, (LPCSTR)s);
		}
		else
		{
			sResult.Format(mod.szFmt, mod.aValues[0]);
			if (mod.dwModID == 127)
				return MOD_ALLSKILL;
			else if (mod.dwModID == 0)
				return MOD_STR;
			else if (mod.dwModID == 1)
				return MOD_ENG;
			else if (mod.dwModID == 2)
				return MOD_DEX;
			else if (mod.dwModID == 3)
				return MOD_VITA;
			else if (mod.dwModID == 39)
				return MOD_FIRE;	
			else if (mod.dwModID == 41)
				return MOD_LIT;	
			else if (mod.dwModID == 43)
				return MOD_COLD;	
			else if (mod.dwModID == 45)
				return MOD_POI;	
		}
	}
	else if (mod.nValueCount == 2)
	{
		sResult.Format(mod.szFmt, mod.aValues[0], mod.aValues[1]);
	}
	else if (mod.nValueCount == 3)
	{
		sResult.Format(mod.szFmt, mod.aValues[0], mod.aValues[1], mod.aValues[2]);
	}
	else if (mod.nValueCount == 4)
	{
		sResult.Format(mod.szFmt, mod.aValues[0], mod.aValues[1], mod.aValues[2], mod.aValues[3]);
	}
	else
		return MOD_INVALID;

	return MOD_OTHER;
}

void CD2ItemData::CalcHoverSize(CWnd *pDlg)
{
	::memset(&czHover, 0, sizeof(SIZE));
	if (pDlg == NULL)
		return;

	CDC* pDC = pDlg->GetDC();
	if (pDC == NULL)
		return;

	int i = 0;
	for (i = 0; i < m_aTitle.GetSize(); i++)
		UpdateCxCy(m_aTitle[i], czHover.cx, czHover.cy, pDC);

	if (!bIdentified)
		UpdateCxCy(g_bLanguage ? "未鉴定的" : "Unidentified", czHover.cx, czHover.cy, pDC);

	for (i = 0; i < m_aHead.GetSize(); i++)
		UpdateCxCy(m_aHead[i], czHover.cx, czHover.cy, pDC);

	for (i = 0; i < m_aMods.GetSize(); i++)
		UpdateCxCy(m_aMods[i], czHover.cx, czHover.cy, pDC);

	for (i = 0; i < m_aFoot.GetSize(); i++)
		UpdateCxCy(m_aFoot[i], czHover.cx, czHover.cy, pDC);
	
	pDlg->ReleaseDC(pDC);
}

void CD2ItemData::UpdateCxCy(const CString &str, long &cx, long &cy, CDC *pDC)
{
	if (pDC == NULL || str.IsEmpty())
		return;

	CSize cz = pDC->GetTextExtent(str);
	cx = max(cx, cz.cx);
	cy += cz.cy + LINE_GAP;
}

void CD2ItemData::DrawItem(CDC *pDC, const CRect &rect)
{
	if (pDC == NULL || czImage.cx == 0)
		return;

	POINT pt;
	pt.x = rect.left + (rect.Width() - czImage.cx) / 2;
	pt.y = rect.top + (rect.Height() - czImage.cy) / 2;
	image.Draw(pDC, pt);
}

COLORREF CD2ItemData::GetDescColor() const
{
	return GetDescColor(dwQualityID, bEthereal, nSockets, bRuneword, bQuestItem);
}

void CD2ItemData::InsertStr(const CString &str)
{
	if (!str.IsEmpty())
		m_aMods.InsertAt(0, str);
}

BOOL CD2ItemData::IsValSame(int n1, int n2, int n3, int n4)
{
	int nAll = n1 + n2 + n3 + n4;
	nAll /= 4;

	return nAll > 0 && nAll == n1 && nAll == n2 && nAll == n3 && nAll == n4;
}

BOOL CD2ItemData::IsBlund(DWORD dwTypeID)
{
	for (int i = 0; BLUNDS[i] > 0; i++)
	{
		if (BLUNDS[i] == dwTypeID)
			return TRUE;
	}
	return FALSE;
}

BOOL CD2PlayerData::AddItem(CD2ItemData *pItem)
{
	if (pItem == NULL)
		return FALSE;

	if (m_aItemList.Find(pItem) == NULL)
		m_aItemList.InsertRear(pItem);	
	
	return TRUE;
}

BOOL CD2PlayerData::RemoveItem(DWORD dwItemID)
{
	POSITION pos = m_aItemList.GetHeadPosition();
	while (pos)
	{
		POSITION cur = pos;
		CD2ItemData* p = m_aItemList.GetNext(pos);
		if (p->dwItemID == dwItemID)
		{
			m_aItemList.RemoveAt(cur);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CD2PlayerData::RemoveItem(int nStorageType, int x, int y)
{
	POSITION pos = m_aItemList.GetHeadPosition();
	while (pos)
	{
		POSITION cur = pos;
		CD2ItemData* p = m_aItemList.GetNext(pos);
		if (p->dwStorageID == (DWORD)nStorageType && p->x == (BYTE)x && p->y == (BYTE)y)
		{
			m_aItemList.RemoveAt(cur);
			return TRUE;
		}
	}

	return FALSE;
}

CD2ItemData* CD2PlayerData::FindItem(int nStorageID, int x, int y)
{
	POSITION pos = m_aItemList.GetHeadPosition();
	while (pos)
	{
		POSITION cur = pos;
		CD2ItemData* p = m_aItemList.GetNext(pos);
		if (p->dwStorageID != (DWORD)nStorageID)
			continue;
		
		if (nStorageID == 10 || (p->x == (BYTE)x && p->y == (BYTE)y))
			return p;
	}

	return FALSE;
}

UINT CD2ItemData::FindUnknownResource(BYTE cx, BYTE cy)
{
	UINT nRes = 0;
	WORD wTemp = MAKEWORD(cy, cx);
	switch (wTemp)
	{
	case 0x0101:
		nRes = IDB_IMG_UNK11;
		break;

	case 0x0102:
		nRes = IDB_IMG_UNK12;
		break;

	case 0x0103:
		nRes = IDB_IMG_UNK13;
		break;

	case 0x0104:
		nRes = IDB_IMG_UNK14;
		break;

	case 0x0201:
		nRes = IDB_IMG_UNK21;
		break;

	case 0x0202:
		nRes = IDB_IMG_UNK22;
		break;

	case 0x0203:
		nRes = IDB_IMG_UNK23;
		break;

	case 0x0204:
		nRes = IDB_IMG_UNK24;
		break;

	default:
		break;
	}

	return nRes;
}

BOOL CD2ItemData::IsSpecialCode(LPSTR lpsz) const
{
	if (lpsz == NULL)
		return FALSE;

	lpsz[0] = 0;
	
	// Special cases:
	// mbl Hwanin's Blessing
	// uar Tyrael's might
	// tbl Iratha's Cord
	// tgl Iratha's Cuff
	// crn Milabrega's Diadem

	if (sCode == "mbl" && dwQualityID == 5)
	{
		// Prevent Monster Heal
		if (FindMod(117))
			::strcpy(lpsz, "tmbl");			
	}
	else if (sCode == "uar" && dwQualityID == 7)
	{
		// Slain Monsters Rest in Peace
		if (FindMod(108))
			::strcpy(lpsz, "vuar");
	}
	else if (sCode == "tbl" && dwQualityID == 5)
	{
		// +5 To Minimum Damage
		if (FindMod(21) || FindMod(23) || FindMod(159))
			::strcpy(lpsz, "ttbl");
		
	}
	else if (sCode == "tgl" && dwQualityID == 5)
	{
		// Half Freeze Duration
		if (FindMod(118))
			::strcpy(lpsz, "ttgl");
	}
	else if (sCode == "crn" && dwQualityID == 5)
	{
		// +15 to Life
		if (FindMod(7))
			::strcpy(lpsz, "tcrn");
	}

	return ::strlen(lpsz);
}

BOOL CD2ItemData::FindMod(DWORD dwModID) const
{
	for (int i = 0; i < nModCount; i++)
	{
		if (aMods[i].dwModID == dwModID)
			return TRUE;
	}
	return FALSE;
}

BOOL CD2ItemData::LoadImageResource(CResPack* pPack)
{
	if (pPack == NULL)
		return FALSE;

	char szCode[16] = "";	
	if (sCode == "cm1" && dwQualityID == 7)
	{
		::strcpy(szCode, "ucm1"); // annihilus
	}
	else if (IsIndexedItem(sCode))
	{
		// indexed items, rings/amulets/charms
		 ::sprintf(szCode, "%s%d", sCode, nArtwork);
	}
	else if (!IsSpecialCode(szCode))
	{		
		if (dwQualityID == 5) // set
			::sprintf(szCode, "s%s", (LPCSTR)sCode);
		else if (dwQualityID == 7)
			::sprintf(szCode, "u%s", (LPCSTR)sCode);
		else
			::strcpy(szCode, sCode);

	}

	const int LEN = ::strlen(szCode);
	if (LEN < 3)
		return FALSE;

	BYTE aData[10000];
	DWORD dwExtracted = pPack->Extract(*((DWORD*)szCode), aData, 10000);
	if (!dwExtracted && LEN == 4)
		dwExtracted = pPack->Extract(*((DWORD*)(szCode + 1)), aData, 10000);

	if (!dwExtracted)
		return FALSE;
	
	return image.Load(aData, dwExtracted);
}

void CD2ItemData::LoadUnknownResource(CResPack* pPack)
{
	UINT nRes = FindUnknownResource(cx, cy);
	if (nRes == 0)
		return;

	BYTE aData[1024];
	DWORD dwResSize = pPack->Extract(nRes, aData, 1024);
	image.Load(aData, dwResSize);
}

BOOL CD2ItemData::IsQuestItem(LPCSTR lpszCode)
{
	if (lpszCode == NULL)
		return FALSE;

	for (int i = 0; QUESTITEMS[i]; i++)
	{
		if (::memcmp(lpszCode, QUESTITEMS[i], 3) == 0)
			return TRUE;
	}

	return FALSE;
}

BOOL CD2ItemData::IsIndexedItem(LPCSTR lpszCode)
{
	if (lpszCode == NULL)
		return FALSE;

	for (int i = 0; INDEXITEMS[i]; i++)
	{
		if (::memcmp(lpszCode, INDEXITEMS[i], 3) == 0)
			return TRUE;
	}

	return FALSE;
}

COLORREF CD2ItemData::GetDescColor(DWORD dwQuality, BOOL bEthereal, int nSockets, BOOL bRuneword, BOOL bQuestItem)
{
	if (dwQuality == 4)
		return RGB(80, 80, 172);
	else if (dwQuality == 5)
		return RGB(24, 252, 0);
	else if (dwQuality == 6)
		return RGB(216, 184, 100);
	else if (dwQuality == 7 || bRuneword || bQuestItem)
		return RGB(148, 128, 100);
	else if (dwQuality == 8)
		return RGB(208, 132, 32);
	else if (bEthereal || nSockets > 0)
		return RGB(80, 80, 80);
	else
		return RGB(196, 196, 196);
}

CD2ItemDesc::CD2ItemDesc()
{
	dwID = 0;
	dwLevel = 0;
	nSockets = 0;
	bEthereal = FALSE;
	bRuneword = FALSE;
	bQuest = FALSE;
	descColor = RGB(196, 196, 196);
}

CD2ItemDesc::CD2ItemDesc(const CD2ItemDesc& rhs)
{
	dwID = rhs.dwID;
	dwLevel = rhs.dwLevel;
	nSockets = rhs.nSockets;
	bEthereal = rhs.bEthereal;
	bRuneword = rhs.bRuneword;
	bQuest = rhs.bQuest;
	descColor = rhs.descColor;
	sName = rhs.sName;
	sCode = rhs.sCode;
	sQlyName = rhs.sQlyName;
	sType = rhs.sType;
	sLocation = rhs.sLocation;
	sPlayer = rhs.sPlayer;
	sAccount = rhs.sAccount;
	sRealm = rhs.sRealm;
}

const CD2ItemDesc& CD2ItemDesc::operator=(CD2ItemDesc& rhs)
{
	if (this == &rhs)
		return *this;

	dwID = rhs.dwID;
	dwLevel = rhs.dwLevel;
	nSockets = rhs.nSockets;
	bEthereal = rhs.bEthereal;
	bRuneword = rhs.bRuneword;
	bQuest = rhs.bQuest;
	descColor = rhs.descColor;
	sName = rhs.sName;
	sCode = rhs.sCode;
	sQlyName = rhs.sQlyName;
	sType = rhs.sType;
	sLocation = rhs.sLocation;
	sPlayer = rhs.sPlayer;
	sAccount = rhs.sAccount;
	sRealm = rhs.sRealm;
	return *this;
}

BOOL CD2ItemDesc::operator==(CD2ItemDesc& rhs) const
{
	return dwID == rhs.dwID;
}

BOOL CD2ItemDesc::operator>(CD2ItemDesc& rhs) const
{
	return dwID > rhs.dwID;
}

BOOL CD2ItemDesc::FromDatabase(CADORecordset *pRS)
{
	if (pRS == NULL || pRS->IsEof())
		return FALSE;

	DWORD dwTemp;
	pRS->GetFieldValue("iid", dwID);
	pRS->GetFieldValue("itemtitle", sName);
	pRS->GetFieldValue("icode", sCode);
	pRS->GetFieldValue(g_bLanguage ? "storagenamechs" : "storagenameeng", sLocation);
	pRS->GetFieldValue(g_bLanguage ? "typenamechs" : "typenameeng", sType);
	pRS->GetFieldValue("pname", sPlayer);
	pRS->GetFieldValue("aname", sAccount);
	pRS->GetFieldValue("rname", sRealm);
	pRS->GetFieldValue("ilevel", dwLevel);
	pRS->GetFieldValue(g_bLanguage ? "qualitynamechs" : "qualitynameeng", sQlyName);
	pRS->GetFieldValue("isockets", nSockets);
	pRS->GetFieldValue("iflags", dwTemp);
	bQuest = CD2ItemData::IsQuestItem(sCode);
	bEthereal = dwTemp & 0x00400000;
	bRuneword = dwTemp & 0x04000000;	
	sName.Replace('^', '\'');
	sName.Replace('|', ' ');
	pRS->GetFieldValue("qualityid", dwTemp);
	descColor = CD2ItemData::GetDescColor(dwTemp, bEthereal, nSockets, bRuneword, bQuest);
	return TRUE;
}

void CD2ItemDesc::CopyData(const CD2ItemData *pData)
{
	sName.Empty();
	sPlayer.Empty();
	sRealm.Empty();
	sAccount.Empty();

	if (pData == NULL)
		return;
		
	dwID = pData->dwItemID;
	dwLevel = pData->dwiLevel;
	nSockets = pData->nSockets;
	bEthereal = pData->bEthereal;
	bRuneword = pData->bRuneword;
	bQuest = pData->bQuestItem;
	descColor = pData->GetDescColor();
	sName = pData->sName;
	sCode = pData->sCode;
	sQlyName = pData->sQualityName;
	sType = pData->sTypeName;
	sLocation = pData->sLocation;
}
