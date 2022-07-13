#ifndef SPEC_LEM_H
#define SPEC_LEM_H

#include "base.h"
#include "law.h"
#include "exp.h"
#include "lem.h"
#include "theory.h"




bool LEM::operator==(LEM* other){
	if(this->thesis=other->thesis){
		if(this->observation=other->observation){
			if(this->curr_state=other->curr_state){
				return true;
			}
		}
	}
	return 0;
}


LEM::LEM(Law* T, Exp* O, bool _value, Theory* def=GLOBAL, char add=AUTO_ADD_LEMTO){
	thesis= T;
	observation=O;
	sort_assumption();
	lem_value= val(_value);

	if( !*T ){
		lem_value=undef; 
		cout<<"\a\a";
		return;
	}
	if( !*O ){ 
		lem_value=undef;
		cout<<"\a\a";
		return;
	}
	if( T->year >= O->year) {		//legge successiva a experimento
		this->year= T->year;
		if(lem_value) 	curr_state=cons_law;
		else 		curr_state=fals_law;
	}
	else {							//
		this->year=O->year;			//experimento successivo a legge
		if(lem_value) 	curr_state=cons_exp;
		else 		curr_state=fals_exp;
	}
	safe=true;
	GLOBAL->add(this,add);
	if(def != GLOBAL) def->add(this,add);
}

int LEM::sort_assumption(){
	for(int i=0; i<N_str; i++){ assumption[i]=observation->built_from[i]; }
	return 1;
}
		
INF_S LEM::inf_state(Theory* context=GLOBAL, int y=TODAY){
	return thesis->inf_state(context,y);
};

LEM_S LEM::state(Theory* context=GLOBAL, int y=TODAY){
	Law* T= no_law;
	Exp* O= no_exp;	
//	if(! *this ) return ;
	if( context->involve(thesis,y) ) T=thesis;
	if( context->involve(observation,y) ) O=observation;

	if( !*T ){ return research;}
	if( !*O ){ return hypotesis;}
	
	if( T->year >= O->year) {		//legge successiva a experimento
		if(lem_value) 	return cons_law;
		else 		return fals_law;
	}
	else {							//experimento successivo a legge
		if(lem_value) 	return cons_exp;
		else 		return fals_exp;
	}
}

LEM_V LEM::value(Theory* t=GLOBAL, int y=TODAY){
	if( t->involve(thesis,y) && t->involve(observation,y) ) return lem_value;
	return undef;
}



string LEM::value_string(Theory* t=GLOBAL, int y=TODAY){
	LEM_V v=value(t,y);
	if(v==cons) return " consistente con ";
	if(v==fals) return " falsificata da ";
	if(v==undef) return " unclear ";
}

string LEM::state_string(Theory* t=GLOBAL,int y=TODAY){
	LEM_S s=state(t,y);
	if(s==research ) return " osservazione ";
	if(s==hypotesis) return " ipotesi ";
	if(s==cons_exp ) return " validita' ";
	if(s==fals_exp ) return " falsificazione ";
	if(s==cons_law ) return " spiegazione di";
	if(s==fals_law ) return " legge incons con ";
}

string LEM::inf_string(Theory* t=GLOBAL,int y=TODAY){
	return thesis->inf_string(t,y);
}


#endif
