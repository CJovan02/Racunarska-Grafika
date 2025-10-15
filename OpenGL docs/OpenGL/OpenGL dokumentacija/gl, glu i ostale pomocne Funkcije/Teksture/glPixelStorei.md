Funkcija definiše način na koji je bitmapa (slika) smeštena u memoriji.

Ovo podešava **poravnanje bajtova (byte alignment)** kada se podaci slike šalju na GPU.

Šta to znači?

U memoriji, svaki red piksela (scanline) može da bude „poravnat“ na 1, 2, 4 ili 8 bajtova — zavisi od formata slike.  
OpenGL mora znati kako su redovi poravnati da bi pravilno čitao piksele.

U većini slučajeva (npr. kod 24-bitnih ili 32-bitnih slika) **poravnanje na 4 bajta** (`GL_UNPACK_ALIGNMENT = 4`) je standardno i bezbedno podešavanje.


Uglavnom, ova linija osigurava da OpenGL **tačno zna kako da pročita piksele iz slike** kad se slike kasnije salju na GPU.