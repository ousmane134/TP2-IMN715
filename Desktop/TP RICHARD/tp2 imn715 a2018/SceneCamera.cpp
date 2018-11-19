#include "SceneCamera.h"

#include "scene.h"

#include "fichier.h"

#include "io.h"

#include "string.h"

#include "Spotlight.h"
#include "Ponctuelle.h"



SceneCamera::SceneCamera() 

{

}



SceneCamera::~SceneCamera()

{

}



SceneCamera::SceneCamera(const SceneCamera& s)

{

	m_Camera = s.m_Camera;

	m_Designateur = s.m_Designateur;



	m_pscene = (Ensemble*)s.m_pscene->copie();

	m_pScene = (Ensemble*)s.m_pscene->copie();

}



void SceneCamera::operator=(const SceneCamera& s)

{

	m_Camera = s.m_Camera;

	m_Designateur = s.m_Designateur;



	m_pscene = (Ensemble*)s.m_pscene->copie();

	m_pScene = (Ensemble*)s.m_pscene->copie();

}



void SceneCamera::DrawScene()

{

	Affiche_Scene(*Scene(),ATTR);

}



booleen SceneCamera::LoadScene(char* fichier)

{

	m_pscene = new Ensemble();

	m_pScene = new Ensemble();

	m_pScene->ajoute(m_pscene);



	if ( !LireSCN(fichier) ) return FAUX;

	if ( !LireVSN(fichier) ) return FAUX;



	return VRAI;

}



booleen SceneCamera::LireVSN(char fichier[])

{

	Fichier f;

	char s[256];



	sprintf(s, "%s.vsn", fichier);

	if ( !f.Open(s, "r") ) return FAUX;



	printf("Lecture du .vsn en cours...\n");



	point	pt;

	vecteur	v;

	Lire_point(f, &pt);

	m_Camera.PR (pt);

	printf("  PR <%f,%f,%f>\n",pt.x(),pt.y(),pt.z());

	Lire_vecteur(f, &v);

	m_Camera.VN (v);

	printf("  VN <%f,%f,%f>\n",v.x(),v.y(),v.z());

	Lire_vecteur(f, &v);

	m_Camera.VH (v);

	printf("  VH <%f,%f,%f>\n",v.x(),v.y(),v.z());



	reel r;

	if ( !f.mreel(r) ) { printf("Reel attendu\n"); exit(1); }

	m_Camera.DO (r);

	printf("  do %f\n",r);

	if ( !f.mreel(r) ) { printf("Reel attendu\n"); exit(1); }

	m_Camera.DV (r);

	printf("  dv %f\n",r);

	if ( m_Camera.DV() < m_Camera.DO() ) 

    {

		printf("DV est plus petit que DO. Il faut que DV soit plus grand que DO\n");

		exit(1);

	}



	double v1,v2,v3,v4;

	if ( !f.mreel(v1) ) { printf("Reel attendu\n"); exit(1); }

	if ( !f.mreel(v2) ) { printf("Reel attendu\n"); exit(1); }

	if ( !f.mreel(v3) )	{ printf("Reel attendu\n"); exit(1); }

	if ( !f.mreel(v4) )	{ printf("Reel attendu\n"); exit(1); }

	printf("  FENETRE: %f,%f,  %f,%f\n",v1,v2,v3,v4);

	m_Camera.fenetre(v1,v2,v3,v4);



	if ( !f.mreel(v1) ) { printf("Reel attendu\n"); exit(1); }

	if ( !f.mreel(v2) ) { printf("Reel attendu\n"); exit(1); }

	if ( !f.mreel(v3) ) { printf("Reel attendu\n"); exit(1); }

	if ( !f.mreel(v4) ) { printf("Reel attendu\n"); exit(1); }

	m_Camera.cloture (v1,v2,v3,v4);

	printf("  CLOTURE: %f,%f,  %f,%f\n",v1,v2,v3,v4);



	if ( !f.mreel(r) ) { printf("Reel attendu\n"); exit(1); }

	m_Camera.DH (r);

	printf("  dh %f\n",r);

	if ( !f.mreel(r) && !f.fin() ) { printf("Reel attendu\n"); exit(1); }

	m_Camera.DY (r);

	printf("  dy %f\n",r);



	m_Camera.perspective(VRAI);



	//Lecture des lumieres

	entier nbLum = 0;

    booleen Spot = FAUX;

	while ( !f.fin() && nbLum < 8)

	{
        //Lecture du type de lumiere

        chaine Type;

        if ( !f.mchaine( Type ) )
        {
            if ( !f.fin() )
            {
                printf( "Type de lumiere attendu\n" );
                exit(1);
            }
            else
                break;
        }

        if ( Type == chaine( "SP" ) )
            Spot = VRAI;
        else
            if ( Type == chaine( "PC" ) )
                Spot = FAUX;
            else
            {
                printf("Type de lumi�re invalide\n"); 
                exit(1);
            }

        

		//Lecture de la position de la source

		double v1,v2,v3;

		if ( !f.mreel(v1) )

		{

			if ( !f.fin() ) { printf("Reel attendu\n"); exit(1); } else break;

		}

		if ( !f.mreel(v2) ) { printf("Reel attendu\n"); exit(1); }

		if ( !f.mreel(v3) ) { printf("Reel attendu\n"); exit(1); }



		pt = point (v1,v2,v3);

        printf("  \nLumiere %d %s\n",nbLum+1, ( Spot ? "(Spotlight)" : "" ));

		printf("  POS <%f,%f,%f>\n",v1,v2,v3);



		//Lecture de la couleur diffuse de la source

		reel rd,gd,bd;

		if ( !f.mreel(rd) ) { printf("Reel attendu\n"); exit(1); }

		if ( !f.mreel(gd) ) { printf("Reel attendu\n"); exit(1); }

		if ( !f.mreel(bd) ) { printf("Reel attendu\n"); exit(1); }

		printf("  Couleur diffuse <%f,%f,%f>\n",rd,gd,bd);



		//Lecture de la couleur ambiante de la source

		reel ra,ga,ba;

		if ( !f.mreel(ra) ) { printf("Reel attendu\n"); exit(1); }

		if ( !f.mreel(ga) ) { printf("Reel attendu\n"); exit(1); }

		if ( !f.mreel(ba) ) { printf("Reel attendu\n"); exit(1); }

		printf("  Couleur ambiante <%f,%f,%f>\n",ra,ga,ba);


        // Lecture de l'intensit� des photons

        reel rph, gph, bph;

		if ( !f.mreel(rph) ) { printf("Reel attendu\n"); exit(1); }

		if ( !f.mreel(gph) ) { printf("Reel attendu\n"); exit(1); }

		if ( !f.mreel(bph) ) { printf("Reel attendu\n"); exit(1); }

        printf("   Energie des photons <%f,%f,%f>\n",rph,gph,bph);


        if ( !Spot )
        {
            Ponctuelle* lum = new Ponctuelle( uneLumierePonctuelle, pt, ra,ga,ba, rd,gd,bd, rph, gph, bph );
    		m_Camera.AddLumiere(lum);
        }
        else
        {

            // Lecture de la direction du spotlight

            reel dx, dy, dz;

		    if ( !f.mreel(dx) ) { printf("Reel attendu\n"); exit(1); }

		    if ( !f.mreel(dy) ) { printf("Reel attendu\n"); exit(1); }

		    if ( !f.mreel(dz) ) { printf("Reel attendu\n"); exit(1); }

            vecteur v( dx, dy, dz );

            printf( "Direction du spotlight <%f, %f, %f>\n", dx ,dy, dz );


            // Lecture de l'angle d'ouverture et du coefficient de degradation du spotlight

            reel angle;

		    if ( !f.mreel(angle) ) { printf("Reel attendu\n"); exit(1); }

            printf( "Angle d'ouverture : %f\n", angle );


            Spotlight* lum = new Spotlight( unSpotlight, pt, ra, ga, ba, rd, gd, bd, rph, gph, bph, v, angle );
            m_Camera.AddLumiere( lum );
        }


		nbLum++;

	}



	f.Close();



	printf("Lecture du .vsn terminee\n");

	reel	d0 = m_Camera.DO ();

	reel	dv = m_Camera.DV ();

	reel    hmin = m_Camera.Hmin ();

	reel    hmax = m_Camera.Hmax ();

//	m_Camera.Pas(0);



	return VRAI;

}



