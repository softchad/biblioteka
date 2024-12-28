# Bibliotekos Valdymo Sistema

## Projekto Aprašymas

Šis projektas yra bibliotekos valdymo sistema, kuri leidžia vartotojams:
- Peržiūrėti visas bibliotekos knygas.
- Filtruoti knygas pagal žanrą.
- Rezervuoti knygas ir peržiūrėti savo rezervacijas.
- Tvarkyti vartotojų paskyras (prisijungti, registruotis).

Projektas parašytas naudojant C++ ir įgyvendina visas pagrindines Objektinio Programavimo (OOP) savybes: inkapsuliaciją, paveldėjimą, abstrakciją ir polimorfizmą. Taip pat laikomasi SOLID principų, kurie užtikrina programos lankstumą ir plėtrą.

## Funkcionalumas

- **Vartotojo autentifikacija:**
  - Prisijungimas su vartotojo vardu ir slaptažodžiu.
  - Naujo vartotojo registracija.

- **Knygų valdymas:**
  - Visų knygų sąrašo peržiūra.
  - Filtravimas pagal žanrą.
  - Tik laisvų knygų peržiūra.

- **Rezervacijų valdymas:**
  - Knygos rezervavimas.
  - Rezervacijų istorijos peržiūra.
  - Rezervacijos atšaukimas.

- **Duomenų išsaugojimas:**
  - Vartotojų informacija išsaugoma faile `users.txt`.
  - Rezervacijos išsaugomos faile `reservations.txt`.
  - Knygų sąrašas įkeliamas iš failo `books.txt`.

## Naudojimo Instrukcija

### Failų Formatų Reikalavimai

- `books.txt`: Knygų sąrašas turi būti pateiktas šiuo formatu:
  ```
  Pavadinimas|Autorius|Metai|Žanras
  Dzuljeta ir Romeo|Viljamas Sekspyras|1597|Klasika
  Tomo Sojerio nuotykiai|Markas Tvenas|1876|Klasika
  ```

- `users.txt`: Vartotojų informacija saugoma šiuo formatu:
  ```
  vartotojas1 slaptažodis1
  vartotojas2 slaptažodis2
  ```

- `reservations.txt`: Rezervacijos saugomos tokiu formatu:
  ```
  VartotojoVardas|KnygosPavadinimas|Autorius|Žanras|Prieinamumas|RezervacijosData|AtsiimtiIkiData
  vardas|Dzuljeta ir Romeo|Viljamas Sekspyras|Klasika|0|2024-01-01 12:00:00|2024-01-11 12:00:00
  ```

### Programos Paleidimas

1. Užtikrinkite, kad turite kompiliatorių, kuris palaiko C++.
2. Atsisiųskite ir išsaugokite visus projekto failus.
3. Kompiliuokite programą naudodami šią komandą:
   ```
   g++ -o bibliotekos_valdymas main.cpp
   ```
4. Paleiskite programą:
   ```
   ./bibliotekos_valdymas
   ```

## OOP Savybės

- **Inkapsuliacija:** Kiekvienos klasės duomenys yra privatūs arba saugoti, prieinami tik per viešus metodus.
- **Paveldėjimas:** Klasė `Book` paveldi bendrą `LibraryItem` funkcionalumą. Galėtų būti praplėsta pridėjus klases `Magazine` arba `DVD`.
- **Abstrakcija:** `LibraryItem` yra abstrakti klasė, turinti gryną virtualų metodą `displayInfo()`.
- **Polimorfizmas:** Virtualūs metodai leidžia skirtingoms klasėms perrašyti bazinį funkcionalumą.

## SOLID Principai

- **S (Single Responsibility):** Kiekviena klasė turi vieną atsakomybę.
- **O (Open/Closed):** Nauji funkcionalumai gali būti pridėti nekeičiant esamo kodo.
- **L (Liskov Substitution):** `LibraryItem*` gali būti pakeista į bet kurią jos paveldėtą klasę.
- **I (Interface Segregation):** Šis principas nėra aktualus mažoms sistemoms kaip ši.
- **D (Dependency Inversion):** Sistema naudoja abstrakcijas (`LibraryItem`) vietoje konkrečių įgyvendinimų.

## Autorius
- **Vardas:** Vytautas Petronis
- **Grupė:** PI23SN

## Pastabos
- Jei failai `books.txt` ar `users.txt` neegzistuoja, programa automatiškai juos sukurs.
- Programą galima pritaikyti pridėti daugiau bibliotekos elementų, tokių kaip žurnalai ar DVD.
