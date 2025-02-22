/**
 * Physique.cpp : Vous trouverez ici toutes les fonctions implemente pour la classe Physique.
 * Auteur : Quentin GRUCHET & Fadi MECHRI.
 **/

#include "Physique.hpp"

/**
  * @brief Constructeur de la classe Physique.
  * 
  * Le constructeur est vide car nous utilisons les setters pour initialiser les differents paramètres.
  * 
  * @return NULL.
  **/
Physique::Physique() {
    
}

/**
  * @brief Destructeur de la classe Physique.
  * 
  * Le destructeur est vide car tout est géré par le 'garbage collector'.
  * 
  * @return NULL.
  **/
Physique::~Physique() {

}

/**
  * @brief Setter de l'attribut de classe m_SrcMac.
  * 
  * @param src : Adresse MAC source souhaitée.
  * @return void.
  **/
void Physique::setMacSrc(MAC src) {
    m_SrcMac = src;
}

/**
  * @brief Getter de l'attribut de classe m_SrcMac.
  *
  * @return La valeur de l'adresse MAC source.
  **/
MAC Physique::getMacSrc() {
    return m_SrcMac;
}

/**
  * @brief Setter de l'attribut de classe m_DestMac.
  * 
  * @param dest : Adresse destination souhaitée.
  * @return void.
  **/
void Physique::setMacDest(MAC dest) {
    m_DestMac = dest;
}

/**
  * @brief Getter de l'attribut de classe m_DestMac.
  * 
  * @return La valeur de l'adresse MAC de destination.
  **/
MAC Physique::getMacDest() {
    return m_DestMac;
}

/**
  * @brief Permet de convertir une adresse MAC en un std::bitset de 48 bits en vue de l'encapsuler.
  * 
  * On converti d'abord chaque champs de la structure MAC en chaine de caractère. 
  * Puis nous créeons un bitset de la concaténation de ces chaines de caractère. 
  * 
  * @param adresse : L'adresse MAC que l'on souhaite convertir.
  * @return Le bitset resultant de la convertion.
  **/
std::bitset<48> Physique::convertir(const MAC& adresse) {
	std::string s1 = adresse.a.to_string();
	std::string s2 = adresse.b.to_string();
	std::string s3 = adresse.c.to_string();
	std::string s4 = adresse.d.to_string();
	std::string s5 = adresse.e.to_string();
	std::string s6 = adresse.f.to_string();
	return std::bitset<48>(s1 + s2 + s3 + s4 + s5 + s6);
}

/**
  * @brief Permet de convertir deux bitset de 16 bits en une structure MAC.
  * 
  * Après désencapsulation, les adresses MAC sont contenues dans trois std::bitset<16> ou chaque 8 bits représente un champ de l'adresse MAC.
  * Cette fonction permet donc de diviser ces bitset en deux, pour remplir la MAC.
  * 
  * @param adrPartBA : Premier bitset contenant les champs a et b.
  * @param adrPartDC : Second bitset contenant les champs c et d.
  * @param adrPartFE : Troisième bitset contenant les champs e et f.
  * @return Une structure MAC avec les champs remplie.
  **/
MAC Physique::convertir(const std::bitset<16>& adrPartBA, 
    const std::bitset<16>& adrPartDC, const std::bitset<16>& adrPartFE) 
{
  // Initialisation de l'adress MAC.
  MAC mac;

  //division en deux de chaque bitset de 16 bits
  //et association dans les champs de mac;
  std::bitset<8> gauche, droite;

  diviser(adrPartBA, droite, gauche);
  mac.a = gauche;
  mac.b = droite;

  diviser(adrPartDC, droite, gauche);
  mac.c = gauche;
  mac.d = droite;

  diviser(adrPartFE, droite, gauche);
  mac.e = gauche; 
  mac.f = droite;

  return mac;
}

/**
  * @brief Permet de convertir un bitset de 48 bits en une pile de bitset de 16 bits.
  * 
  * @param adresse Le bitset que l'on veut diviser.
  * @return La pile contenant l'argument découpé en plusieurs morceaux.
  **/
std::stack<std::bitset<16>> Physique::decoupageMac(const std::bitset<48>& adresse) {
  std::stack<std::bitset<16>> pile;
  std::bitset<16> eltPile;

  //
  size_t i;
  size_t posAdr = 0;
  for(i = 0; i < 16; ++i) {
    eltPile[i] = adresse[posAdr];
    posAdr++;
  }
  pile.push(eltPile);
  for(i = 0; i < 16; ++i) {
    eltPile[i] = adresse[posAdr];
    posAdr++;
  }
  pile.push(eltPile);
  for(i = 0; i < 16; ++i) {
    eltPile[i] = adresse[posAdr];
    posAdr++;
  }
  pile.push(eltPile);

  return pile;
}

/**
  * @brief Permet l'encapsulation de la couche Physique.
  * 
  * @param paquet : Resultat de l'encapsulation de la couche Internet.
  * @return. Le resultat de la désencapsulation. Donc une pile contenant la couche Physique + Internet + Transport.
  **/
 std::stack<std::bitset<16>> Physique::encapsuler(std::stack<std::bitset<16>>& paquet) {
    paquet.push(concat(m_SrcMac.a, m_SrcMac.b));
    paquet.push(concat(m_SrcMac.c, m_SrcMac.d));
    paquet.push(concat(m_SrcMac.e, m_SrcMac.f));
    paquet.push(concat(m_DestMac.a, m_DestMac.b));
    paquet.push(concat(m_DestMac.c, m_DestMac.d));
    paquet.push(concat(m_DestMac.e, m_DestMac.f));

    return paquet;
 }

/**
  * @brief Permet la desencapsulation de la couche Physique.
  * 
  * @param trame. Une pile qui contitent la couche Transport + Internet + Physique. 
  * @return La même pile mais avec la couche Physique en moins.
  **/
std::stack<std::bitset<16>> Physique::desencapsuler(std::stack<std::bitset<16>>& trame) {
    for (size_t i = 0; i < 6; i++)
    {
        trame.pop();
    }

    return trame;
}