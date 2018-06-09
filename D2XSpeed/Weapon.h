/****************************************************
*       Weapon.h                                    *
* Declaration of Weapon class                       *
* 2001-12-17 Bin Liu                                *
* All rights reserved.                              *
*****************************************************/

#ifndef WEAPON_H
#define WEAPON_H

// the weapon class
class Weapon  
{
public:

	// constructors
	Weapon();
	Weapon(LPCTSTR lpszName);

	// accesses
	int GetWsm() const;
	int GetCategory() const;
	int GetRange() const;
	CString GetName() const;

	// operator overloading
	BOOL operator==(const Weapon& rhs) const;

protected:

	// determinations
	int CalcWsm() const;
	int CalcCategory() const;
	int CalcRange() const;

	// member variables
	CString m_strName; // weapon displaying name
	int m_nCategory; // weapon category
	int m_nWsm; // build-in WSM value
	int m_nRange; // weapon range
};

#endif