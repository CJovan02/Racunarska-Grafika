
Ukoliko zelimo da koristimo neke transformacije koje ne zelimo da se primene ne celoj sceni koristimo [[Matrix Stack]].

## Kombinovanje transformacija

### Mnozenje matrice transformacija sa leve strane (GL default)

OpenGL transformacije mnozi sa leve strane, to znaci da transformacija koja se **zadnje napise se zapravo prva primenjuje.**
Ako koristimo **RAZMISLJANJE** da svaka transformacija objekta menja njegov lokalni koordinatni sistem, onda napisane transformacije mozemo da citamo redom, sasvim logicno razmisljanje i lako za shvatiti.

Sta to znaci: ako prvo rotiramo objekat, za 90 stepeni po X osi, to ce rotirati njegov lokalni koordinatni sistem (njegova Y osa ce sada biti u istom pravcu kao i globalna Z osa).
Nakon toga ga transliramo za 50 po Y osi, on ce se pomeriti za 50 na Y osi na njegovom **LOKALNOM** koordinatnom sistemu.
Odosno to ce nama izgledati kao da se pomerio za 50 na Z osi, gledajuci globalni koordinatni sistem.

![[Lokalna transf 1.png]]
![[Lokalna transf 2.png]]

Ako koristimo razmisljanje da se sve vrti i pomera oko globalnog koordinatnog sistema, onda kod moramo da citamo odozdo, znaci transformacija koja je zadnje napisana ce se prva primenitit, ali ce se transformisati na osnovu globalnog koordinatnog sistema.

![[Globalna transf 1.png]]

![[Globalna transf 2.png]]

Ovo su samo nacini razmisljanja, kao sto se vidi oni ne uticu na nacin pisanja koda i na konacno resenje.

Ovde vlajko to objasnjava, verovatno bolje od mene.
https://www.youtube.com/watch?v=nZvP9YveNyA&list=PLWLPHZCdUNsOh4telXXzBG_KMSikT_MTm&index=7

## Mnozenje matrice transformacija sa desne strane

OpenGl ne radi ovako ali u GDI aplikacijama moze da se bira nacin mnozenja matrice.

Uglavnom se cela nasa logika od malopre okrece napako sada.

Kada koristimo **GLOBALNO** razmisljanje, kod citamo redom.
Kada koristiomo **LOKALNO** rezmisljanje, kod citamo unazad.

Ja sam pojeo govna na I kol sam desno mnozenje koristio a vlajko je napravio kolokvijum da mora da se koristi levo mnozenje, ja nisam znao za to i opasno sam sjebao ceo kolikvijum :)

