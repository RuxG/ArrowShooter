#331CA Grigorie Ruxandra


##Detalii implementare
------------------------
------------------------


1) Gameplay
-------------
	** Jocul incepe in first-person. Pentru a comuta intre first si third person, se apasa tasta "Q".

	** Jucatorul are initial 3 vieti. La coliziunea cu obstacole, sau la pierderea intregului combustibil,
	acesta pierde o viata. La coliziunea cu o platforma roz, acesta castiga o viata (pana la maxim 3).

	** W - creste viteza
	** S - scade viteza
	** A - deplasare stanga
	** D - deplasare dreapta
	
	** Space - saritura


2) Generare platforme
-----------------------

	** Generez platformele linie cu linie.

	** Platformele de pe o linie au aceeasi lungime, generata random la creearea lor.

	** O linie de platforme poate avea una din urmatoarele 7 configuratii posibile:

		0. [platforma] [platforma] [platforma]
		1. [platforma] [platforma] [  spatiu ]
		2. [  spatiu ] [platforma] [platforma]
		3. [platforma] [  spatiu ] [platforma]
		4. [platforma] [  spatiu ] [  spatiu ]
		5. [  spatiu ] [platforma] [  spatiu ]
      	  	6. [  spatiu ] [  spatiu ] [platforma]

	** Pentru a ma asigura ca exista intotdeauna un drum bun, pe care jucatorul il poate urmari, 
	nu voi genera niciodata urmatoarele secvente de linii consecutiv:
	
		* 3 si (4, 6)
		* 4 si 6
		* 6 si 4

	** Platformele marcate cu "[paltforma]" pot avea culorile albastru (cu probabilitate mare), 
	sau verde (cu probabilitate mai mica, pentru a ma asigura ca jucatorul primeste suficient
	combustibil). 

	** In locurile marcate cu "[spatiu]" voi genera (sau nu, cu probabilitate de 0.5), o platforma extra,
	speciala (verde, galbena, portocalie, rosie, roz).

	** Astfel, daca jucatorul urmareste secventa de platforme corecta, el poate continua jocul la infinit.



3) Efecte speciale platforme
-----------------------------

	** Platformele speciale verzi, galbene si portocalii, ofera jucatorului un buff, care dureaza BUFF_TIME
	secunde. 
	
	** Platformele roz si rosii (care confera o viata, respectiv incheie jocul), isi exercita efectul instant.

	** La coliziunea jucatorului cu alta platforma speciala, buff-ul curent se inlocuieste cu cel nou.

4) Bonusuri
--------------

	** Am introdus obstacole. Acestea apar random, cu o probabilitate, pe platforme extra. 
	Coliziunea cu acestea va duce o pierderea unei vieti.
	
	** Am introdus platforme roz, care ofera jucatorului vieti. 