booleen SceneCamera::LireSCN(char fichier[])

{

	char s[256];

	Fichier f;



	sprintf(s, "%s.scn", fichier);

	if (!f.Open(s, "r")) return FAUX;



	printf("Lecture du .scn en cours...\n");

	Objet	*o = Objet::Lecture(f);

	printf("Lecture du .scn termine\n\n");



	f.Close();



	if (o->type() != unEnsemble) 

	{

		m_pscene = new Ensemble;

		m_pscene->ajoute(o,ATTR);

	} 

	else 

		m_pscene = (Ensemble *)o;



	m_pScene->enleve(1); // scene

	m_pScene->ajoute(m_pscene);



	return VRAI;

}



booleen SceneCamera::EcrireScene(char fichier[])

{

	//Ecriture du SCN

	Fichier	f;

	char s[256];

	sprintf(s, "%s.scn", fichier);

	if (!f.Open(s, "w")) 

		return FAUX;

	m_pscene->Ecrire(&f);

	f.Close();



	//Ecriture du VSN

	if ( !EcrireVSN(fichier) ) 

		return FAUX;



	return VRAI;

}



booleen SceneCamera::EcrireVSN(char fichier[])

{

	Fichier f;

	char s[256];

	sprintf(s, "%s.scn", fichier);

	if (!f.Open(s, "w")) return FAUX;



	Ecrire_point(&f, m_Camera.PR());

	Ecrire_vecteur(&f, m_Camera.VN());

	Ecrire_vecteur(&f, m_Camera.VH());



	f.Wreel(m_Camera.DO()); f.Wchaine(" ");

	f.Wreel(m_Camera.DV()); f.Wchaine("\n");



	f.Wreel(m_Camera.Pmin()); f.Wchaine(" ");

	f.Wreel(m_Camera.Pmax()); f.Wchaine(" ");

	f.Wreel(m_Camera.Hmin()); f.Wchaine(" ");

	f.Wreel(m_Camera.Hmax()); f.Wchaine("\n");

	f.Wreel(m_Camera.Xmin()); f.Wchaine(" ");

	f.Wreel(m_Camera.Xmax()); f.Wchaine(" ");

	f.Wreel(m_Camera.Ymin()); f.Wchaine(" ");

	f.Wreel(m_Camera.Ymax()); f.Wchaine("\n");



	f.Wreel(m_Camera.DH()); f.Wchaine(" ");

	f.Wreel(m_Camera.DY()); f.Wchaine("\n");



	f.Close();



	return VRAI;

}

