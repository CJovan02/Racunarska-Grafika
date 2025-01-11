
Zamenjuje front i back buffer kada se koristi **duplo baferovanje.**

Ovo je skroz isto kao koriscenje memorijskog dc-ja u GDI-u da bi se izbeglo flikerovanje.

Ovo zamenjuje buffer-e tako da se renderovana scena zapravo prikaze na ekranu, a samim tim se i eleminise fliker.

Moze da se vidi u view.cpp fajlu da funkcija *OnEraseBkgnd* vraca 1 (TRUE). 

## Double buffering

- **Back buffer** je gde se scena renderuje (off-screen).
- **Front buffer** je sta se trenutno renderuje na ekranu.