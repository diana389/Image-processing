# IMAGE PROCESSING

Exista urmatoarele functii:

##  `TASK1:`
Se citesc datele din fisierul de intrare si se formeaza arborele pe baza matricei citite, prin functia `div_p`. Apoi, se formeaza vectorul pe baza arborelui, prin functia `tree_to_vector`, se scriu datele in fisierul de iesire si se elibereaza memoria.

##  `TASK2:`
Se citesc datele din fisierul de intrare si se formeaza arborele pe baza vectorului, prin functia `vector_to_tree`. Apoi, se formeaza matricea pe baza arborelui, prin functie `tree_to_matrix`, se scriu datele in fisierul de iesire si se elibereaza memoria.

##  `TASK3:`
Se citesc datele din fisierul de intrare si se formeaza arborele pe baza matricei citite, prin functia `div_p`. In functie de tipul primit ca argument, arborele se oglindeste, se formeaza noua matrice prin functia `tree_to_matrix`, se scriu datele in fisierul de iesire si se elibereaza memoria.

##  `AVERAGE:`
Primeste ca argumente matricea de pixeli, dimensiunea blocului pentru care se doreste media, indicii de la care incepe si variabilele in care se salveaza rezultatele si calculeaza media aritmetica.

##  `INIT:`
Aloca memorie pentru cei patru fii ai nodului transmis ca parametru.

##  `DIV_P:`
Se calculeaza media si scorul, iar apoi, in functie de scor, fie se aloca memorie pentru un nivel urmator si de continua impartirea, fie s-a ajuns la frunze. La final, pentru oricare dintre cazuri, se completeza datele nodului.

##  `TREE_TO_VECTOR:`
In cazul in care nodul este NULL inseamna ca nodul anterior a fost o frunza, iar acest lucru este marcat prin parametrul ***"x"*** si cand se revine la apelul anterior indicii iau valoarea -1. <br>
In caz contrar, indicii se completeaza in functie de variabila ***"indice_prev"***, care retine indicele elementului vizitat anterior, adica al fiului (deoarece functia este recursiva).

##  `VECTOR_TO_TREE:`
Daca nodul curent nu este frunza, se aloca memorie pentru urmatorul nivel si se continua parcurgerea, altfel pointerii catre nivelul urmator din arbore iau valoarea NULL. In ambele czauri, datele se completeaza in structura nodului.

##  `TREE_TO_MATRIX:`
Arborele se parcurge recursiv pana se ajunge la frunze, unde este completata zona corespunzatoare din matrice.

##  `MIRROR_H:`
Functia inverseaza primele doua noduri de pe nivel si pe ultimele doua si continua parcurgerea pana se ajunge la NULL.

##  `MIRROR_V:`
Functia inverseaza primul si ultimul nod de pe nivel si pe cele din mijloc si continua parcurgerea pana se ajunge la NULL.

##  `FREE_TREE:`
Aceasta functie elibereaza memoria alocata pentru arbore.


