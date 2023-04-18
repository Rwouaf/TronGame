 
#pragma warning( disable : 4996 ) 

 
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <time.h>     
#include "G2D.h"

using namespace std;


struct GameData
{
	GameData() {}

};



void Render(const GameData& G)  // const ref => garantit que l'on ne modifie pas les donnes de G
{
}


void Logic(GameData& G)
{
	
}
 

void AssetsInit(GameData & G)
{
   // Size pass� en ref et texture en param
//    G.Heros.IdTex = G2D::initTextureFromString(G.Heros.Size, G.Heros.texture);  
//    G.Heros.Size  = G.Heros.Size  * 2; // on peut zoomer la taille du sprite

//    G.Key.IdTex   = G2D::initTextureFromString(G.Key.Size, G.Key.texture);
//    G.Key.Size    = G.Key.Size * 1.5; // on peut zoomer la taille du sprite

//    G.Coffre.IdTex = G2D::initTextureFromString(G.Coffre.Size, G.Coffre.texture);
//    G.Coffre.Size  = G.Coffre.Size * 1.5; // on peut zoomer la taille du sprite

// 	for (int i = 0; i < 3; i++){
// 		G.Momies[i].IdTex = G2D::initTextureFromString(G.Momies[i].Size, G.Momies[i].texture);
// 		G.Momies[i].Size  = G.Momies[i].Size * 2; // on peut zoomer la taille du sprite
//	}

}

int main(int argc, char* argv[])
{ 
	GameData G;

	//G2D::initWindow(V2(G.Lpix * 15, G.Lpix * 15), V2(200,200), string("Labyrinthe"));
	  
	AssetsInit(G);

	int nbCalltoLogicPerSec = 50;

	G2D::Run(Logic,Render,G, nbCalltoLogicPerSec,true);
 
}
 
  
 