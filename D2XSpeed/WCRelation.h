/****************************************************
*       WCRelation.h                                *
* Declaration of WCRelation class                   *
* 2001-12-17 Bin Liu                                *
* All rights reserved.                              *
*****************************************************/

#ifndef WCRELATION_H
#define WCRELATION_H

#include "D2XConsts.h"
#include "Weapon.h"

// ------------- class IasUnit --------------------------

// used to computer IAS table
class IasUnit
{
public:
	IasUnit(): m_nIAS(-1), m_nSpeed(-1) {}
	void Set(int nIAS, int nSpeed) { m_nIAS = nIAS; m_nSpeed = nSpeed; }
	int m_nIAS;
	int m_nSpeed;
};


// --------------- class WCRelation ----------------------

// weapon-character relation
class WCRelation
{
public:
	int Speed() const;
	BOOL Lightning_able() const;
	BOOL IsSameCharClass(LPCTSTR lpszCharClass) const;
	BOOL IsSameWeapon(LPCTSTR lpszWeaponName) const;

	// constructors
	WCRelation();
	WCRelation(LPCTSTR lpszCharClass, LPCTSTR lpszWeaponName);

	// accesses
	int GetCharClass() const;
	Weapon GetWeapon() const;
	float GetBase() const;
	int GetWIas() const;
	int GetNWIas() const;
	int GetIas() const;
	int GetFana() const;
	int GetBos() const;
	int GetFrenzy() const;
	int GetWerewolf() const;
	int GetSIas() const;
	int GetAttackForm(void) const;

	int SetFana(int nFana);
	int SetBos(int nBos);
	int SetFrenzy(int nFrenzy);
	int SetCharClass(LPCTSTR lpszCharClass);
	int SetWerewolf(int nWerewolf);
	Weapon SetWeapon(LPCTSTR lpszWeaponName);
	int SetAttackForm(LPCTSTR lpszAttackForm);
	int SetWIas(int nWIas);
	int SetNWIas(int nNWIas);

	// condition checkings
	BOOL Zeal_able() const;
	BOOL Throw_able() const;
	BOOL Smite_able() const;
	BOOL Strafe_able() const;
	BOOL Were_able() const;
	BOOL Frenzy_able() const;
	BOOL Bos_able() const;
	BOOL Fury_able() const;
	BOOL IsIasDistinct() const;

	// determinations
	float CalcBase() const;
	int GetTable(IasUnit* tableArray) const;
	static int DimiIas(int nIas);
	static int RevIas(int nDimiIas);
	int EIas() const;
	int Speed(int nEIasVal) const;

	// provider
	static int GetCharNameList(char* nameArray[]);
	static int GetWeaponFilterList(char* nameArray[]);
	static int GetWeaponNameList(char* nameArray[], LPCTSTR lpszFilter = TF_ALL);
	int GetAttackFormList(char* nameArray[]);

protected:

	// member variables
	int m_nCharClass; // character class
	Weapon m_weapon; // weapon
	float m_fBase; // base speed
	int m_nAttackForm; // attack form
	int m_nWIas; // IAS from weapon
	int m_nNWIas; // IAS from items other than weapon
	int m_nFana; // IAS from fanaticism aura
	int m_nBos; // IAS from Burst of Speed skill
	int m_nFrenzy; // IAS from Frenzy skill
	int m_nWerewolf; // IAS from Werewolf skill
	BOOL CheckAttackForm(); // adjust m_nAttackForm
};

#endif