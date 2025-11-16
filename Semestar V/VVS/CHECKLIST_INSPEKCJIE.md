# Checklist za Inspekciju Koda - RestaurantInventory

## 1. Validacija Inputa ✓/✗

- [ ] Svi korisnički inputi su validirani
- [ ] Provjera null vrijednosti na svim potrebnim mjestima
- [ ] Provjera praznih stringova gdje je potrebno
- [ ] Provjera formata (brojevi, datumi, email, itd.)
- [ ] Provjera raspona vrijednosti (min/max)
- [ ] Provjera duplikata gdje je potrebno
- [ ] Jasne poruke o greškama korisniku

**Pronađeni problemi:**

- ❌ Nedostaje validacija naziva i opisa u `DodajStavku`
- ❌ Nedostaje validacija naziva u `UkloniStavku` i `AzurirajStavku`
- ❌ `double.Parse` bez validacije u `DodajStavku` (linija 134)
- ❌ Provjera null za `Dobavljac` u `InventarService.DodajStavku`

---

## 2. Error Handling ✓/✗

- [ ] Svi izuzeci su uhvaćeni i obrađeni
- [ ] Try-catch blokovi su na odgovarajućim mjestima
- [ ] Korisnik dobija jasne poruke o greškama
- [ ] Greške se loguju (ako postoji logging)
- [ ] Nema neobrađenih izuzetaka koji mogu crash-ovati aplikaciju
- [ ] Graceful degradation gdje je moguće

**Pronađeni problemi:**

- ❌ `double.Parse` može baciti `FormatException` bez try-catch (linija 134)
- ❌ Generički catch blok u `Main` metodi ne daje dovoljno informacija
- ❌ Nema specifičnih error poruka za različite tipove grešaka

---

## 3. Null Safety ✓/✗

- [ ] Provjera null prije korištenja objekata
- [ ] Null-conditional operator (`?.`) gdje je prikladno
- [ ] Null-coalescing operator (`??`) gdje je prikladno
- [ ] Provjera null za reference tipove
- [ ] Nullable reference types su pravilno korišćeni

**Pronađeni problemi:**

- ❌ `NullReferenceException` rizik u `InventarService.DodajStavku` (linija 22-24)
- ❌ Nema provjere null za `stavka` u nekim metodama
- ❌ Nekonzistentno korištenje null-checking

---

## 4. Logika Aplikacije ✓/✗

- [ ] Logika je ispravna i testirana
- [ ] Nema unreachable code-a
- [ ] Nema dead code-a
- [ ] Logika je jasna i razumljiva
- [ ] Nema logičkih grešaka

**Pronađeni problemi:**

- ❌ Logička greška u `StavkaInventaraService.JeKriticna` (linija 8-24)
- ❌ Unreachable code u `JeKriticna` metodi (linija 18)

---

## 5. DRY Princip (Don't Repeat Yourself) ✓/✗

- [ ] Nema duplikacije koda
- [ ] Zajednička logika je izdvojena u metode
- [ ] Konstante su definisane jednom
- [ ] Helper metode se koriste gdje je potrebno

**Pronađeni problemi:**

- ❌ Duplikacija koda između `FilterService.PrimijeniFilter` i `InventarService.Pretrazi`
- ❌ Ista logika filtriranja na dva mjesta

---

## 6. Održivost Koda ✓/✗

- [ ] Kod je čitljiv i razumljiv
- [ ] Metode su kratke i fokusirane
- [ ] Nazivi su jasni i opisni
- [ ] Nema previše ugniježđenih struktura
- [ ] Kompleksnost je prihvatljiva

**Pronađeni problemi:**

- ⚠️ Nekonzistentno imenovanje (miješanje hrvatskog i engleskog)
- ⚠️ Nekorišćena klasa `FilterService`
- ⚠️ Nekonzistentno formatiranje

---

## 7. Konfiguracija ✓/✗

- [ ] Hardcoded vrijednosti su premještene u konfiguraciju
- [ ] Magic numbers su zamijenjeni konstantama
- [ ] Konfiguracija je čitljiva i organizovana
- [ ] Postoji mogućnost mijenjanja ponašanja bez rekompajliranja

**Pronađeni problemi:**

- ❌ Magic numbers u `StavkaInventaraService` (30 i 60 dana)
- ❌ Nema konfiguracijskog fajla
- ❌ Sve vrijednosti su hardcoded

---

## 8. Dokumentacija ✓/✗

- [ ] XML komentari na javnim metodama i klasama
- [ ] README.md fajl postoji
- [ ] Dokumentovana je arhitektura
- [ ] Dokumentovane su dizajn odluke
- [ ] Komentari u kodu gdje je potrebno

**Pronađeni problemi:**

- ❌ Nema XML komentara
- ❌ Nema README.md fajla
- ❌ Nema dokumentacije o arhitekturi

---

## 9. Testiranje ✓/✗

- [ ] Postoje unit testovi
- [ ] Postoje integracijski testovi
- [ ] Testovi pokrivaju kritične funkcionalnosti
- [ ] Testovi su automatski pokretani u CI/CD
- [ ] Code coverage je prihvatljiv (>70%)

