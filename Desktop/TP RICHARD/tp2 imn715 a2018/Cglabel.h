//*******************************************************************

//*																	*

//* Programme: cgLabel.h											*

//*																	*

//*******************************************************************

//*																	*

//* Description:													*

//*		Implantation d'�tiquettes. Les �tiquettes sont simplement	*

//*		la repr�sentation graphique de textes.						*

//*																	*

//*******************************************************************



#ifndef _cgLABEL

#define _cgLABEL



#include "cgControl.h"

#include "cgcString.h"



class cgLabel : public cgControl

{

protected:

	cgcString m_texte;



	int x,y, w,h;



public:

	cgLabel(int left, int top, cgcString texte, booleen transparent = FAUX);

	~cgLabel();



	cgcString GetCaption();

	void SetCaption(cgcString texte);



	//Messages qui peuvent �tre capt�s

	virtual void OnDraw();

};



#endif

