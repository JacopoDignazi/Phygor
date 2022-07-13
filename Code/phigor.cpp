#include "base.h"
#include "law.h"
#include "exp.h"
#include "lem.h"
#include "theory.h"
#include "law_s.h"
#include "exp_s.h"
#include "lem_s.h"
#include "theory_s.h"
#include "axiom.h"
#include "relation_state.h"
#include "chrono.h"
#include "check_state.h"

#include "overall.h"
#include "loading.h"



 	
int main(){
	
	loading();
	
	cout<<endl<<"\n\t\t\t ---=={ PHIGOR 3.3 }==--- ";
	cout<<endl<<"\n\t\t\tJacopo D'Ignazi 04/04/2017";
	cout<<endl<<endl<<endl;
	
	
	while(sel != 'z')	{
		cout<<endl<<"   a) Crea nuovo";
		cout<<endl<<"   b) Modifica esistente";
		cout<<endl<<"   c) Lavora a teoria";
		cout<<endl<<"   d) Esplora";
		cout<<endl<<"   ?) Controlla stato";
		cout<<endl<<"\n   scegli modalita' (z per uscire)    ";
		cin>>sel;
		context= GLOBAL;
		switch(sel){
			case 'a': {
				cout<<"\n\n ***********************************************************************\n";
				s_sel= '1';	
				while(s_sel != 'z'){
					if(context !=GLOBAL)	cout<<endl<<"     a.0) Mostra tutto ( globale )";
					cout<<endl<<"     a.1) Mostra tutto ( contesto )";
					cout<<"\t\t\t\t\t\t contesto attuale: "<<context->name;
					cout<<endl<<"     a.2) Cambia contesto";
					cout<<endl<<"     a.3) Nuova legge";
					cout<<endl<<"     a.4) Nuovo esperimento";
					cout<<endl<<"     a.5) Nuova corrispondenza (LEM)";	
					cout<<endl<<"          scegli modalita' (z per menu' principale)    ";	
					cin>>s_sel;
					switch(s_sel){
						case '0' :{
							display_theory();
							GLOBAL->display_all();
							break;
						}
						
						case '1' :{
							if(context==GLOBAL) display_theory();
							context->display_all();
							break;
						}

						case '2' :{
							cout<<endl<<"      inserire teoria di riferimento (z per annullare):  ";
							cin>>text;
							if(text == "z" ) break;
							context=search_theory(text);
							if(!*context){
								cout<<endl<<"      teoria "<<text<<" non trovata, reset contesto a global\n";
								context=GLOBAL; 
							}
							break;
						}

						case '3' :{
							while(1){
								cout<<endl<<"      inserire nome (z per annullare):   ";
								cin>>text_l; if(text_l == "z") break;
								cout<<"      inserire anno (0 per annullare):   ";
								cin>>year; if(year == 0) break;
								if(!!* (law= GLOBAL->search_law(text_l)) ){
								    cout<<endl<<"        legge già esistente in anno "<<law->Get_year();
									break;
								}
								law= new Law(text_l,year,context);
							}
							break;
						}

						case '4' :{
							while(1){
								cout<<endl<<"      inserire nome (z per annullare):   ";
								cin>>text_e; if(text_e == "z") break;
								cout<<"      inserire anno (0 per annullare):   ";
								cin>>year; if(year == 0 ) break;
								if(!!* (exp= GLOBAL->search_exp(text_e)) ){    
									cout<<endl<<"        esper già esistente in anno "<<exp->Get_year();
									break;
								}
								exp= new Exp(text_e,year,context);
							}
							break;
						}

						case '5' :{
							while(1){
								cout<<endl<<"      inserire nome legge (tesi):   ";
								cin>>text_l; if(text_l == "z") break;
								law= GLOBAL->search_law(text_l);
								if(!*law){cout<<endl<<"      legge non trovata"; break;}
								
								cout<<endl<<"      inserire nome esperimento :   ";
								cin>>text_e; if(text_e == "z") break;
								exp= GLOBAL->search_exp(text_e);
								if(!*exp){cout<<endl<<"      esperimento non trovato"; break;}
								
								cout<<endl<<"      inserire valore di confronto (v/f) ";
								cin>>text_v;
								if(text_v == "v") { value=true; }
								else if(text_v == "f") {value= false;}								
								else break;
								lem=GLOBAL->search_lem(law,exp);
								if(!*lem){								
									lem= new LEM(law,exp,value);
									cout<<"\t\t\t\t\t     "<<law->Get_name()<<lem->value_string()<<exp->Get_name();
								}
								else{ 
									cout<<endl<<"     trovata relazione esistente:"
									<<law->Get_name()<<lem->value_string()<<exp->Get_name();
								}
							}
							break;
						}
						
						case 'z':{
							cout<<"\n\n ***********************************************************************\n";							
							break;
						}
						
						default :{
							cout<<endl<<"     modalita' non esistente, inserire numero tra 1,5 (z per menu' principale) ";
						}
					}
				}
				break;
			}		
			
			case 'b' :{
				s_sel= 'a';
				cout<<"\n\n ***********************************************************************\n";
				while(s_sel != 'z'){
					cout<<endl<<"     b.0) Mostra tutto ( globale )";
					cout<<endl<<"     b.1) Set up leggi";
					cout<<endl<<"     b.2) Set up esperimento";	
					cout<<endl<<"     b.3) NO   Modifica legge (nome, anno)";
					cout<<endl<<"     b.4) NO   Modifica esperimento (nome, anno)";
					cout<<endl<<"     b.5) NO   Modifica relazione lem (valore)";					
					cout<<endl<<"          scegli modalita' (z per menu' principale)    ";	
					cin>>s_sel;
					switch(s_sel){						
						case '0' :{
							display_theory();
							GLOBAL->display_all();
							break;
						}
						
						case '1':{
							while(1){
								cout<<endl<<"      inserire nome legge:    ";
								cin>>text_l; 
								law=GLOBAL->search_law(text_l);
								if(text_l == "z") break;
								if(!*law) {cout<<"    legge non trovata"; break;}
								cout<<endl<<"      inserire nome legge dedotta da "<<text_l<<" :     ";
								cin>>text_s_l;
								s_law=GLOBAL->search_law(text_s_l);
								if(text_s_l == "z") break;
								if(!*s_law) {cout<<"    legge non trovata"; break;}	
								
								cout<<"      inferenza creata:  "
								<<law->Get_name()<<" ==> "<<s_law->Get_name()<<" ("<<s_law->Set_up(law)<<")";							
								
							}
							break;
						}

						case '2':{
							cout<<endl<<"      inserire nome esperimento:    ";
							cin>>text_e; 
							exp=GLOBAL->search_exp(text_e);
							if(text_e == "z") break;
							if(!*exp) {cout<<"    esperimento non trovato"; break;}
							while(1){
								cout<<endl<<"      inserire nome legge / nome strumento:   ";
								cin>>text_l;
								law=GLOBAL->search_law(text_l);
								if(text_l == "z") break;
								if(!*law) {cout<<"    legge non trovata"; break;}	
								if( (check= exp->Set_up(law)) > 0) {
									cout<<"      aggiunta assunzione "<<check<<" a "<<text_e;
								}
								if( check==-2) cout<<"      "<<text_l<<" gia' presente in "<<text_e;
								if( check==-1) cout<<" raggiunto limite massimo built_from [N_str = "<<N_str<<"]";	
							}
							break;
						}
					}
				}						
				break;	
			}
								
			case 'c' :{
				cout<<"\n\n ***********************************************************************\n";
				cout<<endl<<"     inserire nome teoria:    ";
				cin>>text;     
				context=search_theory(text);
				if(!*context){
					cout<<endl<<"      teoria "<<'"'<<text<<'"'<<" inesistente, creare teoria? (s/n)     ";
					cin>>s_text;
					if(s_text == "n") {context= GLOBAL; break;}
					if(s_text == "s") {
						context=make_theory(text);
					cout<<"\t\t\t\t\t      creata teoria "<<theory[T_count]->name;
					}
				}
				if( context == GLOBAL ){
					cout<<endl<<"     impossibile selezionare "<<GLOBAL->name<<" : usata come globale";
					cout<<endl<<"     NB: la teoria globale contiene gia' tutte le informazioni inserite nel programma"<<endl;
					break;
				}
				
				s_sel= '1';	
				while(s_sel != 'z'){
					cout<<endl<<"     c.0) Mostra tutto ( globale )";
					cout<<endl<<"     c.1) Mostra tutto ( teoria )";
					cout<<"\t\t\t\t\t\t teoria : "<<context->name;
					cout<<endl<<"     c.2) Modifica nome";
					cout<<endl<<"     c.3) Aggiungi legge";// esistente a "<<context->name;
					cout<<endl<<"     c.4) Aggiungi esperimento";// esistente a "<<context->name;
					cout<<endl<<"     c.5) Aggiungi relazione (LEM)";// esistente a "<<context->name;
					cout<<endl<<"          scegli modalita' (z per menu' principale)    ";	

					cin>>s_sel;
					switch(s_sel){
						case '0' :{
							display_theory();
							GLOBAL->display_all();
							break;
						}
						
						case '1' :{
							context->display_all();
							break;
						}
						
						case '2' :{
							cout<<endl<<"     nome teoria: "<<context->name<<"; inserire nuovo nome teoria:    ";
							cin>>text; 
							if(text== "z") break;    
							context=search_theory(text);
							if(!!*context){
								cout<<endl<<"      teoria "<<'"'<<text<<'"'<<" gia' esistente";
								break;
							}
							cout<<"\t\t\t\t\t      nuovo nome teoria -->"<<context->name;
							break;
						}
						
						case '3' :{
							while(1){
								cout<<endl<<"     inserire nome legge:     ";
								cin>>text_l;
								law  =GLOBAL->search_law(text_l);
								s_law=context->search_law(text_l);
								if(text_l=="z") break;
								if(!*law) {
									cout<<"     legge inesistente"<<endl;
									continue; //(o break?) continue salta la parte sotto?
								}
								if(!!*s_law){
									cout<<"     legge "<<s_law->Get_name()<<" gia' in "<<context->name<<endl;
									continue;
								}
								context->add(law);
								cout<<"\t\t\t\t\t      "<<context->name<<" include "<<law->Get_name();
							}			
						break;
						}

						case '4' :{
							while(1){
								cout<<endl<<"     inserire nome esperimento:     ";
								cin>>text_e;
								exp  =GLOBAL->search_exp(text_e);
								s_exp=context->search_exp(text_e);
								if(text_e=="z") break;
								if(!*exp) {
									cout<<"     esperimento inesistente"<<endl;
									continue; //(o break?) continue salta la parte sotto?
								}
								if(!!*s_exp){
									cout<<"     esperimento "<<s_exp->Get_name()<<" gia' in "<<context->name<<endl;
									continue;
								}
								context->add(exp);
								cout<<"\t\t\t\t\t      "<<context->name<<" include "<<exp->Get_name();
							}			
							break;
						}

						case '5' :{
							while(1){
								cout<<endl<<"     inserire nome tesi:     ";
								cin>>text_l;
								if(text_l == "z") break;
								law=GLOBAL->search_law(text_l);
								if(!*law) {cout<<"     legge inesistente"; break;}
								
								cout<<"     inserire nome osservazione:     ";
								cin>>text_e;
								if(text_e == "z") break;
								exp=GLOBAL->search_exp(text_e);
								if(!*exp) {cout<<"     esperimento inesistente"; break;}
								
								lem=GLOBAL->search_lem(law,exp);
								s_lem=context->search_lem(law,exp);
								if(!*lem) {cout<<"     relazione inesistente"; break;}
								if(!!*s_lem){
									cout<<"     "<<context->name<<" contiene gia"
									<<law->Get_name()<<s_lem->value_string()<<exp->Get_name();
									continue;
								}
								context->add(lem);
								cout<<"\t\t\t\t\t     "<<context->name<<" include "
								<<law->Get_name()<<lem->value_string()<<exp->Get_name();
							}	
							break;
						}
					}
				}
				break;
			}
										
			case 'd' :{
				s_sel='1';
				year=TODAY;
				context=GLOBAL;
				cout<<"\n\n ***********************************************************************\n";				
				while(s_sel!= 'z'){
					if(context != GLOBAL || year != TODAY) cout<<endl<<"     d.0) display_all globale";
					cout<<endl<<"     d.1) display_all di contesto";
					cout<<"\t\t\t\t\t\t contesto attuale: "<<context->name;
					cout<<endl<<"     a.2) Cambia contesto";
					cout<<"\t\t\t\t\t\t\t        in anno: "<<year;
					cout<<endl<<"     d.3) Informazioni legge";
					cout<<endl<<"     d.4) Informazioni esperimento";
					cout<<endl<<"     d.5) NO Informazioni relazioni tra oggetti";
					cout<<endl<<"     d.6) cronologia di teoria";
					cout<<endl<<"          scegli modalita' (z per menu' principale)    ";	
					cin>>s_sel;
					switch(s_sel){
						case '0' :{
							display_theory();
							GLOBAL->display_all();
							break;
						}

						case '1' :{
							context->display_all(year);
							break;
						}
						
						case '2' :{
							cout<<endl<<"      inserire teoria di contesto (z per annullare):  ";
							cin>>text;
							if(text == "z" ) break;
							context=search_theory(text);
							if(!*context){
								cout<<endl<<"      teoria "<<text<<" non trovata, reset contesto a global\n";
								context=GLOBAL; 
							}
							cout<<"     inserire anno di contesto:                     ";
							cin>>year;
							break;
						}
												
						case '3':{
							int n=1;
							while(1){
								cout<<endl<<"      inserire nome legge:    ";
								cin>>text_l; 
								law=GLOBAL->search_law(text_l);
								if(text_l == "z") break;
								if(!*law) {cout<<"    legge non trovata"; break;}
								cout<<endl;

								law->display_axioms(context,year);
								cout<<endl<<endl<<endl;
								cout<<"      "<<law->Get_name()<<" "<<law->Get_year()<<" : ";
								check=law->Put_axioms(axL,context,year);
								if(check == 0) cout<<" assioma ";
								else{
									int i=0;
									cout<<" { ";
									while(!!*axL[i]){ 
										if(i != 0) cout<<", ";
										cout<<axL[i]->Get_name();
										++i;
										if(!*axL[i]) cout<<" }";
									}
								}
								if(context != GLOBAL) cout<<"  nella teoria "<<context->name;
								if(year != TODAY ) cout<<" in anno "<<year;
								int check_e=context->map_state(law,'e', year); cout<<endl<<check_e;
								int check_i=context->map_state(law,'i', year); cout<<endl<<check_i;
								n=1; 
								Law* s_law=no_law; cout<<endl<<endl;
								if(!(check_e%17)) 	cout<<"\n      -risulta ciclica";
								if(!(check_e%7)) cout<<"\n      -non ha inferenze esplicite";
								else {
									cout<<"\n      - inferenze esplicite:";
									cout<<"\n           equivalenza:"; n=1;							
									while( !!*(s_law=context->search_law_ax(law,'s', 'e', year, n)) ){
										cout.width(10); cout<<" "; cout<<s_law->Get_name()<<" ";//<<lem->
										n++;
									}									
									cout<<"\n           deduttive in:"; n=1;
									while( !!*(s_law=context->search_law_ax(law,'d', 'e', year, n)) ){
										cout.width(10); cout<<" "; cout<<s_law->Get_name()<<" ";//<<lem->
										n++;
									}
									cout<<"\n           induttive da:"; n=1;									
									while( !!*(s_law=context->search_law_ax(law,'i', 'e', year, n)) ){
										cout.width(10); cout<<" "; cout<<s_law->Get_name()<<" ";//<<lem->
										n++;
									} 
								}
								if(!(check_i%7))  cout<<"\n      - non ha inferenze implicite:";
								else{
									cout<<"\n      - inferenze implicite:";
									cout<<"\n           equivalenza:"; n=1;							
									while( !!*(s_law=context->search_law_ax(law,'s', 'i', year, n)) ){
										cout.width(10); cout<<" "; cout<<s_law->Get_name()<<" ";//<<lem->
										n++;
									}
									cout<<"\n           deduttive in:"; n=1;
									while( !!*(s_law=context->search_law_ax(law,'d', 'i', year, n)) ){
										cout.width(10); cout<<" "; cout<<s_law->Get_name()<<" ";//<<lem->
										n++;
									}
									cout<<"\n           induttive da:"; n=1;
									while( !!*(s_law=context->search_law_ax(law,'i', 'i', year, n)) ){
										cout.width(10); cout<<" "; cout<<s_law->Get_name()<<" ";//<<lem->
										n++;
									}
								}
								if(!(check_e%6)) cout<<"\n      -non ha relazioni lem esplicite";	
								else{
									int n=1;
									cout<<"\n      - relazioni lem esplicite"; n=1;
									cout<<"\n          come tesi:"; n=1;									
									while( !!*(lem=context->search_lem_ax(law,'d', 't','e', year, n)) ){
										cout.width(10); cout<<" "; cout<<lem->value_string(context,year)<<" ";//<<lem->
										n++;
									}
									while( !!*(lem=context->search_lem_ax(law,'s', 't','e', year, n)) ){
										cout.width(10); cout<<" "; cout<<lem->value_string(context,year)<<" ";//<<lem->
										n++;
									}									
									cout<<"\n          come assunzione:"; n=1;									
									while( !!*(lem=context->search_lem_ax(law,'d', 's','e', year, n)) ){
										cout.width(10); cout<<" "; cout<<lem->value_string(context,year)<<" ";//<<lem->
										n++;
									}
									while( !!*(lem=context->search_lem_ax(law,'s', 's','e', year, n)) ){
										cout.width(10); cout<<" "; cout<<lem->value_string(context,year)<<" ";//<<lem->
										n++;
									}																		
								}
								if(!(check_i%6)) cout<<"\n      -non ha relazioni lem implicite";
								else{
									cout<<"\n      - relazioni lem implicite:"; n=1;
									cout<<"\n          come tesi:"; n=1;									
									while( !!*(lem=context->search_lem_ax(law,'d', 't','i', year, n)) ){
										cout.width(10); cout<<" "; cout<<lem->value_string(context,year)<<" ";//<<lem->
										n++;
									}
									while( !!*(lem=context->search_lem_ax(law,'s', 't','i', year, n)) ){
										cout.width(10); cout<<" "; cout<<lem->value_string(context,year)<<" ";//<<lem->
										n++;
									}																		
									cout<<"\n          come assunzione:"; n=1;									
									while( !!*(lem=context->search_lem_ax(law,'d', 's','i', year, n)) ){
										cout.width(10); cout<<" "; cout<<lem->value_string(context,year)<<" ";//<<lem->
										n++;
									}
									while( !!*(lem=context->search_lem_ax(law,'s', 's','i', year, n)) ){
										cout.width(10); cout<<" "; cout<<lem->value_string(context,year)<<" ";//<<lem->
										n++;
									}										
								}																											
								
								cout<<"\n\n -------------------------------------------------------------------------- \n";
							}
							break;
						}
							
						case '4':{
							int n=1;
							while(1){
								cout<<endl<<"      inserire nome esperimento:    ";
								cin>>text_e; 
								exp=GLOBAL->search_exp(text_e);
								if(text_e == "z") break;
								if(!*exp) {cout<<"    esperimento non trovato"; break;}
								cout<<endl<<endl<<endl;
								cout<<"      "<<exp->Get_name()<<" "<<exp->Get_year()<<", costruito su assiomi: ";
								check=exp->Put_axioms(axE,context,year);
								if(check == 0) cout<<" NON COSTRUITO ";
								else{
									int i=0;
									cout<<" { ";
									while(!!*axL[i]){ 
										if(i != 0) cout<<", ";
										cout<<axL[i]->Get_name();
										++i;
										if(!*axL[i]) cout<<" }";
									}
								}
								if(context != GLOBAL) cout<<"  nella teoria "<<context->name;
								if(year != TODAY ) cout<<" in anno "<<year;
								int check_e=context->map_state(exp,'e', year); cout<<endl<<check_e;
								//int check_i=context->map_state(law,'i', year); cout<<endl<<check_i;

								cout<<"\n\n -------------------------------------------------------------------------- \n";
								
							}
							
							break;
						}
						
						case '6':{
							GLOBAL->print_chrono();
							break;	
							
						}
					}
		
				}					
				break;	
			}
			
			case '?':{
				
				
				cout<<"\n\n ***********************************************************************\n";
				
				cout<<endl<<"       "<<GLOBAL->print_uninferencing_laws()<<" leggi ininferenti"<<endl;	
				cout<<endl<<"       "<<GLOBAL->print_unrelated_laws()<<" leggi senza relazioni lem"<<endl;	
				cout<<endl<<"       "<<GLOBAL->print_eq_ax()<<" leggi equivalenti"<<endl;
				cout<<endl<<"       "<<GLOBAL->print_ciclic_inference()<<" inferenze cicliche"<<endl;
								
				cout<<endl<<"       "<<GLOBAL->print_undefined_exps()<<" esperimenti indefiniti"<<endl;					 					
				cout<<endl<<"       "<<GLOBAL->print_unrelated_exps()<<" esperimenti senza relazioni lem"<<endl;
				cout<<endl<<"       "<<GLOBAL->print_ciclic_set_up()<<" esperimenti ciclici"<<endl;

				cout<<endl<<"       "<<GLOBAL->print_conflicting_lems()<<" lem discordi"<<endl;
				cout<<endl<<"       "<<GLOBAL->print_tautologic_lems()<<" lem tautologici"<<endl;
				cout<<endl<<"       "<<GLOBAL->print_ciclic_lems()<<" lem ciclici "<<endl;
				
				cout<<"\n\n ***********************************************************************\n";
			
			/*	for(int i=0; i<=T_count; i++){	//in contesto di teoria
					theory[i]->print_unrelated_laws();	//leggi che non compaiono in lem
					theory[i]->print_unrelated_exps();	//exp che non compaiono in lem
					theory[i]->print_conflicting_lems();//lem in conflitto tra loro
											    //stessi L,E e diverso val
				}*/								//L,noe vs L,E; nol,E vs L,E
													//forse dovrei fare a meno di M(?,E) e M(E,?)
				
				break;
			}
				
			default:{
				cout<<endl<<"     modalità inesistente, inserire a,b,c,d; z per uscire dal programma";
//				cout<<endl<<"     i per informazioni, o per opzioni, ? per correggere gli errori";
					
				break;
			}
		
		
		}///questa chiude lo switch


	}	
}
	
	
