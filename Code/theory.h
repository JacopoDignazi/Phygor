#ifndef THEORY_H
#define THEORY_H

#include "base.h"
#include "law.h"
#include "exp.h"
#include "lem.h"

class Theory{

	Law* law[N_law];
	Exp* exp[N_exp];
	LEM* lem[N_lem];
	Math* ground[N_math];
	
	int sort_law();
	int sort_exp();
	int sort_lem();
//	int sort_str(Exp*);
	int sort_all();


	friend Exp::sort_built();
		
	public:
		string name;
		Theory(string);
		int add(Law*);
		int add(Exp*);
		int add(LEM*, char);

		int Put_axioms(Law**, char, char, int);
//		int compare_axioms(Law*,Law*,int);
//		int compare_axioms(Exp*,Exp*,int);
//		int compare_axioms(Law*,Exp*,int);
//		int compare_axioms(LEM*,int);
		
		Law* search_law(string);
		Exp* search_exp(string);

		LEM* search_lem(Law*,Exp*, char, int);		//int è per n-simo trovato (default n=1)		
		LEM* search_lem(Law*, char, int);	/////uso in check_err e chrono
		LEM* search_lem(Exp*, int);			/////uso in check_err e chrono
		
		Law* search_law_ax(Law**, char, int, int);
		Exp* search_exp_ax(Law**, char, int, int );
		LEM* search_lem_ax(Law**, char, char, int, int);
		Law* search_law_ax(Law*, char, char, int, int); 
		Exp* search_exp_ax(Law*, char, char, int, int );
		LEM* search_lem_ax(Law*, char, char, char, int, int);
		
		bool involve(Law*, int);	//dice se sono contenuti 
		bool involve(Exp*, int);
		bool involve(LEM*, int);				
		int inf_rel(Law*,Law*,int);
		int inf_rel(Exp*,Exp*,int);
		int inf_rel(Law*,Exp*,int);
		int map_state(Law*, char, int);
		int map_state(Exp*, char, int);
		int map_state(LEM*,int);		

		int print_ciclic_inference(int);	////////leggi con se stesse in induz (in T,y)		
		int print_uninferencing_laws(int);	////////leggi senza alcuna inferenza (in T,y)
		int print_eq_ax(int);				////////leggi con stessi assiomi 	 (in T,y)	
			
		int print_undefined_exps(int);		////////exper senza strumenti		 (in T,y)
		int print_ciclic_set_up(int);		////////exper con strum successivi   (in T,y)

		int print_unrelated_laws(char,int);	////////leggi che non compaiono come 't' o 's' o 'g' (in T,y)
		int print_unrelated_exps(int);		////////exper che non compaiono in LEM				 (in T,y)
		int print_conflicting_lems(int);	////////LEM con stesse T,O, val uguali o meno		 (in T,y)
		int print_tautologic_lems(int);		////////LEM con axTesi == axStru	 (in T,y)
		int print_ciclic_lems(int);			////////LEM i cui assiomi sono indotti dalla tesi    (in T,y)
		
		int print_chrono(int,int);
		int display_all(char);
		
		Theory(){ name="NULL_T"; };
		bool operator!();

};

Theory* no_theory= new Theory();
Theory* GLOBAL=no_theory;

#endif
