#pragma warning( disable : 4996 ) 

 
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include "G2D.h"
 
 
using namespace std;
  

struct moto
{
	V2 Pos;
	V2 Size;
	V2 Dir;
	vector<V2> previousPos;
	int IdSpriteUP;
	int IdSpriteLeft;
	int IdSpriteRight;
	int IdSpriteDown;

	moto(V2 pos, V2 size, V2 dir)
	{
		Pos = pos;
		Size = size;
		Dir = dir;
	}
	void InitTextures(string textureUP, string textureLeft, string textureRight,string textureDown)
	{
		IdSpriteUP = G2D::extractTextureFromPPM(textureUP, true);
		IdSpriteLeft   = G2D::extractTextureFromPPM(textureLeft, true);
		IdSpriteRight  = G2D::extractTextureFromPPM(textureRight, true);
		IdSpriteDown   = G2D::extractTextureFromPPM(textureDown, true);
		int zoom = 2;
		Size = Size * zoom;
	}
};

struct GameData
{
	moto motoY = moto(V2(500, 425), V2(20, 20), V2(0, 1));
	moto motoB = moto(V2(1000, 425), V2(20, 20), V2(0, -1));
     
	GameData() {
	}
};


enum Ecran {
    ECRAN_ACCEUIL,
    ECRAN_JEU,
   	ECRAN_END
};

Ecran ecran = ECRAN_ACCEUIL;

Ecran gestionEcrantAcceuil(const GameData& G){
	
	G2D::clearScreen(Color::Black);
	//affiche le texte au centre de l'ecran
	G2D::drawStringFontMono(V2(400, 400), "Appuyer sur Entrer pour commencer",35 , 5, Color::Yellow);
	G2D::drawStringFontMono(V2(400, 400), "Appuyer sur Entrer pour commencer",35 , 4, Color::Cyan);
	G2D::Show();
	if (G2D::isKeyPressed(Key::ENTER)) {
		return ECRAN_JEU;
	}
	return ECRAN_ACCEUIL;
}

Ecran gestionEcrantJeu(const GameData& G){
	
	G2D::clearScreen(Color::Black);
	 
	//dessine un rectangle de couleur bleu qui fait le tour de l'ecran
	G2D::drawRectangle(V2(0, 0), V2(1600, 900), Color::Blue, 1);

	//dessine un rectangle de couleur noir un peu plus petit que le rectangle bleu
	G2D::drawRectangle(V2(50, 50), V2(1500, 800), Color::Black, 1);
	
	int idspriteY;
	if (G.motoY.Dir == V2(-1,0))  idspriteY = G.motoY.IdSpriteLeft;
	if (G.motoY.Dir == V2(1,0)) idspriteY = G.motoY.IdSpriteRight;
	if (G.motoY.Dir == V2(0,-1))  idspriteY = G.motoY.IdSpriteDown;
	if (G.motoY.Dir == V2(0,1)) idspriteY = G.motoY.IdSpriteUP;

	int idspriteB;
	if (G.motoB.Dir == V2(-1,0))  idspriteB = G.motoB.IdSpriteLeft;
	if (G.motoB.Dir == V2(1,0)) idspriteB = G.motoB.IdSpriteRight;
	if (G.motoB.Dir == V2(0,-1))  idspriteB = G.motoB.IdSpriteDown;
	if (G.motoB.Dir == V2(0,1)) idspriteB = G.motoB.IdSpriteUP;

	G2D::drawRectWithTexture(idspriteY,G.motoY.Pos, G.motoY.Size);
	G2D::drawRectWithTexture(idspriteB,G.motoB.Pos, G.motoB.Size);

	//si previousePos n'est vide pas vide, on dessine un rectangle de couleur jaune à l'arrière de la moto
	if (G.motoY.previousPos.size() > 0) {
		for (int i = 0; i < G.motoY.previousPos.size(); i++) {
			G2D::drawRectangle(G.motoY.previousPos[i], V2(5,5), Color::Yellow, 1);
		}
	}

	G2D::Show();
	return ECRAN_JEU;
}

void render(const GameData& G)  // const ref => garantit que l'on ne modifie pas les donnes de G
{
	switch (ecran)
	{
		case ECRAN_ACCEUIL:
			ecran = gestionEcrantAcceuil(G);
			break;
		case ECRAN_JEU:
			ecran = gestionEcrantJeu(G);
			break;
		case ECRAN_END:
			break;
	}
}



//fonction de déplacement de la moto
void moveMotoY(GameData& G)
{
	G.motoY.Pos = G.motoY.Pos + G.motoY.Dir * 3;
	G.motoY.previousPos.push_back(G.motoY.Pos);
}

void moveMotoB(GameData& G)
{
	G.motoB.Pos = G.motoB.Pos + G.motoB.Dir * 3;
	G.motoB.previousPos.push_back(G.motoB.Pos);
}
 
//gestion des touches pour le joueur1 et l'empêche de faire demi-tour
void gestionToucheJ1(GameData& G)
{
	if (G2D::isKeyPressed(Key::Z) && (G.motoY.Dir != V2(0, -1)))
	{
		G.motoY.Dir = V2(0, 1);	
	}
	else if (G2D::isKeyPressed(Key::S) && (G.motoY.Dir != V2(0, 1)))
	{
		G.motoY.Dir = V2(0, -1);
	}
	else if (G2D::isKeyPressed(Key::Q) && (G.motoY.Dir != V2(1, 0)))
	{
		G.motoY.Dir = V2(-1, 0);
	}
	else if (G2D::isKeyPressed(Key::D) && (G.motoY.Dir != V2(-1, 0)))
	{
		G.motoY.Dir = V2(1, 0);
	}
	
}

void gestionToucheJ2(GameData& G)
{
	if (G2D::isKeyPressed(Key::UP) && (G.motoB.Dir != V2(0, -1)))
	{
		G.motoB.Dir = V2(0, 1);	
	}
	else if (G2D::isKeyPressed(Key::DOWN) && (G.motoB.Dir != V2(0, 1)))
	{
		G.motoB.Dir = V2(0, -1);
	}
	else if (G2D::isKeyPressed(Key::LEFT) && (G.motoB.Dir != V2(1, 0)))
	{
		G.motoB.Dir = V2(-1, 0);
	}
	else if (G2D::isKeyPressed(Key::RIGHT) && (G.motoB.Dir != V2(-1, 0)))
	{
		G.motoB.Dir = V2(1, 0);
	}
	
}

void Logic(GameData& G)
{
	moveMotoY(G);
	moveMotoB(G);
	gestionToucheJ1(G);
	gestionToucheJ2(G);
}
 

void AssetsInit(GameData & G)
{
	G.motoY.InitTextures(".//assets//motoUP.ppm",".//assets//motoLeft.ppm",".//assets//motoRight.ppm",".//assets//motoDown.ppm");
	G.motoB.InitTextures(".//assets//motoUP2.ppm",".//assets//motoLeft2.ppm",".//assets//motoRight2.ppm",".//assets//motoDown2.ppm");
}

int main(int argc, char* argv[])
{ 
	GameData G;

	G2D::initWindow(V2(1600,900), V2(150,100), string("TRON"));
	  
	AssetsInit(G);

	int nbCalltoLogicPerSec = 50;

	G2D::Run(Logic,render,G, nbCalltoLogicPerSec,true);
 
}
 
  
 
  
 