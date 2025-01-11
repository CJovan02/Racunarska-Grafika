
OpenGL je jedna biblioteka koja se na ovom predmetu koristi u skladu sa MFC aplikacijom. Ona omogucava novu vrstu renderovanja, odnosno 3D renderovanja.

Kod openGL projekta osim defaultnih klasa koje MFC aplikacija generise postoje i **GL folder** gde se nalaze openGL funkcije koje se koristie i takodje posotji **GLRenderer fajl** koji sluzi kao bridge izmedju openGL-a i MFC.

U tom fajlu se nalaze funkcije za crtanje u OpenGL-u. Taj fajl izgleda dosta slicno kao View.cpp fajl kod MFC aplikacije, zadrzi funkcije za inicijalizaciju scene, crtanje scene, brisanje scene itd...
**Sve glavne funkcije iz GLRenderer.cpp fajla se pozivaju u View.cpp fajlu.**

## [[Kreiranje OpenGL projekta]]

## [[GLRenderer glavne funkcije]]

## [[Kamera, pogled i koordinatni sistem OpenGL-a]]

## [[Crtanje u OpenGL-u]]
## [[Crtanje geometrijskih tela]]

## [[Transformacije u OpenGL-u]]

## [[Osvetljenje u OpenGL-u]]