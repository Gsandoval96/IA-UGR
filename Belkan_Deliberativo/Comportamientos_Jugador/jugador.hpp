#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <iostream>
using namespace std;


class ComportamientoJugador : public Comportamiento{

  private:
    int orientacion; //N=0,E=1,S=2,O=3
    bool perdido; //es true si no ha pisado un pk
    int pulgarcito[100][100]; //mapa con las casillas pisadas

    int pulgarcitoAux[200][200];
    char mapaResultadoAux[200][200];

    int ejecuciones;

    int objetos[5];

    //posiciones respecto al mapaRec
    int posX;
    int posY; 
    int posRX;
    int posRY; 
    int objetosMochila;
   
    Action lastAction; //última acción realizada

    void gira(int i);

    void autofillAux(int a, int b, int c, int d);

    void autofill();

    int minimo(int x,int y,int z);
	
    void resetear();

    void decidirDestino(Action &accion);

    bool hayObstaculo(Sensores sensores,int i);

    void actualizarPulgarcito(Sensores sensores);

    void actualizarMapaResultado(Sensores sensores);

    void orientar(Action accion,bool colision);

    void interacciones(Sensores sensores, Action &accion);

    void ajustar(Sensores sensores);

    void piensaObjeto(Sensores sensores, Action &accion);

    bool tieneObjeto(int i);

    char tipoObstaculo(Sensores sensores);

    void rotaObjetos(Sensores sensores, Action &accion);

    bool casillaSegura(Sensores sensores);
	

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
	orientacion = 0;
	ejecuciones=0;
	perdido=true;
	lastAction=actIDLE;
	objetosMochila=0;
	for(int p=0;p<100;p++){
		for(int q=0;q<100;q++){
			if((p>=0 && p<=2) ||(q>=0 && q<=2) || (p>=97 && p<=99) ||(q>=97 && q<=99)) mapaResultado[p][q]='P';
			pulgarcito[p][q]=0;
		}
	}
	for(int i=0;i<200;i++)
		for(int j=0;j<200;j++){
			pulgarcitoAux[i][j]=0;
			mapaResultadoAux[i][j]='?';
		}
	posX=99;
	posY=99;
	pulgarcito[posX][posY]=1;

	for(int i=0;i<5;i++) objetos[i]=0;
	
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);

    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}


  

};


#endif
