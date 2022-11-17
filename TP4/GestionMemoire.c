/*
 * GestionMemoire
 *
 * Ecole polytechnique de Montreal, GIGL, Automne  2022
 * Julie Labbe 1904635, Nicolas Depelteau 2083544
*/

#include "./libs/lib.h"
#include "time.h"
#include "stdio.h"

#define TAILLE_DEPLACEMENT 10
#define MASQUE_PAGE 0x3FF
#define temps time

unsigned int calculerNumeroDePage(unsigned long adresse) {
    return( (unsigned int) (adresse >> TAILLE_DEPLACEMENT) );
}

unsigned long calculerDeplacementDansLaPage(unsigned long adresse) {
    return (adresse & MASQUE_PAGE);
}

unsigned long calculerAdresseComplete(unsigned int numeroDePage, unsigned long deplacementDansLaPage) {
	return ( ((unsigned long)numeroDePage << TAILLE_DEPLACEMENT) | deplacementDansLaPage );
}

void rechercherTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	unsigned int numeroPage = calculerNumeroDePage(req->adresseVirtuelle);
	for (unsigned int i = 0; i < TAILLE_TLB; ++i) {
		
		if (mem->tlb->numeroPage[i] == numeroPage && mem->tlb->entreeValide[i])
		{
			req->estDansTLB = 1;
			unsigned long adressePhysique  = calculerAdresseComplete(mem->tlb->numeroCadre[i], calculerDeplacementDansLaPage(req->adresseVirtuelle));
			req->adressePhysique = adressePhysique;
			temps(&(req->date));
			mem->tlb->dernierAcces[i] = req->date;
			return;
		}
	}
	req->estDansTLB = 0;
	req->adressePhysique = 0;
}

void rechercherTableDesPages(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	unsigned int numeroPage = calculerNumeroDePage(req->adresseVirtuelle);

	if (mem->tp->entreeValide[numeroPage]) {
		req->estDansTablePages = 1;
		req->adressePhysique = calculerAdresseComplete(mem->tp->numeroCadre[numeroPage], calculerDeplacementDansLaPage(req->adresseVirtuelle));
		return;
	}
	req->adressePhysique = 0;
	req->estDansTablePages = 0;
}

void ajouterDansMemoire(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	for (unsigned int i = 0; i < TAILLE_MEMOIRE; i++) {
		if (mem->memoire->utilisee[i] == 0) {
			mem->memoire->utilisee[i] = 1;
			mem->memoire->numeroPage[i] = calculerNumeroDePage(req->adresseVirtuelle);
			mem->memoire->dernierAcces[i] = req->date;
			mem->memoire->dateCreation[i] = req->date;
			req->adressePhysique = calculerAdresseComplete(i, calculerDeplacementDansLaPage(req->adresseVirtuelle));
			return;
		}
	}
}

void mettreAJourTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	unsigned int indiceARemplacer = 0;
	unsigned long datePlusAncien = mem->tlb->dateCreation[0];
	for (unsigned int i = 0; i < TAILLE_TLB; i++) {
		if(mem->tlb->entreeValide[i] == 0)
		{
			indiceARemplacer = i;
			break;
		}
		if (mem->tlb->dateCreation[i] < datePlusAncien) {
			indiceARemplacer = i;
			datePlusAncien = mem->tlb->dateCreation[i];
		}
	}
	mem->tlb->numeroPage[indiceARemplacer] = calculerNumeroDePage(req->adresseVirtuelle);
	mem->tlb->numeroCadre[indiceARemplacer] = calculerNumeroDePage(req->adressePhysique);
	mem->tlb->entreeValide[indiceARemplacer] = 1;
	mem->tlb->dernierAcces[indiceARemplacer] = req->date;
	mem->tlb->dateCreation[indiceARemplacer] = req->date;
}

// NE PAS MODIFIER
int main() {
    evaluate(
		&calculerNumeroDePage, 
		&calculerDeplacementDansLaPage, 
		&calculerAdresseComplete, 
        &rechercherTLB, 
		&rechercherTableDesPages,
		&mettreAJourTLB,
		&ajouterDansMemoire
    );
    return 0;
}
