Galie Ana Maria 312CD

_._._._._._._[[Preparations and functions that keep being reused]]_._._._._._._._

[list.h]: s-a implementat o structura lista care va folosita dealungul programului
	  + functii de:
		-creare lista si noduri (createList/createNode)
		-construire a unei liste din nodurile date din tree (constructList)
		-inserare a unui nod (insertNode)
		- eliberare lista (freeList)

[hierarchy.c]: -search
		-sortEmployees
		-insertUnfireEmployee
		-moveSubordinates

_._._._._._._._._._[[CERINTA 1]]_._._._._._._._._._._._._._

[____[HIRE]____]

Se verifica cazul in care root (tree) este NULL sau manager_name este NULL.
Daca da se face o alocare speciala a root-ului + team.
In orice alt caz se aloca memoria pentru nod si team-ul sau.

Se verifica daca managerul unde trebuie pus nodul este chiar root-ul:
=> nu mai este nevoie sa cautam nodul prin ierarhie cu functia (search)
=> se creaza conexiunile dintre employee si root 
=> se sorteaza vectorul de noduri team cu ajutorul (sortEmployee)

[sortEmployee foloseste metoda bubblesort pentru a reorganiza team-ul unui manager]

Daca managerul NU este root atunci se cauta prin ierarhie managerul cu (search) iar
pasii de creare a legaturii sunt la fel ca in cazul precedent.

[search se foloseste de structura de lista si functiile din list.h]
- pune toate referintele nodurilor din tree intr-o lista pentru a usura cautarea
- lista este eliberata in momentul gasirii managerului


[____[FIRE]____]

Se verifica conditiile de existenta impuse in cerinta dupa gasirea employee-ului.
Se retine managerul nodului.

[moveSubordinates se foloseste list.h]
- se muta toti subordonatii nodului employee la team-ul managerului actual
- in acelati timp se sterg din team-ul lui employee care urmeaza sa fie concediat
- se creaza o functie si se apeleaza (insertUnfiredEmployee)
- nodurile sunt asezate inapoi in vectorul team fara employeeToFire
- se elibereaza lista

[insertUnfiredEmployee]
- toti angazatii care nu au employee_name vor fi mutati in lista
- pozitiile lor initiala in team vor fi facute NULLe

In final se elibereaza team si nodul in sine si se sorteaza echipa managerului.

[___[PROMOTE]___]

Se verifica conditiile de existenta (sa nu fie root si sa nu fie pe nivelul 1).

Pasii sunt asemanatori cu fire dar in loc sa se elibereze employee si employee->team
acesta va fi mutat fara echipa lui la team-ul manager-ul SUPERIOR (oldManager->manager).

Se resorteaza noua echipa.

[___[MOVE_EMPLOYEE]___]

Pasii si conditiile se repeta ca la PROMOTE.
Managerul acum va fi cautat cu (search) prin ierarhie.

[___[MOVE_TEAM]___]

Se verifica conditiile + daca nodul are 0 subordonati se apeleaza direct (move_employee).

Se taie conexiunile dintre liderul employee si managerul vechi 
+ se reface echipa managerului ca la (fire) + se sorteaza.

Atasarea/Mutarea echipei se va face in continuare ca la move_employee

[___[FIRE_TEAM]___]

Se verifica condiitile date.
Parcurgere identica ca la (move_team) dar in loc sa se mute echipa aceasta este
eliberata folosindu-se functia (destroy_tree).

[___[PREORDE_TRAVERSAL]___]

- este construita o lista si este populata cu noduri cu referinta in apelarea recursiva
a functiei (constructList) in mod preorder.
- se afiseaza continutul listei in formatul dorit
- se elibereaza lista

[___[DESTROY TREE]___]

- creaza o lista cu toate nodurile din ierarhie
- ia fiecare nod in parte din lista cu referinta la angazatul din tree si ii elibereaza team,
team-ul in sine (care a fost alocat dinamic cu calloc in hire) si nodul din tree
- se elibereaza lista folosita



