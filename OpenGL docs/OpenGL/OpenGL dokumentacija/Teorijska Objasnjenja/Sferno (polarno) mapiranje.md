polarno, sferno, ugaono, cilindricno mapiranje...

```c++
u = beta / 360;
v = alpha / 180;
```

ili:
```c++
u = atan2(z, x) / (2 * PI) + 0.5;
v = asin(y / R) / PI + 0.5;
```

Koristiš **uglove** da izračunaš gde tekstura treba da padne — drugim rečima, **mapiraš teksturu po uglovima**, ne po ravnima.

Zove se:

> **Cylindrical Mapping** – ako koristiš samo ugao oko ose i visinu  
> **Spherical Mapping** – ako koristiš i azimut (horizontalni ugao) i elevaciju (vertikalni ugao)

Vizuelno izgleda kao da uzmes teksturu i obmotas je CELU oko nekog tela, najvise ima smisla da se koristi za cilindar, tu se koristi samo *alpha / 360* za U koordinatu, dok je V 0 ili 1 ([[Lepljenje teksture na cilindar]])

![[Lepljnje teksture na omotac kupe]]

