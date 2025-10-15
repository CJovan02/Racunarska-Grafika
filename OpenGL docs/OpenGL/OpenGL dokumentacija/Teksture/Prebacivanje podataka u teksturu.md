
Radimo to preko [[glTexImage2D]] funkcije.

Podatke za teksturu mozemo da ucitamo preko AUX ili preko DImage.
Nisam siguran kako funkcionise AUX ali u uradjenim blanketima vidim da je korisnjen DImage.

Takodje mozemo rucno generisati [[Mipmap]]e korisnjenjem ove funkcije, doduse to nije potrebno jer postoji funkcija za automatsko generisanje mipmape -> [[gluBuild2DMipmaps]]