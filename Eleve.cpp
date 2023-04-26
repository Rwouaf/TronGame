 
#pragma warning( disable : 4996 ) 

 
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <time.h>     
#include "G2D.h"

using namespace std;

struct moto
{
	V2 Pos;
	V2 Size;
	V2 Dir;
	int IdTex;
	string color;
	string texture;
	V2 previousPos;

	moto(V2 pos, V2 size, V2 dir, string c)
	{
		Pos = pos;
		Size = size;
		Dir = dir;
		previousPos = pos;
		texture = 
		"[     BBB     ]"
		"[   BB"+c+""+c+""+c+"BB   ]"
		"[ "+c+"B"+c+""+c+"GGG"+c+""+c+"B"+c+" ]"
		"[ "+c+"B"+c+"GGGGG"+c+"B"+c+" ]"
		"[ "+c+"B"+c+" GGG "+c+"B"+c+" ]"
		"[ "+c+"B"+c+" GGG "+c+"B"+c+" ]"
		"[ "+c+"B GG"+c+"GG B"+c+" ]"
		"[   B"+c+"G"+c+"G"+c+"B   ]"
		"[   B"+c+"GGG"+c+"B   ]"
		"[   B"+c+"GGG"+c+"B   ]"
		"[ "+c+"B"+c+"GGGGG"+c+"B"+c+" ]"
		"[ "+c+"BGGG"+c+"GGGB"+c+" ]"
		"[ "+c+"BGG"+c+""+c+""+c+"GGB"+c+" ]"
		"[  BGG"+c+""+c+""+c+"GGB  ]"
		"[  BGG"+c+""+c+""+c+"GGB  ]"
		"[   BB"+c+""+c+""+c+"BB   ]"
		"[     BBB     ]";
	}
};

struct GameData
{

	moto motoY = moto(V2(100, 100), V2(0, 0), V2(0, 0), "Y");
	moto motoB = moto(V2(500, 100), V2(0, 0), V2(0, 0), "C");

	GameData() {}

};



void Render(const GameData& G)  // const ref => garantit que l'on ne modifie pas les donnes de G
{
	G2D::clearScreen(Color::Black);
	 
	//dessine un rectangle de couleur bleu qui fait le tour de l'ecran
	G2D::drawRectangle(V2(0, 0), V2(1600, 900), Color::Blue, 1);

	//dessine un rectangle de couleur noir un peu plus petit que le rectangle bleu
	G2D::drawRectangle(V2(50, 50), V2(1500, 800), Color::Black, 1);

	G2D::drawRectWithTexture(G.motoY.IdTex, G.motoY.Pos,   G.motoY.Size);
	G2D::drawRectWithTexture(G.motoB.IdTex, G.motoB.Pos,   G.motoB.Size);

	G2D::Show();
}


void Logic(GameData& G)
{
	
}
 

void AssetsInit(GameData & G)
{
   // Size pass� en ref et texture en param
    G.motoY.IdTex = G2D::initTextureFromString(G.motoY.Size, G.motoY.texture);  
    G.motoY.Size  = G.motoY.Size  * 2; // on peut zoomer la taille du sprite

	G.motoB.IdTex = G2D::initTextureFromString(G.motoB.Size, G.motoB.texture);
	G.motoB.Size = G.motoB.Size * 2; // on peut zoomer la taille du sprite

}

int main(int argc, char* argv[])
{ 
	GameData G;

	G2D::initWindow(V2(1600,900), V2(150,100), string("TRON"));
	  
	AssetsInit(G);

	int nbCalltoLogicPerSec = 50;

	G2D::Run(Logic,Render,G, nbCalltoLogicPerSec,true);
 
}
 
  
 