//*******************************************************************
//*																	*
//* Programme: ressort.h	 			 							*
//*																	*
//*******************************************************************
//*																	*
//* Description:													*
//*		D�finition d'un ressort pour le r�seau de masses-ressorts	*
//*                                                                 *
//*******************************************************************

#pragma once
#include "masse.h"

class ressort
{
private:
	int idmasse1; // Index masse 1
	int idmasse2; // Index masse 2
	reel ConstRaideur;  // Constante de rigidit� du ressort
	reel LongueurRepos; //Longueur au repos

public:
	ressort(void);
	~ressort(void);
	void setconstRaideur(const reel& ka);
	reel GetconstRaideur();
	void setLongueurRepos(const reel& l0);
	reel GetLongueurRepos();
	void setidmasse1(const int &masseid);
	int Getmasse1id();
	void setidmasse2(const int &masseid);
	int Getmasse2id();
};
