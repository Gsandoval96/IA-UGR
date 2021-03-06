#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <vector>

using namespace std;

void ComportamientoJugador::gira(int i){
	orientacion+=i;
	orientacion=orientacion%4;
}

void ComportamientoJugador::autofill(){
	int cont[4];
	for(int c=0;c<4;c++) cont[c]=0; 

	for(int i=0;i<100;i++)
		for(int j=0;j<100;j++){
			if(mapaResultado[i][j]=='S') cont[0]++;
			if(mapaResultado[i][j]=='T') cont[1]++;
			if(mapaResultado[i][j]=='B') cont[2]++;
			if(mapaResultado[i][j]=='A') cont[3]++;	
		}
	

	int max=cont[0];
	for(int i=1;i<4;i++)
		if(cont[i]>max) max=cont[i];
	

	for(int i=0;i<100;i++)
		for(int j=0;j<100;j++){
			if(mapaResultado[i][j]=='?'){
				if(max==cont[0]) mapaResultado[i][j]='S';
				else if(max==cont[1]) mapaResultado[i][j]='T';
				else if(max==cont[2]) mapaResultado[i][j]='B';
				else mapaResultado[i][j]='A';
			}
		}
	
}

int ComportamientoJugador::minimo(int x,int y,int z){
	int min=x;
	if(y<min) min = y;
	if(z<min) min = z;
	return min;
}

void ComportamientoJugador::resetear(){
	orientacion = 0;
	perdido=true;
	lastAction=actIDLE;
	objetosMochila=0;

	for(int i=0;i<200;i++)
		for(int j=0;j<200;j++){
			pulgarcitoAux[i][j]=0;
			mapaResultadoAux[i][j]='?';
			}
	posX=99;
	posY=99;
	pulgarcitoAux[posX][posY]=1;
}

void ComportamientoJugador::decidirDestino(Action &accion){
	int min;
	if(perdido){
		if(orientacion==0){
			min=minimo(pulgarcitoAux[posY-1][posX],pulgarcitoAux[posY][posX-1],pulgarcitoAux[posY][posX+1]);

			if(min==pulgarcitoAux[posY-1][posX]) accion=actFORWARD;
			else if(min==pulgarcitoAux[posY][posX-1]) accion=actTURN_L;
			else accion=actTURN_R; 
		}
		else if(orientacion==1){
			min=minimo(pulgarcitoAux[posY][posX+1],pulgarcitoAux[posY-1][posX],pulgarcitoAux[posY+1][posX]);

			if(min==pulgarcitoAux[posY][posX+1]) accion=actFORWARD;
			else if(min==pulgarcitoAux[posY-1][posX]) accion=actTURN_L;
			else accion=actTURN_R; 

		}
		else if(orientacion==2){
			min=minimo(pulgarcitoAux[posY+1][posX],pulgarcitoAux[posY][posX+1],pulgarcitoAux[posY][posX-1]);

			if(min==pulgarcitoAux[posY+1][posX]) accion=actFORWARD;
			else if(min==pulgarcitoAux[posY][posX+1]) accion=actTURN_L;
			else accion=actTURN_R; 
		}
		else {
			min=minimo(pulgarcitoAux[posY][posX-1],pulgarcitoAux[posY+1][posX],pulgarcitoAux[posY-1][posX]);

			if(min==pulgarcitoAux[posY][posX-1]) accion=actFORWARD;
			else if(min==pulgarcitoAux[posY+1][posX]) accion=actTURN_L;
			else accion=actTURN_R; 
		}
	}
	else{
		if(orientacion==0){
			min=minimo(pulgarcito[posRY-1][posRX],pulgarcito[posRY][posRX-1],pulgarcito[posRY][posRX+1]);

			if(min==pulgarcito[posRY-1][posRX]) accion=actFORWARD;
			else if(min==pulgarcito[posRY][posRX-1]) accion=actTURN_L;
			else accion=actTURN_R; 
		}
		else if(orientacion==1){
			min=minimo(pulgarcito[posRY][posRX+1],pulgarcito[posRY-1][posRX],pulgarcito[posRY+1][posRX]);

			if(min==pulgarcito[posRY][posRX+1]) accion=actFORWARD;
			else if(min==pulgarcito[posRY-1][posRX]) accion=actTURN_L;
			else accion=actTURN_R; 

		}
		else if(orientacion==2){
			min=minimo(pulgarcito[posRY+1][posRX],pulgarcito[posRY][posRX+1],pulgarcito[posRY][posRX-1]);

			if(min==pulgarcito[posRY+1][posRX]) accion=actFORWARD;
			else if(min==pulgarcito[posRY][posRX+1]) accion=actTURN_L;
			else accion=actTURN_R; 
		}
		else {
			min=minimo(pulgarcito[posRY][posRX-1],pulgarcito[posRY+1][posRX],pulgarcito[posRY-1][posRX]);

			if(min==pulgarcito[posRY][posRX-1]) accion=actFORWARD;
			else if(min==pulgarcito[posRY+1][posRX]) accion=actTURN_L;
			else accion=actTURN_R; 
		}
	}
}

