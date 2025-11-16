# Izvještaj o Inspekciji Koda - RestaurantInventory

**Datum inspekcije:** 2024  
**Moderator inspekcije:** [Ime Moderatora]  
**Tim:** Tim 4

---

## 1. Pregled

Aplikacija **RestaurantInventory** je konzolna aplikacija za upravljanje inventarom restorana. Aplikacija omogućava dodavanje, pretraživanje, ažuriranje i uklanjanje stavki inventara, te generisanje izvještaja.

### Statistika

- **Ukupno linija koda:** ~600
- **Broj klasa:** 10
- **Broj servisa:** 4
- **Broj modela:** 6

---

## 2. Kategorizacija Problema

### 2.1. Kritični Problemi (Blocker/Critical) 🔴

#### **ISSUE-001: Potencijalna NullReferenceException u InventarService.DodajStavku**

**Lokacija:** `Services/InventarService.cs:22-24`  
**Ozbiljnost:** Kritična  
**Checklist:** Validacija inputa

```csharp
if (inventar.Stavke.Any(s => s.Naziv.Equals(stavka.Naziv, StringComparison.OrdinalIgnoreCase) &&
                             s.Dobavljac.Equals(stavka.Dobavljac, StringComparison.OrdinalIgnoreCase)))
```

**Problem:** Ako `stavka.Dobavljac` ili postojeća stavka u listi ima `null` vrijednost za `Dobavljac`, poziv `Equals` će baciti `NullReferenceException`.

**Rješenje:**

```csharp
if (inventar.Stavke.Any(s => s.Naziv.Equals(stavka.Naziv, StringComparison.OrdinalIgnoreCase) &&
                             (s.Dobavljac ?? "").Equals(stavka.Dobavljac ?? "", StringComparison.OrdinalIgnoreCase)))
```

---

#### **ISSUE-002: Nevalidiran unos cijene može baciti FormatException**

**Lokacija:** `Program.cs:134`  
**Ozbiljnost:** Kritična  
**Checklist:** Error handling

```csharp
double cijena = string.IsNullOrWhiteSpace(unos) ? 0 : double.Parse(unos, CultureInfo.InvariantCulture);
```

**Problem:** Ako korisnik unese nevalidan string (npr. "abc"), `double.Parse` će baciti `FormatException` koja nije uhvaćena.

**Rješenje:**

```csharp
double cijena = 0;
if (!string.IsNullOrWhiteSpace(unos))
{
    if (!double.TryParse(unos, NumberStyles.Float, CultureInfo.InvariantCulture, out cijena))
    {
        Console.WriteLine("Neispravan unos cijene! Koristit će se 0.");
        cijena = 0;
    }
}
```

---

#### **ISSUE-003: Logička greška u StavkaInventaraService.JeKriticna**

**Lokacija:** `Services/StavkaInventaraService.cs:8-24`  
**Ozbiljnost:** Kritična  
**Checklist:** Logika aplikacije

**Problem:** Metoda `JeKriticna` ima logičku grešku. Ako je `stavka.Kolicina < stavka.MinKolicina`, metoda uvijek vraća `true` bez obzira na ostale uslove (linija 18 je unreachable code).

**Trenutni kod:**

```csharp
if (stavka.Kolicina < stavka.MinKolicina)
{
    if (stavka.DatumNabavke < DateTime.Now.AddDays(-30))
        return true;

    if (stavka.Kategorija == Kategorija.Hrana || stavka.Kategorija == Kategorija.Pice)
        return true;

    return true; // Ova linija je uvijek dostupna
}
```

**Rješenje:** Refaktorisati logiku da bude jasnija i ispravna.

---

### 2.2. Visoki Problemi (High/Major) 🟠

#### **ISSUE-004: Duplikacija koda - FilterService i InventarService.Pretrazi**

**Lokacija:** `Services/FilterService.cs` i `Services/InventarService.cs:47-82`  
**Ozbiljnost:** Visoka  
**Checklist:** DRY princip, Održivost koda

