//*******************************************************************

//*																	*

//* Programme: chaine.h												*

//*																	*

//*******************************************************************

//*																	*

//* Description:													*

//*		D�finition des chaines de caract�res						*

//*																	*

//*******************************************************************


#ifndef _CHAINE
#define _CHAINE

#include "definition.h"

class	chaine 

{
	char	s[100];
	short	i;


public:
	chaine();
	chaine(char *);

	char *	C();

	booleen	operator==(chaine &);
};

#endif

