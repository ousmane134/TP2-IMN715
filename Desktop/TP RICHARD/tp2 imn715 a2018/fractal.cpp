
#include "fractal.h"
#include "geo.h"
#include "canon.h"
#include "util.h"
#include "utilalg.h"
#include "alg.h"
#include "definition.h"
#include "attr.h"
#include "englobe.h"


point	Fractal::milieu(const point& p1, const point& p2, const reel& Hmin, const reel& Hmax) const
{
// Retourne le point milieu perturb�.
// Cette m�thode devrait �tre d�finie et utilis�e pour l'intersection d'un rayon avec
// un fractal ainsi que lors de l'affichage du fractal
	reel    x1,y1,z1,x2,y2,z2;
    reel x, seed;


	p1.coordonnees(&x1,&y1,&z1);
	p2.coordonnees(&x2,&y2,&z2);

	seed = (x1 + x2)*5.2 + (y1 + y2)*0.5 + (z1 + z2)*8 + PI;
	seed = seed * seed * seed *seed * seed;
	seed = seed - (int) seed;
	x = seed * (Hmax - Hmin) + Hmin;
	return point(x + (x1+x2)/2, (y1+y2)/2, (z1+z2)/2);
}
 
 
reel	Fractal::hinf () const
{
    return h1;
}

reel	Fractal::hsup () const
{
    return h2;
}


int	Fractal::niv_sub () const
{
    return nivsub;
}


 
point	Fractal::p1 () const
{
    return pt1;
}

point	Fractal::p2 () const
{
    return pt2;
}

point	Fractal::p3 () const
{
    return pt3;
}

Fractal::Fractal ()
{
	s = unFractal;
}

Fractal::Fractal (const point&p1, const point&p2, const point&p3, const int& n, const reel& hinf, const reel& hsup)
{
	s = unFractal;

	pt1 =p1;
	pt2 = p2;
	pt3 = p3;
	nivsub=n;
	h1 = hinf;
	h2 = hsup;
}

Objet*	Fractal::copie () const
{
	Fractal	*o = new Fractal;

	*o->attr = *this->attr;
	o->defvol = this->defvol;
	o->nivsub = this->nivsub;
	o->pt1 = this->pt1;
	o->pt2 = this->pt2;
	o->pt3 = this->pt3;
	o->h1 = this->h1;
	o->h2 = this->h2;
	o->normal = this->normal;

	if (this->defvol) o->vol = this->vol->copie();
	return(o);
}

booleen Inter_Triangle(const point& p1, const point& p2,const point& p3, const point& p, const vecteur& v, reel *k, vecteur *normal)
// Retourne VRAI s'il y a intersection avec le rayon p,v et le triangle p1,p2,p3. Retourne aussi k; le point d'intersection sera � p + *k * v.
// Le vecteur normal est aussi retourn�.
   
{

	plan	pl(p1,p2,p3);

	point	pi;
	reel k2;

	k2 = *k;
	if (!pl.inter (p,v, &pi, &k2)) return FAUX;
	if (*k < k2) return FAUX;
	*k = k2;

	vecteur v1(p1,p2);
	vecteur v2(p2,p3);
	vecteur v3(p3,p1);

	vecteur u1(p1,pi);
	vecteur u2(p2,pi);
	vecteur u3(p3,pi);

	vecteur n1 = v1^u1;
	vecteur n2 = v2^u2;
	vecteur n3 = v3^u3;

	n1.normalise();
	n2.normalise();
	n3.normalise();

	if (n1*n2 < 0.0 || n2*n3 < 0.0 || n1*n3 < 0.0) return FAUX;
	else {
		*normal = pl.normale ();
		reel cost = v * *normal;
		if (cost > 0) *normal = -*normal;
		return VRAI;
	}
}
booleen Inter_Cube(const point & p, const vecteur& v, const reel& xmin, const reel & xmax,const reel& ymin,const reel& ymax,const reel& zmin,const reel& zmax)
// Retourne VRAI s'il y a intersection du rayon p,v avec le cube canonique xmin, xmax, ymin, ymax, zmin, zmax
   {
   Cube cube(Deplacement(point(xmin,ymin,zmin) - O) *
			 Homothetie (O,X, xmax-xmin) *
			 Homothetie (O,Y, ymax-ymin) *
			 Homothetie (O,Z, zmax-zmin));
   return cube.inter(p,v);
   }