**Problem:** Ista logika filtriranja je implementirana na dva mjesta. `InventarService.Pretrazi` i `FilterService.PrimijeniFilter` imaju identičnu implementaciju.

**Rješenje:** Koristiti `FilterService.PrimijeniFilter` unutar `InventarService.Pretrazi` umjesto duplikacije.

---

#### **ISSUE-005: Nedostaje validacija inputa na više mjesta**

**Lokacija:** `Program.cs` (više metoda)  
**Ozbiljnost:** Visoka  
**Checklist:** Validacija inputa, Robustnost

**Problemi:**

- `DodajStavku`: Nema provjere da li su `naziv` i `opis` prazni
- `UkloniStavku`: Nema provjere da li je `naziv` prazan
- `AzurirajStavku`: Nema provjere da li je `naziv` prazan
- `GenerisiIzvjestaj`: Nema provjere da li je `tip` prazan

**Rješenje:** Dodati validaciju na svim mjestima gdje se prima korisnički input.

---

#### **ISSUE-006: Nedostaje logging**

**Lokacija:** Cijela aplikacija  
**Ozbiljnost:** Visoka  
**Checklist:** Logging, Debugging, Monitoring

**Problem:** Aplikacija nema logging mehanizam. Nemoguće je pratiti šta se dešava u produkciji, debugovati probleme ili pratiti korisničke akcije.

**Rješenje:** Integrirati logging framework (npr. Serilog, NLog) i dodati logove za:

- Dodavanje/uklanjanje/ažuriranje stavki
- Greške i izuzetke
- Važne operacije

---

#### **ISSUE-007: Nedostaju unit testovi**

**Lokacija:** Cijeli projekt  
**Ozbiljnost:** Visoka  
**Checklist:** Testiranje, Kvalitet koda

**Problem:** Projekt ne sadrži unit testove. Nemoguće je automatski provjeriti ispravnost koda ili osigurati da promjene ne razbiju postojeću funkcionalnost.

**Rješenje:**

- Kreirati test projekt
- Dodati unit testove za sve servise
- Integrirati u CI/CD pipeline

---

#### **ISSUE-008: Nedostaje dokumentacija**

**Lokacija:** Cijeli projekt  
**Ozbiljnost:** Visoka  
**Checklist:** Dokumentacija

**Problemi:**

- Nema XML komentara na javnim metodama i klasama
- Nema README.md fajla
- Nema dokumentacije o arhitekturi

**Rješenje:**

- Dodati XML komentare na sve javne API-je
- Kreirati README.md sa uputstvima za instalaciju i korištenje
- Dokumentovati arhitekturu i dizajn odluke

---

#### **ISSUE-009: Nekonzistentno imenovanje (miješanje jezika)**

**Lokacija:** Cijeli projekt  
**Ozbiljnost:** Visoka  
**Checklist:** Konvencije imenovanja, Održivost

**Problem:** Kod miješa hrvatski i engleski jezik:

- Hrvatski: `DodajStavku`, `PrikazSaSortiranjem`, `ProvjeriMinimalne`
- Engleski: `InventarService`, `StavkaInventara`, `Filter`

**Rješenje:** Usvojiti jedan jezik (preporučeno engleski) za sve nazive u kodu, a hrvatski koristiti samo za UI poruke.

---

#### **ISSUE-010: Hardcoded vrijednosti (magic numbers)**

**Lokacija:** `Services/StavkaInventaraService.cs:12, 21`  
**Ozbiljnost:** Visoka  
**Checklist:** Konfiguracija, Održivost

**Problem:**

```csharp
if (stavka.DatumNabavke < DateTime.Now.AddDays(-30))  // Magic number 30
if ((DateTime.Now - stavka.DatumNabavke).TotalDays > 60)  // Magic number 60
```

**Rješenje:** Premjestiti u konfiguraciju ili konstante:

```csharp
private const int KRITICNI_PERIOD_DANA = 30;
private const int MAKSIMALNI_PERIOD_DANA = 60;
```

---

### 2.3. Srednji Problemi (Medium) 🟡

#### **ISSUE-011: Nedostaje dependency injection**

