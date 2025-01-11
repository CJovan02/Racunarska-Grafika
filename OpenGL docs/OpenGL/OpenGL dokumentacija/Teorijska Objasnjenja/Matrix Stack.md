
Svaka matrica transformacije (model view, projekciona, textura...) ima svoj stek.

U tom steku se cuvaju cele matrice transformacije.

## Cemu ovo sluzi?

Da kada zelimo neki individualni objekat transformisemo, da se njegove transformacije na primene na celu scenu.

```c++
glPushMatrix()
{
	// Transformacije
}
glPopMatrix()
```
*Nisu potrebne {} zagrade, one sluze zbog preglednosti samo.*

**glPushMatrix** gura trenutnu matricu na matrix stack. Nakon toga pisemo transformacije koje zelimo da primenimo na neki objekat, kada zavrimo pozovemo **glMatrixPop** koja uzme staru matricu sa stack-a i sve stare transformacije se vracaju nazad.