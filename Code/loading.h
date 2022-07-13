#include "base.h"
#include "law.h"
#include "exp.h"
#include "lem.h"
#include "theory.h"
#include "law_s.h"
#include "exp_s.h"
#include "lem_s.h"
#include "theory_s.h"
#include "chrono.h"
#include "overall.h"



void loading(){	

	
	GLOBAL=SCIENZA;
	theory[0]=GLOBAL;
	for(int i=0; i<N_law; i++){
		axL[i]=no_law;
		axE[i]=no_law;
	}
	
}
/*	Law* s[3];
	//s[0]= new Law("EM",1800);
	//s[1]= new Law("RR",1905);
	//Law* f= new Law("equianno",1905);
	//s[2]= new Law("scintillatore",1949);
	
	//for(int i=0; i<3; i++) e->Set_up(s[i]);

	LEM* res= new LEM (c);		
	LEM* hip= new LEM(s[2]);	
	LEM* prova= new LEM(a,e,true);
	LEM* ee=new LEM(e);

	LEM* prova2= new LEM(a,b,false);
	
	LEM* prova3= new LEM(s[1],c,false);
	LEM* prova4= new LEM(s[0],c,true);
	LEM* prova5= new LEM(f,e,false);
	Exp* cern= new Exp("cern",2018);*/
	
	//SCIENZA->add(a);
	//SCIENZA->add(e);
	//SCIENZA->add(prova);