**Lokacija:** `Program.cs:15`  
**Ozbiljnost:** Srednja  
**Checklist:** Arhitektura, Testabilnost

**Problem:** Direktno instanciranje servisa u `Main` metodi otežava testiranje i zamjenu implementacija.

**Rješenje:** Integrirati DI container (npr. Microsoft.Extensions.DependencyInjection).

---

#### **ISSUE-012: Nema konfiguracijskog fajla**

**Lokacija:** Cijeli projekt  
**Ozbiljnost:** Srednja  
**Checklist:** Konfiguracija

**Problem:** Svi parametri su hardcoded u kodu. Nemoguće je mijenjati ponašanje aplikacije bez rekompajliranja.

**Rješenje:** Dodati `appsettings.json` i koristiti `IConfiguration`.

---

#### **ISSUE-013: Nedostaje error handling za edge cases**

**Lokacija:** `Program.cs:236`  
**Ozbiljnost:** Srednja  
**Checklist:** Error handling

**Problem:** U `AzurirajStavku`, ako pretraga vrati više stavki sa istim nazivom, uzima se samo prva (`FirstOrDefault`), što može biti neočekivano ponašanje.

**Rješenje:** Dodati provjeru i upozorenje ako postoji više stavki sa istim nazivom.

---

#### **ISSUE-014: Nekonzistentno korištenje null-checking**

**Lokacija:** Različite lokacije  
**Ozbiljnost:** Srednja  
**Checklist:** Null safety

**Problem:** Nekonzistentno korištenje `string.IsNullOrEmpty` vs `string.IsNullOrWhiteSpace`.

**Rješenje:** Standardizovati na `string.IsNullOrWhiteSpace` gdje je to prikladno.

---

#### **ISSUE-015: Nedostaje validacija datuma u GenerisiIzvjestaj**

**Lokacija:** `Program.cs:288-294`  
**Ozbiljnost:** Srednja  
**Checklist:** Validacija inputa

**Problem:** Nema provjere da li je početni datum prije krajnjeg datuma.

**Rješenje:** Dodati validaciju:

```csharp
if (izvjestaj.PocetakPerioda.HasValue && izvjestaj.KrajPerioda.HasValue &&
    izvjestaj.PocetakPerioda.Value > izvjestaj.KrajPerioda.Value)
{
    Console.WriteLine("Početni datum mora biti prije krajnjeg datuma!");
    return;
}
```

---

### 2.4. Niski Problemi (Low/Minor) 🟢

#### **ISSUE-016: Nekonzistentno formatiranje**

**Lokacija:** Različite lokacije  
**Ozbiljnost:** Niska  
**Checklist:** Stil koda

**Problem:** Nekonzistentno korištenje praznih linija i indentacije.

**Rješenje:** Konfigurisati `.editorconfig` i primijeniti auto-formatiranje.

---

#### **ISSUE-017: Nedostaju using direktive na nekim mjestima**

**Lokacija:** `Services/FilterService.cs:17`  
**Ozbiljnost:** Niska  
**Checklist:** Organizacija koda

**Problem:** Korištenje `System.StringComparison` umjesto samo `StringComparison` (iako je `using System;` prisutan).

**Rješenje:** Koristiti kratke nazive gdje je moguće.

---

#### **ISSUE-018: Nekorišćene klase/servisi**

**Lokacija:** `Services/FilterService.cs`  
**Ozbiljnost:** Niska  
**Checklist:** Cleanup koda

**Problem:** `FilterService` postoji ali se ne koristi nigdje u aplikaciji (duplikat sa `InventarService.Pretrazi`).

**Rješenje:** Ili koristiti `FilterService` ili ga ukloniti.

---

## 3. Dobre Prakse ✅

1. **Separation of Concerns:** Kod je dobro organizovan u modele i servise
2. **Enum korištenje:** Dobro korištenje enuma za `Kategorija` i sortiranje
3. **Korištenje LINQ:** Efektivno korištenje LINQ za filtriranje i pretraživanje
4. **Try-catch blokovi:** Postoje try-catch blokovi u `Main` metodi
5. **CultureInfo:** Korištenje `CultureInfo.InvariantCulture` za parsiranje brojeva
6. **StringComparison:** Korištenje `StringComparison.OrdinalIgnoreCase` za case-insensitive poređenje