**Pronađeni problemi:**

- ❌ Nema unit testova
- ❌ Nema test projekta
- ❌ Nema CI/CD integracije

---

## 10. Logging ✓/✗

- [ ] Postoji logging mehanizam
- [ ] Logovi se generišu za važne operacije
- [ ] Greške se loguju sa detaljnim informacijama
- [ ] Logovi su konfigurabilni
- [ ] Logovi su strukturirani

**Pronađeni problemi:**

- ❌ Nema logging mehanizma
- ❌ Nemoguće je pratiti šta se dešava u produkciji
- ❌ Nemoguće je debugovati probleme

---

## 11. Arhitektura ✓/✗

- [ ] Separation of Concerns je poštovan
- [ ] Dependency Injection je korišćen
- [ ] Servisi su pravilno organizovani
- [ ] Modeli su pravilno definisani
- [ ] Nema tight coupling-a

**Pronađeni problemi:**

- ⚠️ Nema Dependency Injection
- ⚠️ Direktno instanciranje u `Main` metodi
- ✅ Dobra organizacija u modele i servise

---

## 12. Konvencije Imenovanja ✓/✗

- [ ] Konzistentno korištenje jezika
- [ ] Nazivi su jasni i opisni
- [ ] Prati se C# naming conventions
- [ ] Nema skraćenica koje nisu standardne
- [ ] Konzistentno korištenje camelCase/PascalCase

**Pronađeni problemi:**

- ❌ Miješanje hrvatskog i engleskog jezika
- ⚠️ Nekonzistentno korištenje naziva

---

## 13. Stil Koda ✓/✗

- [ ] Konzistentno formatiranje
- [ ] Konzistentna indentacija
- [ ] Konzistentno korištenje praznih linija
- [ ] `.editorconfig` je konfigurisan
- [ ] Auto-formatiranje je omogućeno

**Pronađeni problemi:**

- ⚠️ Nekonzistentno formatiranje
- ⚠️ Nema `.editorconfig` fajla

---

## 14. Performance ✓/✗

- [ ] Nema nepotrebnih petlji
- [ ] Efektivno korištenje LINQ-a
- [ ] Nema memory leak-ova
- [ ] Efektivno korištenje resursa

**Dobre prakse:**

- ✅ Efektivno korištenje LINQ-a
- ✅ Nema očiglednih performance problema

---

## 15. Sigurnost ✓/✗

- [ ] Nema SQL injection rizika (ako se koristi baza)
- [ ] Input sanitization je implementiran
- [ ] Nema hardcoded credentials
- [ ] Sigurno rukovanje osjetljivim podacima

**Napomena:** Za konzolnu aplikaciju bez baze, sigurnosni rizici su minimalni, ali treba paziti na input validaciju.

---

## 16. Edge Cases ✓/✗

- [ ] Edge cases su razmotreni
- [ ] Prazni inputi su obrađeni
- [ ] Null vrijednosti su obrađene
- [ ] Granične vrijednosti su testirane
- [ ] Višestruki rezultati su obrađeni

**Pronađeni problemi:**

- ⚠️ Edge case: više stavki sa istim nazivom u `AzurirajStavku`
- ⚠️ Nema provjere da li je početni datum prije krajnjeg u `GenerisiIzvjestaj`

---

## 17. User Experience ✓/✗

- [ ] Jasne poruke korisniku
- [ ] Informativne poruke o greškama
- [ ] Konzistentan UI/UX
- [ ] Validacija sa jasnim porukama

**Pronađeni problemi:**

- ⚠️ Loša poruka u `UkloniStavku`: "Ako je stavka postojala, uklonjena je."
- ⚠️ Nema jasnih poruka o greškama validacije

---

## 18. Code Quality Tools ✓/✗

- [ ] Linter je konfigurisan
- [ ] Code analysis je omogućen
- [ ] Warnings su riješeni
- [ ] Code metrics su praćeni

**Preporuka:**

- Konfigurisati SonarQube ili sličan alat
- Omogućiti code analysis u build procesu

---

## Sažetak

### Ukupno provjera: 18 kategorija

### Rezultati:

- ✅ **Dobro:** 2 kategorije (Performance, Arhitektura - djelomično)
- ⚠️ **Treba poboljšanje:** 8 kategorija
- ❌ **Kritično:** 8 kategorija

### Prioriteti:

1. **Kritično (P0):** Validacija Inputa, Error Handling, Null Safety, Logika Aplikacije
2. **Visoko (P1):** DRY Princip, Dokumentacija, Testiranje, Logging
3. **Srednje (P2):** Konfiguracija, Arhitektura, Konvencije Imenovanja
4. **Nisko (P3):** Stil Koda, Edge Cases, User Experience

---

## Preporuke za Sljedeću Inspekciju

1. Fokusirati se na kritične probleme (P0)
2. Dodati unit testove prije sljedeće inspekcije
3. Implementirati logging mehanizam
4. Standardizovati imenovanje
5. Dodati dokumentaciju

---

**Kreirao:** [Ime Inspektora]  
**Datum:** [Datum]  
**Verzija:** 1.0
