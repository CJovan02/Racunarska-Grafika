Funkcija koja aktivira/selektuje teksturu.

**glBindTexture(texture_type, texture_id)**

Kada se prvi put pozove za dati identifikator, ona **kreira** objekat te teksture u memoriji graficke kartice i **selektuje** ga.

**Selektovanje** znaci da ce sve naredne funkcije, kao sto su [[glTexParameteri]], glTexImage2D... ce se odnositi bas na selektovanu teksturu.

Kada se ova funkcija pozove za vec kreirani objekat, onda ce se taj objekat **selektovati**

Kada se pozove za vrednost 0, isključuje rad sa objektima tekstura i aktivira neimenovanu „podrazumevanu“ teksturu.