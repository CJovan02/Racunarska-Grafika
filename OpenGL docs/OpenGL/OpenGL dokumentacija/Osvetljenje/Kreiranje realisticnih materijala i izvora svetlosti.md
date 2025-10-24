## Projekat koriscen za ovaj primer je dat na github repozitorijumu

Ovo nije potrebno za ispit, samo malo dodatno objasnjenje osvetljenja i materijala.

## Realisticni materijali

Da bi materijal bio koliko toliko realistican on bi **trebalo da reflektuje svaku boju barem malo**.
U realnom svetu ako imamo plavi zid (ili bilo koji drugi objekat), odnosno objekat **reflektuje najvise plavi spektar**, i uperimo crveni laser ka zidu, deo zida koji laser obasjava ce biti i dalje crven.

Ovo znaci da taj objekat ipak reflektuje crvenu boju.

Ovo za nas znaci da kada kreiramo materijal, ambijentalno i difuziono svojstvo bi rebalo da reflektuje svaku boju.

Odnosno ako zelimo da objekat bude crvene boje, on bi reflektovao najvise crvenu boju i po malo zelenu i plavu.

Evo primera:
![[Pasted image 20250111183153.png]]

Imamo 4 materijala:
1. reflektuje najvise crvenu boju
2. reflektuje najvise zelenu boju
3. reflektuje najvise plavu boju
4. reflektuje sve boje podjednako
Ovo vazi i za ambijentalno i difuziono svojstvo.

## Sta ce se desiti kada promenimo boju svetla?

![[Pasted image 20250111183527.png]]

![[Pasted image 20250111183558.png]]

![[Pasted image 20250111183616.png]]

![[Pasted image 20250111184114.png]]

![[Pasted image 20250111184134.png]]

![[Pasted image 20250111184157.png]]

## Odsjaj i specular light

Bela refleksija koja se vidi na predmetima je **specular light** zajedno sa **shininess**.

Izvor bi trebalo da emituje neutralnu specular svetlost u vecini situacija, osim ako ne pravimo laser ili neon light.

Kada se radi o nemetalnim objektima oni uglavnom reflektuju sve boje spekularnom svetla.
Metal reflektuje spekularno svetlo isto kao i ambijentalno i difuziono.