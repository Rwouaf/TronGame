#pragma warning( disable : 4996 ) 

 
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include "G2D.h"
#include <thread>
#include <chrono>
 
 
using namespace std;
  
//struct moto
//Pos : possition de la moto
//Size : taille de la moto
//Dir : direction de la moto
//previousPos : tableau de V2 qui contient les positions precedentes de la moto
//IdSpriteUP : id de la texture de la moto quand elle va vers le haut
//IdSpriteLeft : id de la texture de la moto quand elle va vers la gauche
//IdSpriteRight : id de la texture de la moto quand elle va vers la droite
//IdSpriteDown : id de la texture de la moto quand elle va vers le bas
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
	//initialise les textures de la moto
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

struct explosions
{
	int idSprite1;
	int idSprite2;
	int idSprite3;
	int idSprite4;
	int idSprite5;
	int idSprite6;
	
	V2 Pos;
	V2 Size;

	explosions(V2 pos, V2 size)
	{
		Pos = pos;
		Size = size;
	}
	//initialise les textures de l'explosion
	void InitTextures(string texture1, string texture2, string texture3, string texture4, string texture5, string texture6)
	{
		idSprite1 = G2D::extractTextureFromPPM(texture1, true);
		idSprite2 = G2D::extractTextureFromPPM(texture2, true);
		idSprite3 = G2D::extractTextureFromPPM(texture3, true);
		idSprite4 = G2D::extractTextureFromPPM(texture4, true);
		idSprite5 = G2D::extractTextureFromPPM(texture5, true);
		idSprite6 = G2D::extractTextureFromPPM(texture6, true);
		int zoom = 2;
		Size = Size * zoom;
	}

};

struct Bonus
{
	V2 Pos;
	V2 Size;
	int IdSprite;

	Bonus(V2 pos, V2 size)
	{
		Pos = pos;
		Size = size;
	}
	//initialise les textures du bonus
	void InitTextures(string texture)
	{
		IdSprite = G2D::extractTextureFromPPM(texture, true);
		int zoom = 2;
		Size = Size * zoom;
	}
};


struct GameData
{
	moto motoY = moto(V2(500, 425), V2(20, 20), V2(0, 1));
	moto motoB = moto(V2(1000, 425), V2(20, 20), V2(0, -1));

	explosions explosion = explosions(V2(0, 0), V2(20, 20));


	GameData() {
	}
};

//enum Ecran : permet de definir les differents ecrans du jeu
enum Ecran {
    ECRAN_ACCEUIL,
    ECRAN_JEU,
   	ECRAN_J1_GAGNE,
	ECRAN_J2_GAGNE,
};
//variable qui contient l'ecran actuel
Ecran ecran = ECRAN_ACCEUIL;

//fonction qui gere l'ecran d'acceuil
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


//fonction qui gere l'ecran de jeu
Ecran gestionEcrantJeu(const GameData& G){
	
	G2D::clearScreen(Color::Black);
	 
	//dessine un rectangle de couleur bleu qui fait le tour de l'ecran
	G2D::drawRectangle(V2(0, 0), V2(1600, 900), Color::Blue, 1);

	//dessine un rectangle de couleur noir un peu plus petit que le rectangle bleu
	G2D::drawRectangle(V2(50, 50), V2(1500, 800), Color::Black, 1);

	//gestion des spirites de la moto jaune en fonction de la direction de la moto
	int idspriteY;
	if (G.motoY.Dir == V2(-1,0))  idspriteY = G.motoY.IdSpriteLeft;
	if (G.motoY.Dir == V2(1,0)) idspriteY = G.motoY.IdSpriteRight;
	if (G.motoY.Dir == V2(0,-1))  idspriteY = G.motoY.IdSpriteDown;
	if (G.motoY.Dir == V2(0,1)) idspriteY = G.motoY.IdSpriteUP;

	//gestion des spirites de la moto bleu en fonction de la direction de la moto
	int idspriteB;
	if (G.motoB.Dir == V2(-1,0))  idspriteB = G.motoB.IdSpriteLeft;
	if (G.motoB.Dir == V2(1,0)) idspriteB = G.motoB.IdSpriteRight;
	if (G.motoB.Dir == V2(0,-1))  idspriteB = G.motoB.IdSpriteDown;
	if (G.motoB.Dir == V2(0,1)) idspriteB = G.motoB.IdSpriteUP;

	//si previousePos n'est vide pas vide, on dessine un rectangle de couleur jaune à l'arrière de la moto
	if (G.motoY.previousPos.size() > 0) {
		for (int i = 0; i < G.motoY.previousPos.size(); i++) {
			G2D::drawRectangle(G.motoY.previousPos[i]+V2(15,15), V2(5,5), Color::Yellow, 1);
		}
	}
	//same pour la moto bleu
	if (G.motoB.previousPos.size() > 0) {
		for (int i = 0; i < G.motoB.previousPos.size(); i++) {
			G2D::drawRectangle(G.motoB.previousPos[i]+V2(15,15), V2(5,5), Color::Cyan, 1);
		}
	}

	//dessine les motos
	G2D::drawRectWithTexture(idspriteY,G.motoY.Pos, G.motoY.Size);
	G2D::drawRectWithTexture(idspriteB,G.motoB.Pos, G.motoB.Size);

	G2D::Show();
	return ECRAN_JEU;
}
//fonction qui gere l'ecran de win de J1
Ecran gestionEcrantJ1Gagne(const GameData& G){
	
	G2D::clearScreen(Color::Black);
	//affiche le texte au centre de l'ecran
	G2D::drawStringFontMono(V2(400, 400), "Joueur 1 a gagne",35 , 5, Color::Yellow);
	G2D::drawStringFontMono(V2(350, 200), "Press enter to restart",35 , 5, Color::Yellow);
	G2D::Show();
	if (G2D::isKeyPressed(Key::ENTER)) {
		return ECRAN_ACCEUIL;
	}
	return ECRAN_J1_GAGNE;
}

