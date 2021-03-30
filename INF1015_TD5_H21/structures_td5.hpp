/*
* INF1015 - TD no. 5 : Fichier source pour les structures de la partie 5 du TD en programmation orientée objet.
* \file		structures_td5.hpp
* \author	E.Barbeau et L-A.Reau
* \date		4 avril 2021
* Créé le	22 mars 2021
*/

// Solutionnaire du TD4 INF1015 hiver 2021
// Par Francois-R.Boyer@PolyMtl.ca

#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <iostream>
#include <memory>
#include <functional>
#include <cassert>
#include "gsl/span"
using gsl::span;
using namespace std;  // On le permet, mais j'ai écrit mon .hpp sans, avant de le permettre dans l'énoncé.

class Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {
public:
	ListeFilms() = default;
	void ajouterFilm(Film* film);
	//void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }
	int capacity() const { return capacite; }
	void detruire();
	Film*& operator[] (int index) { return elements[index]; }
	Film* trouver(const function<bool(const Film&)>& critere) {
		for (auto& film : enSpan())
			if (critere(*film))
				return film;
		return nullptr;
	}

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
};

template <typename T>
class Liste {
public:
	Liste() = default;
	Liste(int capaciteInitiale) :
		capacite(capaciteInitiale),
		elements(make_unique<shared_ptr<T>[]>(capacite))
	{
	}
	Liste(const Liste<T>& autre) :
		capacite(autre.nElements),
		nElements(autre.nElements),
		elements(make_unique<shared_ptr<T>[]>(nElements))
	{
		for (int i = 0; i < nElements; ++i)
			elements[i] = autre.elements[i];
	}
	Liste(Liste<T>&&) = default;  // Pas nécessaire, mais puisque c'est si simpler avec unique_ptr...
	Liste<T>& operator=(Liste<T>&&) = default;  // Nécessaire dans mon cas pour l'initialisation dans lireFilm.

	void ajouter(shared_ptr<T> element)
	{
		assert(nElements < capacite);  // Comme dans le TD1, on ne demande pas la réallocation pour ListeActeurs...
		elements[nElements++] = move(element);
	}

	shared_ptr<T>& operator[] (int index) { return elements[index]; }
	span<shared_ptr<T>> enSpan() const { return span(elements.get(), nElements); }

	shared_ptr<T>* begin() { return &elements[0]; }
	shared_ptr<T>* end() { return &elements[nElements]; }

private:
	int capacite = 0, nElements = 0;
	unique_ptr<shared_ptr<T>[]> elements;
};

using ListeActeurs = Liste<Acteur>;

class Affichable {
public:
	virtual void afficherSur(ostream& os) const = 0;
	virtual ~Affichable() = default;
};

class Item : public Affichable {
public:
	void afficherSur(ostream& os) const override;
	void lireDe(istream& is);

	string titre;
	int anneeSortie = 0;
};

class Film : virtual public Item {
public:
	void afficherSur(ostream& os) const override;
	void afficherSpecifiqueSur(ostream& os) const;  // Affiche la parite de cette classe sans afficher la base virtuelle.

	string realisateur; // (on suppose qu'il n'y a qu'un réalisateur).
	int recette = 0; // Recette globale du film en millions de dollars
	ListeActeurs acteurs;
};

class Livre : virtual public Item {
public:
	Livre() = default;
	explicit Livre(istream& is);
	void afficherSur(ostream& os) const override;
	void afficherSpecifiqueSur(ostream& os) const;
	void lireDe(istream& is);

	string auteur;
	int copiesVendues=0, nPages=0;
};

class FilmLivre : public Film, public Livre {
public:
	FilmLivre(const Film& film, const Livre& livre) : Item(film), Film(film), Livre(livre) { }

	void afficherSur(ostream& os) const override;
};

struct Acteur {
	string nom; int anneeNaissance=0; char sexe='\0';
};
