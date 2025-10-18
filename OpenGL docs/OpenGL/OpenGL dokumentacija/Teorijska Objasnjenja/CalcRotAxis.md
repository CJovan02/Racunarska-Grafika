```c++
void CGLRenderer::CalcRotAxis(double x1, double y1, double z1, double x2, double y2, double z2, double& x3, double& y3, double& z3)
{
	// Analiticka geometrija :D
	// imamo vektor izmedju 1 i 2 (v12)
	// treba da nadjemo njegovu duzinu
	// i da ga normalizujemo (da ima duzinu 1)

	// v12 = (x2 - x1, y2 - y1, z2 - z1);
	// |v12| = sqrt(dx ^ 2 + dy ^ 2 + dz ^ 2)
	// normalizacija -> v12/|v12|

	// ovo su koordinate vektora v12
	double dx = x2 - x1;
	double dy = y2 - y1;
	double dz = z2 - z1;

	// ovo je njegov intenzitet/duzina
	double len = sqrt(dx * dx + dy * dy + dz * dz);

	// ovo je pretvaranje tog vektora u jedinicni vektor (normalizacija)
	x3 = dx / len;
	y3 = dy / len;
	z3 = dz / len;
}
```