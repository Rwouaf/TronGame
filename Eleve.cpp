#pragma warning( disable : 4996 ) 

 
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include "G2D.h"
#include <random>

using namespace std;
  
//struct Bonus 
struct Bonus
{
	V2 Pos;
	V2 Size;
	string Name;
	int IdSprite;
	bool active = false;

	Bonus() {}

	Bonus(string name,V2 pos, V2 size)
	{
		Name = name;
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
	Bonus bonus;
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

struct GameData
{

	Bonus ShieldY = Bonus("shield",V2(5, 855), V2(20, 20));
	Bonus SpeedY = Bonus("speed",V2(5, 855), V2(20, 20));

	vector<Bonus> BonusListY = { ShieldY,SpeedY };

	Bonus ShieldB = Bonus("shield",V2(1555, 855), V2(20, 20));
	Bonus SpeedB = Bonus("speed",V2(1555, 855), V2(20, 20));

	vector<Bonus> BonusListB = { ShieldB,SpeedB };

	moto motoY = moto(V2(500, 425), V2(20, 20), V2(0, 1));
	moto motoB = moto(V2(1000, 425), V2(20, 20), V2(0, -1));


	GameData() {
	}
};

//enum Ecran : permet de definir les differents ecrans du jeu
enum Ecran {
    ECRAN_ACCEUIL,
	ECRAN_BONUS,
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
	G2D::drawStringFontMono(V2(650, 600), "TRON",100 , 10, Color::Yellow);
	G2D::drawStringFontMono(V2(650, 600), "TRON",100 , 7, Color::Cyan);

	G2D::drawStringFontMono(V2(100, 400), "Appuyer sur Entrer pour commencer ou B pour voir les bonus",35 , 5, Color::Yellow);
	G2D::drawStringFontMono(V2(100, 400), "Appuyer sur Entrer pour commencer ou B pour voir les bonus",35 , 3, Color::Cyan);

	G2D::drawRectWithTexture(G.motoY.IdSpriteUP ,V2(500,580), V2(100,100));
	G2D::drawRectWithTexture(G.motoB.IdSpriteUP ,V2(975,580), V2(100,100));
	G2D::Show();
	if (G2D::isKeyPressed(Key::ENTER)) {
		return ECRAN_JEU;
	}
	else if (G2D::isKeyPressed(Key::B)){
		return ECRAN_BONUS;
	}
	
	return ECRAN_ACCEUIL;
}

Ecran gestionEcranBonus(const GameData& G){
	G2D::clearScreen(Color::Black);
	//affiche les explications au centre de l'ecran
	G2D::drawStringFontMono(V2(100, 500),"Bouclier: invincibilite pendant une courte duree.",30 ,4, Color::Magenta);
	G2D::drawStringFontMono(V2(100, 400),"Acceleration: vitesse doublee pendant une courte duree.", 30, 4, Color::Magenta);
	G2D::drawStringFontMono(V2(40, 300),"Appuyer sur H pour revenir a l'accueil.",30 ,4, Color::Red);
	G2D::drawRectWithTexture(G.ShieldY.IdSprite,V2(20, 490), V2(50, 50));
	G2D::drawRectWithTexture(G.SpeedY.IdSprite,V2(20, 390), V2(50, 50));
	G2D::Show();
	if(G2D::isKeyPressed(Key::H)){
		return ECRAN_ACCEUIL;
	}
	return ECRAN_BONUS;
}

//fonction qui gere l'ecran de jeu
Ecran gestionEcrantJeu(const GameData& G){
	
	G2D::clearScreen(Color::Black);
	 
	//dessine un rectangle de couleur bleu qui fait le tour de l'ecran
	G2D::drawRectangle(V2(0, 0), V2(1600, 900), Color::Blue, 1);

	//dessine les carres des joueurs ou les bonus seront affiche
	G2D::drawRectangle(V2(0,850), V2(50,50), Color::Yellow, 1);
	G2D::drawRectangle(V2(0,855),V2(45,45),Color::Gray,1);
	G2D::drawRectangle(V2(1550,850), V2(50,50), Color::Cyan, 1);
	G2D::drawRectangle(V2(1555,855),V2(45,45),Color::Gray,1);
	
	//dessine les bonus pour joueur 1
	if (G.motoY.bonus.Name == "shield") {
		G2D::drawRectWithTexture(G.ShieldY.IdSprite,G.ShieldY.Pos,G.ShieldY.Size);
	}
	else if (G.motoY.bonus.Name == "speed") {
		G2D::drawRectWithTexture(G.SpeedY.IdSprite,G.SpeedY.Pos,G.SpeedY.Size);
	}

	//dessine les bonus pour joueur 2
	if (G.motoB.bonus.Name == "shield") {
		G2D::drawRectWithTexture(G.ShieldB.IdSprite,G.ShieldB.Pos,G.ShieldB.Size);
	}
	else if (G.motoB.bonus.Name == "speed") {
		G2D::drawRectWithTexture(G.SpeedB.IdSprite,G.SpeedB.Pos,G.SpeedB.Size);
	}

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

	//si previousePos n'est pas vide, on dessine un rectangle de couleur jaune à l'arrière de la moto
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

	//dessine un cercle vide de couleur jaune autour de la moto jaune si le bonus shield est actif
	if (G.ShieldY.active) {
		G2D::drawCircle(G.motoY.Pos+V2(15,15), 25, Color::Yellow);
	}
	if (G.ShieldB.active) {
		G2D::drawCircle(G.motoB.Pos+V2(15,15), 25, Color::Cyan);
	}



	G2D::Show();
	return ECRAN_JEU;
}
//fonction qui gere l'ecran de win de J1
Ecran gestionEcrantJ1Gagne(const GameData& G){
	
	G2D::clearScreen(Color::Black);
	//affiche le texte au centre de l'ecran
	G2D::drawStringFontMono(V2(640, 600), "PLayer 1 WIN",35 , 5, Color::Yellow);
	G2D::drawStringFontMono(V2(550, 500), "Press enter to restart ",35 , 5, Color::Yellow);
	G2D::drawStringFontMono(V2(620, 400), "Press H to quit ",35 , 5, Color::Yellow);
	G2D::Show();
	if (G2D::isKeyPressed(Key::ENTER)) {
		return ECRAN_ACCEUIL;
	}
	if (G2D::isKeyPressed(Key::H)) {
		return ECRAN_ACCEUIL;
	}
	return ECRAN_J1_GAGNE;
}

//fonction qui gere l'ecran de win de J2
Ecran gestionEcrantJ2Gagne(const GameData& G){
	
	G2D::clearScreen(Color::Black);
	//affiche le texte au centre de l'ecran
	G2D::drawStringFontMono(V2(640, 600), "Player 2 WIN",35 , 5, Color::Cyan);
	G2D::drawStringFontMono(V2(550, 500), "Press enter to restart",35 , 5, Color::Cyan);
	G2D::drawStringFontMono(V2(620, 400), "Press H to quit",35 , 5, Color::Cyan);
	G2D::Show();
	if (G2D::isKeyPressed(Key::ENTER)) {
		return ECRAN_ACCEUIL;
	}
	if (G2D::isKeyPressed(Key::H)) {
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
		case ECRAN_BONUS:
			ecran = gestionEcranBonus(G);
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
	if (G.SpeedY.active == true){
		G.motoY.Pos = G.motoY.Pos + G.motoY.Dir * 5;
		G.motoY.previousPos.push_back(G.motoY.Pos);
	} else {
		G.motoY.Pos = G.motoY.Pos + G.motoY.Dir * 3;
		G.motoY.previousPos.push_back(G.motoY.Pos);
	}
	
}

//fonction de déplacement de la motoB
void moveMotoB(GameData& G)
{
	if (G.SpeedB.active == true){
		G.motoB.Pos = G.motoB.Pos + G.motoB.Dir * 5;
		G.motoB.previousPos.push_back(G.motoB.Pos);
	} else {
		G.motoB.Pos = G.motoB.Pos + G.motoB.Dir * 3;
		G.motoB.previousPos.push_back(G.motoB.Pos);
	}
	
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

//return true si 2 objets se rencontrent
bool RectCollision(const V2& pos1, const V2& size1, const V2& pos2, const V2& size2) {
    return (pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x && pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y);
}

//return true si la motoY rencontre un laser à elle ou de la motoB
bool CollisionY(const GameData& G)
{
	for (int i = 0; i < G.motoB.previousPos.size(); i++)
	{
		if (RectCollision(G.motoY.Pos, V2(10,10), G.motoB.previousPos[i], V2(2,2)) && G.ShieldY.active == false)
		{
			cout << "Y perd" << endl;
			return true;
		}
		if (RectCollision(G.motoY.Pos, V2(10,10), G.motoY.previousPos[i-20], V2(2,2)) && G.ShieldY.active == false)
		{
			cout << "Y autodestruction" << endl;
			return true;
		}
	}
	return false;
}

//return true si la motoB rencontre un laser à elle ou de la motoY
bool CollisionB(const GameData& G)
{
	for (int i = 0; i < G.motoY.previousPos.size(); i++)
	{
		if (RectCollision(G.motoB.Pos, V2(10,10), G.motoY.previousPos[i], V2(2,2)) && G.ShieldB.active == false)
		{
			cout << "B perd" << endl;
			return true;
		}
		if (RectCollision(G.motoB.Pos, V2(10,10), G.motoB.previousPos[i-20], V2(2,2)) && G.ShieldB.active == false)
		{
			cout << "B autodestruction" << endl;
			return true;
		}
	}
	return false;
}

//fonction qui donne un bonus a la motoY toute les 20 secondes
bool bonusBoolY = true;
int bonusY;
void GiveBonusY(GameData& G)
{
	if (bonusBoolY){
		bonusY = G2D::elapsedTimeFromStartSeconds();
		bonusBoolY = false;
	}
	if (bonusY + 5 < G2D::elapsedTimeFromStartSeconds()) {
		// Choisir un bonus aléatoire dans la liste des bonus disponibles
		std::random_device rd;
   		std::mt19937 gen(rd());

    	std::uniform_int_distribution<> dis2(0, G.BonusListY.size() - 1);
    	Bonus randomBonus = G.BonusListY[dis2(gen)];
		G.motoY.bonus = randomBonus;
		bonusBoolY = true;
	}
}

//fonction qui donne un bonus a la motoB toute les 20 secondes
bool bonusBoolB = true;
int bonusB;
void GiveBonusB(GameData& G)
{
	if (bonusBoolB){
		bonusB = G2D::elapsedTimeFromStartSeconds();
		bonusBoolB = false;
	}
	if (bonusB + 5 < G2D::elapsedTimeFromStartSeconds()) {

		// Choisir un bonus aléatoire dans la liste des bonus disponibles
		std::random_device rd;
   		std::mt19937 gen(rd());

		std::uniform_int_distribution<> dis2(0, G.BonusListB.size() - 1);
		Bonus randomBonus = G.BonusListB[dis2(gen)];
		G.motoB.bonus = randomBonus;
		bonusBoolB = true;
	}
}

//fonction qui utilise le bonus de la motoY
bool timerSpeedBoolY = true;
int timerY;
//utilisatopn de l'accélération
void useSpeedY(GameData& G)
{
	if (G.motoY.bonus.Name == "speed" && G2D::isKeyPressed(Key::E)){
		if (timerSpeedBoolY){
			timerY = G2D::elapsedTimeFromStartSeconds();
			timerSpeedBoolY = false;
		}
		if (timerY + 2 > G2D::elapsedTimeFromStartSeconds()) {
			G.SpeedY.active = true;
		}
		else {
			G.motoY.bonus = Bonus();
		}
	}
	else {
		G.SpeedY.active = false;
		timerSpeedBoolY = true;
	}
}
//utilisation du bouclier
bool timerShieldBoolY = true;
void useShieldY(GameData& G)
{
	if (G.motoY.bonus.Name == "shield" && G2D::isKeyPressed(Key::E)){
		if (timerShieldBoolY){
			timerY = G2D::elapsedTimeFromStartSeconds();
			timerShieldBoolY = false;
		}
		if (timerY + 2 > G2D::elapsedTimeFromStartSeconds()) {
			G.ShieldY.active = true;
		}
		else {
			G.motoY.bonus = Bonus();
		}
	}
	else {
		G.ShieldY.active = false;
		timerShieldBoolY = true;		
	}
}


//fonction qui utilise le bonus de la motoB
bool timerSpeedBoolB = true;
int timerB;
//utilisation de l'accélération
void useSpeedB(GameData& G)
{
	if (G.motoB.bonus.Name == "speed" && G2D::isKeyPressed(Key::M)){
		if (timerSpeedBoolB){
			timerB = G2D::elapsedTimeFromStartSeconds();
			timerSpeedBoolB = false;
		}
		if (timerB + 2 > G2D::elapsedTimeFromStartSeconds()) {
			G.SpeedB.active = true;
		}
		else {
			G.motoB.bonus = Bonus();
		}
	}
	else {
		G.SpeedB.active = false;
		timerSpeedBoolB = true;
	}
}
//utilisation du bouclier
bool timerShieldBoolB = true;
void useShieldB(GameData& G)
{
	if (G.motoB.bonus.Name == "shield" && G2D::isKeyPressed(Key::M)){
		if (timerShieldBoolB){
			timerB = G2D::elapsedTimeFromStartSeconds();
			timerShieldBoolB = false;
		}
		if (timerB + 2 > G2D::elapsedTimeFromStartSeconds()) {
			G.ShieldB.active = true;
		}
		else {
			G.motoB.bonus = Bonus();
		}
	}
	else {
		G.ShieldB.active = false;
		timerShieldBoolB = true;		
	}
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
	G.motoY.Pos = V2(500, 450);
	G.motoY.Dir = V2(0, 1);
	G.motoY.previousPos.clear();
	G.motoB.Pos = V2(1000, 450);
	G.motoB.Dir = V2(0, -1);
	G.motoB.previousPos.clear();
	G.SpeedY.active = false;
	G.SpeedB.active = false;
	G.ShieldY.active = false;
	G.ShieldB.active = false;
	G.motoY.bonus = Bonus();
	G.motoB.bonus = Bonus();
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
		GiveBonusY(G);
		GiveBonusB(G);	
		useSpeedY(G);
		useShieldY(G);
		useSpeedB(G);
		useShieldB(G);
		endGame(G);
	}
}
 
//fonction de gestion des textures
void AssetsInit(GameData & G)
{
	G.motoY.InitTextures(".//assets//motoUP.ppm",".//assets//motoLeft.ppm",".//assets//motoRight.ppm",".//assets//motoDown.ppm");
	G.motoB.InitTextures(".//assets//motoUP2.ppm",".//assets//motoLeft2.ppm",".//assets//motoRight2.ppm",".//assets//motoDown2.ppm");
	G.ShieldY.InitTextures(".//assets//bouclier.ppm");
	G.ShieldB.InitTextures(".//assets//bouclier.ppm");
	G.SpeedY.InitTextures(".//assets//speed.ppm");
	G.SpeedB.InitTextures(".//assets//speed.ppm");
}

int main(int argc, char* argv[])
{ 
	GameData G;

	G2D::initWindow(V2(1600,900), V2(150,100), string("TRON"));
	  
	AssetsInit(G);

	int nbCalltoLogicPerSec = 50;

	G2D::Run(Logic,render,G, nbCalltoLogicPerSec,true);
 
}