//fonction qui gere l'ecran de win de J2
Ecran gestionEcrantJ2Gagne(const GameData& G){
	
	G2D::clearScreen(Color::Black);
	//affiche le texte au centre de l'ecran
	G2D::drawStringFontMono(V2(400, 400), "Joueur 2 a gagne",35 , 5, Color::Cyan);
	G2D::drawStringFontMono(V2(350, 200), "Press enter to restart",35 , 5, Color::Cyan);
	G2D::Show();
	if (G2D::isKeyPressed(Key::ENTER)) {
		return ECRAN_ACCEUIL;
	}
	return ECRAN_J2_GAGNE;
}

void render(const GameData& G)  // const ref => garantit que l'on ne modifie pas les donnes de G
{
	//en fonction de l'ecran actuel, on appelle la fonction qui gere l'ecran
	switch (ecran)
	{
		case ECRAN_ACCEUIL:
			ecran = gestionEcrantAcceuil(G);
			break;
		case ECRAN_JEU:
			ecran = gestionEcrantJeu(G);
			break;
		case ECRAN_J1_GAGNE:
			ecran = gestionEcrantJ1Gagne(G);
			break;
		case ECRAN_J2_GAGNE:
			ecran = gestionEcrantJ2Gagne(G);
			break;
		default:
			break;
	}
}



//fonction de déplacement de la motoY
void moveMotoY(GameData& G)
{
	G.motoY.Pos = G.motoY.Pos + G.motoY.Dir * 3;
	G.motoY.previousPos.push_back(G.motoY.Pos);
}

//fonction de déplacement de la motoB
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

//gestion des touches pour le joueur2 et l'empêche de faire demi-tour
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


bool RectCollision(const V2& pos1, const V2& size1, const V2& pos2, const V2& size2) {
    return (pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x && pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y);
}


bool CollisionY(const GameData& G)
{
	for (int i = 0; i < G.motoB.previousPos.size(); i++)
	{
		if (RectCollision(G.motoY.Pos, V2(10,10), G.motoB.previousPos[i], V2(2,2)))
		{
			cout << "Y perd" << endl;
			return true;
		}
		if (RectCollision(G.motoY.Pos, V2(10,10), G.motoY.previousPos[i-20], V2(2,2)))
		{
			cout << "Y autodestruction" << endl;
			return true;
		}
	}
	return false;
}

bool CollisionB(const GameData& G)
{
	for (int i = 0; i < G.motoY.previousPos.size(); i++)
	{
		if (RectCollision(G.motoB.Pos, V2(10,10), G.motoY.previousPos[i], V2(2,2)))
		{
			cout << "B perd" << endl;
			return true;
		}
		if (RectCollision(G.motoB.Pos, V2(10,10), G.motoB.previousPos[i-20], V2(2,2)))
		{
			cout << "B autodestruction" << endl;
			return true;
		}
	}
	return false;
}


//fonction qui return true si une moto sort du rectangle noir
int OutOfBounds(const GameData& G)
{
	if (G.motoY.Pos.x < 50 || G.motoY.Pos.x > 1520 || G.motoY.Pos.y < 50 || G.motoY.Pos.y > 810) {
		return 1;
	}
	if (G.motoB.Pos.x < 50 || G.motoB.Pos.x > 1520 || G.motoB.Pos.y < 50 || G.motoB.Pos.y > 810) {
		return 2;
	}
	return 0;
}


//fonction qui reset le jeu
void reset(GameData& G)
{
	G.motoY.Pos = V2(100, 100);
	G.motoY.Dir = V2(1, 0);
	G.motoY.previousPos.clear();
	G.motoB.Pos = V2(1500, 800);
	G.motoB.Dir = V2(-1, 0);
	G.motoB.previousPos.clear();
}

//fonction de fin de jeu
void endGame(GameData& G)
{
	if (CollisionY(G) || OutOfBounds(G) == 1)
	{
		ecran = ECRAN_J2_GAGNE;
		reset(G);
	}
	if (CollisionB(G) || OutOfBounds(G) == 2)
	{
		ecran = ECRAN_J1_GAGNE;
		reset(G);
	}
} 

void Logic(GameData& G)
{
	if (ecran == ECRAN_JEU){
		moveMotoY(G);
		moveMotoB(G);
		gestionToucheJ1(G);
		gestionToucheJ2(G);
		endGame(G);
	}
}
 
//fonction de gestion des textures
void AssetsInit(GameData & G)
{
	G.motoY.InitTextures(".//assets//motoUP.ppm",".//assets//motoLeft.ppm",".//assets//motoRight.ppm",".//assets//motoDown.ppm");
	G.motoB.InitTextures(".//assets//motoUP2.ppm",".//assets//motoLeft2.ppm",".//assets//motoRight2.ppm",".//assets//motoDown2.ppm");
	G.explosion.InitTextures(".//explosions//explosion1.ppm",".//explosions//explosion2.ppm",".//explosions//explosion3.ppm",".//explosions//explosion4.ppm",".//explosions//explosion5.ppm",".//explosions//explosion6.ppm");
}

int main(int argc, char* argv[])
{ 
	GameData G;

	G2D::initWindow(V2(1600,900), V2(150,100), string("TRON"));
	  
	AssetsInit(G);

	int nbCalltoLogicPerSec = 50;

	G2D::Run(Logic,render,G, nbCalltoLogicPerSec,true);
 
}
 
  
 
  
 