---

## 4. Preporuke za Poboljšanje

### Kratkoročne (Sprint 1):

1. Popraviti kritične greške (ISSUE-001, ISSUE-002, ISSUE-003)
2. Dodati validaciju inputa na svim mjestima
3. Ukloniti duplikaciju koda (ISSUE-004)
4. Dodati osnovni logging

### Srednjoročne (Sprint 2-3):

1. Dodati unit testove
2. Integrirati dependency injection
3. Dodati konfiguracijski fajl
4. Standardizovati imenovanje

### Dugoročne (Sprint 4+):

1. Dodati kompletnu dokumentaciju
2. Implementirati CI/CD pipeline
3. Dodati integracijske testove
4. Razmotriti migraciju na web API ili desktop aplikaciju

---

## 5. Pareto Analiza

### 5.1. Analiza po Uzrocima

| Uzrok                                 | Broj Problema | %   |
| ------------------------------------- | ------------- | --- |
| Nedostaje validacija                  | 4             | 22% |
| Nedostaje dokumentacija/testiranje    | 3             | 17% |
| Duplikacija koda                      | 2             | 11% |
| Logičke greške                        | 2             | 11% |
| Nekonzistentnost                      | 3             | 17% |
| Nedostaje infrastruktura (DI, Config) | 2             | 11% |
| Stilski problemi                      | 2             | 11% |

**80/20 Pravilo:**

- **Top 3 uzroka (50%):** Nedostaje validacija, Nedostaje dokumentacija/testiranje, Nekonzistentnost
- **Rješenje:** Fokusirati se na validaciju, dodati testove i dokumentaciju, standardizovati kod

### 5.2. Analiza po Defektima

| Tip Defekta                   | Broj | Ozbiljnost | Prioritet |
| ----------------------------- | ---- | ---------- | --------- |
| NullReferenceException rizici | 2    | Kritična   | P1        |
| FormatException rizici        | 1    | Kritična   | P1        |
| Logičke greške                | 1    | Kritična   | P1        |
| Duplikacija koda              | 1    | Visoka     | P2        |
| Nedostaje validacija          | 4    | Visoka     | P2        |
| Nedostaje logging             | 1    | Visoka     | P2        |
| Nedostaju testovi             | 1    | Visoka     | P2        |
| Nedostaje dokumentacija       | 1    | Visoka     | P2        |
| Arhitektonski problemi        | 2    | Srednja    | P3        |
| Stilski problemi              | 3    | Niska      | P4        |

**Prioriteti:**

- **P1 (Kritično):** 4 problema - mora se riješiti odmah
- **P2 (Visoko):** 8 problema - riješiti u narednom sprintu
- **P3 (Srednje):** 2 problema - riješiti kada bude vremena
- **P4 (Nisko):** 3 problema - riješiti kao cleanup

---

## 6. Zaključak

Aplikacija ima dobru osnovnu strukturu i separation of concerns, ali ima nekoliko kritičnih problema koji moraju biti riješeni prije produkcije. Najveći problemi su:

1. **Kritične greške** koje mogu dovesti do crash-a aplikacije
2. **Nedostaje validacija** na više mjesta
3. **Nedostaju testovi i dokumentacija** što otežava održavanje

Preporučeno je riješiti sve P1 i P2 probleme prije puštanja u produkciju.

---

## 7. Checklist za Pull Request

- [ ] Svi P1 problemi su riješeni
- [ ] Dodana je validacija na svim mjestima gdje se prima input
- [ ] Uklonjena je duplikacija koda
- [ ] Dodani su osnovni unit testovi
- [ ] Dodana je osnovna dokumentacija (README.md)
- [ ] Kod je prošao code review
- [ ] Nema compiler warnings
- [ ] Kod je formatiran prema standardima

---

**Kreirao:** [Ime Inspektora]  
**Datum:** [Datum]  
**Verzija izvještaja:** 1.0