bool ComportamientoJugador::hayObstaculo(Sensores sensores,int i){
	bool obstaculo=false;
	if(sensores.terreno[i]=='P'|| sensores.terreno[i]=='M') obstaculo=true;
	
	else if(sensores.terreno[i]=='B' && sensores.objetoActivo!='2') obstaculo=true; 
	else if(sensores.terreno[i]=='A' && sensores.objetoActivo!='1') obstaculo=true;
	else if(sensores.superficie[i]=='d' && sensores.objetoActivo!='3') obstaculo=true;

	return obstaculo;
}

void ComportamientoJugador::actualizarPulgarcito(Sensores sensores){
	if(perdido){
		for(int i=0;i<4;i++){
			int c=i*(i+1);
			for(int j=i*i;j<(i*i+2*i)+1;j++){
				if(hayObstaculo(sensores,j)){
					if(orientacion==0) pulgarcitoAux[posY-i][posX+j-c]+=1000;
					else if(orientacion==1) pulgarcitoAux[posY+j-c][posX+i]+=1000;
					else if(orientacion==2) pulgarcitoAux[posY+i][posX-(j-c)]+=1000;
					else pulgarcitoAux[posY-(j-c)][posX-i]+=1000;
				}
				else{
					if(orientacion==0) pulgarcitoAux[posY-i][posX+j-c]%=1000;
					else if(orientacion==1) pulgarcitoAux[posY+j-c][posX+i]%=1000;
					else if(orientacion==2) pulgarcitoAux[posY+i][posX-(j-c)]%=1000;
					else pulgarcitoAux[posY-(j-c)][posX-i]%=1000;
				}

				if(sensores.terreno[j]=='K'){
					if(orientacion==0) pulgarcitoAux[posY-i][posX+j-c]=-1;
					else if(orientacion==1) pulgarcitoAux[posY+j-c][posX+i]=-1;
					else if(orientacion==2) pulgarcitoAux[posY+i][posX-(j-c)]=-1;
					else pulgarcitoAux[posY-(j-c)][posX-i]=-1;
				}
			}
		}
	}
	else{
		for(int i=0;i<4;i++){
			int c=i*(i+1);
			for(int j=i*i;j<(i*i+2*i)+1;j++){
				if(hayObstaculo(sensores,j)){
					if(orientacion==0) pulgarcito[posRY-i][posRX+j-c]+=1000;
					else if(orientacion==1) pulgarcito[posRY+j-c][posRX+i]+=1000;
					else if(orientacion==2) pulgarcito[posRY+i][posRX-(j-c)]+=1000;
					else pulgarcito[posRY-(j-c)][posRX-i]+=1000;
				}
				else{
					if(orientacion==0) pulgarcito[posRY-i][posRX+j-c]%=1000;
					else if(orientacion==1) pulgarcito[posRY+j-c][posRX+i]%=1000;
					else if(orientacion==2) pulgarcito[posRY+i][posRX-(j-c)]%=1000;
					else pulgarcito[posRY-(j-c)][posRX-i]%=1000;
				}
			}
		}
	}
}