reel min (reel x, reel y)
   {
   if (x<y)
	  return x;
   else
	  return y;
   }

reel max (reel x, reel y)
   {
   if (x>y)
	  return x;
   else
	  return y;
   }

booleen Inter_Fractal(const Fractal& o, const point& p1, const point& p2, const point& p3, int niv, reel h1, reel h2, const point& p, const vecteur& v, reel* k, vecteur *vn)
   {
   		reel k2;
		vecteur vn2;

	if (niv>0) 
	{

//		construction du cube englobant
	    reel xmin, xmax, ymin,ymax,zmin,zmax;
		reel x1,x2,x3,y1,y2,y3,z1,z2,z3;
		p1.coordonnees(&x1,&y1,&z1);
		p2.coordonnees(&x2,&y2,&z2);
		p3.coordonnees(&x3,&y3,&z3);
		xmin = min(min(x1,x2),x3);
		xmax = max(max(x1,x2),x3);
		ymin = min(min(y1,y2),y3);
		ymax = max(max(y1,y2),y3);
		zmin = min(min(z1,z2),z3);
		zmax = max(max(z1,z2),z3);

		point	p1p2 = o.milieu(p1,p2,h1,h2);
		point	p2p3 = o.milieu(p2,p3,h1,h2);
		point	p3p1 = o.milieu(p3,p1,h1,h2);

		reel x12 = p1p2.x();
		reel x23 = p2p3.x();
		reel x31 = p3p1.x();

		xmin = min (xmin,x12+h1);
		xmin = min (xmin,x23+h1);
		xmin = min (xmin,x31+h1);

		xmax = max (xmax,x12+h2);
		xmax = max (xmax,x23+h2);
		xmax = max (xmax,x31+h2);
	    

		if (!Inter_Cube( p,v,xmin,xmax,ymin,ymax,zmin,zmax))
		   return FAUX;

	    booleen ret = FAUX;

		k2 = *k;
		if (Inter_Fractal (o, p1,p1p2,p3p1, niv-1, h1/2, h2/2, p, v, &k2, &vn2)) 
		   {
			*vn = vn2;
			*k = k2;
		    ret = VRAI;
		   }

		k2 = *k;
		if (Inter_Fractal (o, p1p2,p2,p2p3, niv-1, h1/2, h2/2, p, v, &k2, &vn2))
		   {
			*vn = vn2;
			*k = k2;
		    ret = VRAI;
		   }

		k2 = *k;
		if (Inter_Fractal (o, p3p1,p2p3,p3, niv-1, h1/2, h2/2, p, v, &k2, &vn2))
		   {
			*vn = vn2;
			*k = k2;
		    ret = VRAI;
		   }

		k2 = *k;
		if (Inter_Fractal (o, p1p2,p2p3,p3p1, niv-1, h1/2, h2/2, p, v, &k2, &vn2))
		   {
			*vn = vn2;
			*k = k2;
		    ret = VRAI;
		   }

		return ret;

	}
	else 
	   {
	    k2 = *k;
	    if (Inter_Triangle(p1,p2,p3,p,v,&k2,&vn2))
		   {
		   *vn = vn2;
		   *k = k2;
		   return VRAI;
		   }
		return FAUX;
	   }

   }


booleen	Fractal::inter(const point& p, const vecteur& v, reel* k)
{
	Transformation t = attributs().transformation();

	point	pt2 = t.inverse(p);
	vecteur	v2 = t.inverse(v);

	int nbiter = niv_sub(); // le niveau de subdivision du fractal

	vecteur vn2;
	reel k2;
	k2= *k;
	if(Inter_Fractal (*this, p1(),p2(),p3(), nbiter, hinf(), hsup(), pt2, v2, &k2, &vn2))
	   {
	   normal = vn2;
	   *k = k2;
	   return VRAI;
	   }
	else
	   return FAUX;


}

vecteur	Fractal::gradient(const point&p)
{
	return normal;
}

point	Fractal::unpoint()
{
	return pt1;
}

const	Englobant&    Fractal::volume() const
{
	if (!defvol) {
		((Objet*)this)->vol = new Infini();
//		Vous pouvez mettre " = new Cube (transformation);" pour d�finir un volume
//		englobant pour le fractal comme tel, mais ce n'est pas exig� pour le tp2.
//      Voir la procedure Inter_Cube ci-dessus pour figurer la transformation en
//		param�tre.
		((Objet*)this)->defvol = VRAI;
	}
	return *vol;
}
 