void ComportamientoJugador::actualizarMapaResultado(Sensores sensores){
	if(perdido){
		for(int i=0;i<4;i++){
			int c=i*(i+1);
			for(int j=i*i;j<(i*i+2*i)+1;j++){
				if(orientacion==0) mapaResultadoAux[posY-i][posX+j-c]=sensores.terreno[j];
				else if(orientacion==1) mapaResultadoAux[posY+j-c][posX+i]=sensores.terreno[j];
				else if(orientacion==2) mapaResultadoAux[posY+i][posX-(j-c)]=sensores.terreno[j];
				else mapaResultadoAux[posY-(j-c)][posX-i]=sensores.terreno[j];
			}
				
		}
	}
	else{
		for(int i=0;i<4;i++){
			int c=i*(i+1);
			for(int j=i*i;j<(i*i+2*i)+1;j++){
				if(orientacion==0) mapaResultado[posRY-i][posRX+j-c]=sensores.terreno[j];
				else if(orientacion==1) mapaResultado[posRY+j-c][posRX+i]=sensores.terreno[j];
				else if(orientacion==2) mapaResultado[posRY+i][posRX-(j-c)]=sensores.terreno[j];
				else mapaResultado[posRY-(j-c)][posRX-i]=sensores.terreno[j];
			}
				
		}
	}
}

void ComportamientoJugador::ajustar(Sensores sensores){
	
	for(int i=0;i<100;i++)
		for(int j=0;j<100;j++){
			pulgarcito[i][j]+=pulgarcitoAux[posY-sensores.mensajeF+i][posX-sensores.mensajeC+j];
			if(mapaResultadoAux[posY-sensores.mensajeF+i][posX-sensores.mensajeC+j]!='?' && mapaResultado[i][j]=='?') mapaResultado[i][j]=mapaResultadoAux[posY-sensores.mensajeF+i][posX-sensores.mensajeC+j];
			
		}
}

void ComportamientoJugador::orientar(Action accion,bool colision){

	if(lastAction==actFORWARD){
		if(!colision){
			if(orientacion==0){
				posY--;
				posRY--;
			}
			else if(orientacion==1){
				posX++;
				posRX++;
			}
			else if(orientacion==2){
				posY++;
				posRY++;
			}
			else {
				posX--;
				posRX--;
			}
		}
	}

	else if(lastAction==actTURN_L) gira(3);
	else if(lastAction==actTURN_R) gira(1);
}



void ComportamientoJugador::interacciones(Sensores sensores, Action &accion){
	if(sensores.superficie[2]=='0' || sensores.superficie[2]=='1'|| sensores.superficie[2]=='2'|| sensores.superficie[2]=='3'){
		if(objetosMochila==4 && sensores.objetoActivo!='_') accion=actTHROW;

		else if(sensores.objetoActivo=='_') accion=actPICKUP;
		else {
			accion=actPUSH; 
			objetosMochila++;
		}

	}
	else if(sensores.superficie[2]=='a') accion=actIDLE;

	else if(sensores.superficie[2]=='l'){ 
		if(sensores.objetoActivo=='0') accion=actGIVE;
		else accion=actIDLE;
	}

	else if(sensores.superficie[2]=='d'){ 
		if(sensores.objetoActivo=='3') accion=actGIVE;
		else accion=actTURN_L;
	}
}




Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

	// En esta matriz de tamano 100x100 hay que escribir el mapa solucion
	// mapaResultado[fila][columna] = lo que hay en fila columna


	orientar(lastAction,sensores.colision);
	
	if(sensores.reset){ 
		resetear();
	}

	if(perdido && sensores.mensajeC!=-1){
		perdido=false; 
		posRX=sensores.mensajeC; 
		posRY=sensores.mensajeF;
		ajustar(sensores);
	}


	/*cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Mochila: " << sensores.mochila << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << "objetoActivo: " << sensores.objetoActivo<<endl;
	cout << "ORIENTACION: " << orientacion<<endl;
	cout << endl;*/

	actualizarMapaResultado(sensores);
	actualizarPulgarcito(sensores);
	decidirDestino(accion);
	interacciones(sensores,accion);

	if(hayObstaculo(sensores,2) && accion==actFORWARD) accion=actTURN_R;
	
	if(perdido) pulgarcitoAux[posY][posX]++;
	else pulgarcito[posRY][posRX]++;
	
	lastAction=accion;
	ejecuciones++;

	if(ejecuciones==19999) autofill();	

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}





