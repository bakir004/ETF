# Issue Kartice za Azure DevOps - RestaurantInventory

## 🔴 Kritični Issues (Blocker)

---

### ISSUE-001: NullReferenceException u InventarService.DodajStavku

**Tip:** Bug  
**Ozbiljnost:** 🔴 Critical  
**Prioritet:** P0 - Blocker  
**Checklist:** Validacija Inputa, Null Safety

**Opis:**
U metodi `InventarService.DodajStavku` (linija 22-24), provjera da li stavka već postoji može baciti `NullReferenceException` ako `stavka.Dobavljac` ili postojeća stavka u listi ima `null` vrijednost.

**Lokacija:**
`Services/InventarService.cs:22-24`

**Kod:**

```csharp
if (inventar.Stavke.Any(s => s.Naziv.Equals(stavka.Naziv, StringComparison.OrdinalIgnoreCase) &&
                             s.Dobavljac.Equals(stavka.Dobavljac, StringComparison.OrdinalIgnoreCase)))
```

**Očekivano ponašanje:**
Aplikacija ne bi trebala crash-ovati kada se pokuša dodati stavka sa null dobavljačem ili kada postojeća stavka ima null dobavljača.

**Stvarno ponašanje:**
Aplikacija baca `NullReferenceException` i crash-uje.

**Koraci za reprodukciju:**

1. Dodati stavku sa praznim dobavljačem
2. Pokušati dodati drugu stavku sa istim nazivom i praznim dobavljačem

**Rješenje:**

```csharp
if (inventar.Stavke.Any(s =>
    string.Equals(s.Naziv, stavka.Naziv, StringComparison.OrdinalIgnoreCase) &&
    string.Equals(s.Dobavljac ?? "", stavka.Dobavljac ?? "", StringComparison.OrdinalIgnoreCase)))
```

**Acceptance Criteria:**

- [ ] Aplikacija ne crash-uje kada se dodaje stavka sa null dobavljačem
- [ ] Provjera duplikata radi ispravno sa null vrijednostima
- [ ] Dodati unit test koji testira ovaj scenario

---

### ISSUE-002: FormatException pri unosu nevalidne cijene

**Tip:** Bug  
**Ozbiljnost:** 🔴 Critical  
**Prioritet:** P0 - Blocker  
**Checklist:** Error Handling, Validacija Inputa

**Opis:**
U metodi `Program.DodajStavku` (linija 134), `double.Parse` se poziva bez try-catch bloka. Ako korisnik unese nevalidan string (npr. "abc"), aplikacija će crash-ovati sa `FormatException`.

**Lokacija:**
`Program.cs:134`

**Kod:**

```csharp
double cijena = string.IsNullOrWhiteSpace(unos) ? 0 : double.Parse(unos, CultureInfo.InvariantCulture);
```

**Očekivano ponašanje:**
Aplikacija bi trebala prihvatiti unos, validirati ga, i ako je nevalidan, prikazati poruku i koristiti default vrijednost (0).

**Stvarno ponašanje:**
Aplikacija baca `FormatException` i crash-uje.

**Koraci za reprodukciju:**

1. Pokrenuti aplikaciju
2. Odabrati opciju "1. Dodaj stavku"
3. Unijeti nevalidan string za cijenu (npr. "abc")

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

**Acceptance Criteria:**

- [ ] Aplikacija ne crash-uje kada se unese nevalidna cijena
- [ ] Prikazuje se poruka korisniku o nevalidnom unosu
- [ ] Koristi se default vrijednost (0) za nevalidan unos
- [ ] Dodati unit test koji testira ovaj scenario

---

### ISSUE-003: Logička greška u StavkaInventaraService.JeKriticna

**Tip:** Bug  
**Ozbiljnost:** 🔴 Critical  
**Prioritet:** P0 - Blocker  
**Checklist:** Logika Aplikacije, Code Quality

**Opis:**
Metoda `StavkaInventaraService.JeKriticna` ima logičku grešku. Ako je `stavka.Kolicina < stavka.MinKolicina`, metoda uvijek vraća `true` bez obzira na ostale uslove. Linija 18 (`return true;`) je unreachable code jer se uvijek izvršava.

**Lokacija:**
`Services/StavkaInventaraService.cs:8-24`

**Kod:**

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

**Očekivano ponašanje:**
Metoda bi trebala imati jasnu logiku za određivanje da li je stavka kritična, bez unreachable code-a.

**Stvarno ponašanje:**
Metoda uvijek vraća `true` ako je količina ispod minimuma, bez obzira na ostale uslove.

**Koraci za reprodukciju:**

1. Kreirati stavku sa količinom ispod minimuma
2. Pozvati `JeKriticna` metodu
3. Metoda uvijek vraća `true`

**Rješenje:**
Refaktorisati logiku da bude jasnija i ispravna. Vidi detalje u komentaru 8.

**Acceptance Criteria:**

- [ ] Logika metode je ispravna i jasna
- [ ] Nema unreachable code-a
- [ ] Dodati unit testovi koji testiraju sve scenarije
- [ ] Dokumentovati logiku metode

---

## 🟠 Visoki Issues (Major)

---

### ISSUE-004: Duplikacija koda - FilterService i InventarService.Pretrazi

**Tip:** Code Smell  
**Ozbiljnost:** 🟠 High  
**Prioritet:** P1 - High  
**Checklist:** DRY Princip, Održivost Koda

**Opis:**
Ista logika filtriranja je implementirana na dva mjesta: `InventarService.Pretrazi` i `FilterService.PrimijeniFilter`. Ovo krši DRY (Don't Repeat Yourself) princip i otežava održavanje.

**Lokacija:**
`Services/FilterService.cs` i `Services/InventarService.cs:47-82`

**Problem:**

- Duplikacija koda otežava održavanje
- Promjene u logici filtriranja moraju biti napravljene na dva mjesta
- Povećava rizik od grešaka

**Rješenje:**
Refaktorisati `InventarService.Pretrazi` da koristi `FilterService.PrimijeniFilter`:

```csharp
public List<StavkaInventara> Pretrazi(Filter filter)
{
    if (filter == null) throw new ArgumentNullException(nameof(filter));
    return FilterService.PrimijeniFilter(inventar.Stavke, filter);
}
```

**Acceptance Criteria:**

- [ ] Uklonjena je duplikacija koda
- [ ] `InventarService.Pretrazi` koristi `FilterService.PrimijeniFilter`
- [ ] Svi postojeći testovi prolaze
- [ ] Nema promjene u funkcionalnosti

---

### ISSUE-005: Nedostaje validacija inputa na više mjesta

**Tip:** Enhancement  
**Ozbiljnost:** 🟠 High  
**Prioritet:** P1 - High  
**Checklist:** Validacija Inputa, Robustnost

**Opis:**
Nedostaje validacija korisničkog inputa na više mjesta u aplikaciji. Ovo može dovesti do neočekivanog ponašanja ili grešaka.

**Lokacije:**

- `Program.DodajStavku`: Nema provjere da li su `naziv` i `opis` prazni
- `Program.UkloniStavku`: Nema provjere da li je `naziv` prazan
- `Program.AzurirajStavku`: Nema provjere da li je `naziv` prazan
- `Program.GenerisiIzvjestaj`: Nema provjere da li je `tip` prazan

**Problem:**

- Korisnik može unijeti prazne vrijednosti koje mogu dovesti do problema
- Loša korisnička iskustva
- Potencijalni bugovi

**Rješenje:**
Dodati validaciju na svim mjestima gdje se prima korisnički input. Koristiti do-while petlje ili provjere sa return statement-ima.

**Acceptance Criteria:**

- [ ] Svi inputi su validirani
- [ ] Korisnik dobija jasne poruke o greškama
- [ ] Aplikacija ne dozvoljava unos praznih vrijednosti gdje je to potrebno
- [ ] Dodati unit testovi za validaciju

---

### ISSUE-006: Nedostaje logging

**Tip:** Enhancement  
**Ozbiljnost:** 🟠 High  
**Prioritet:** P1 - High  
**Checklist:** Logging, Debugging, Monitoring

**Opis:**
Aplikacija nema logging mehanizam. Nemoguće je pratiti šta se dešava u produkciji, debugovati probleme ili pratiti korisničke akcije.

**Problem:**

- Nemoguće je debugovati probleme u produkciji
- Nemoguće je pratiti korisničke akcije
- Nemoguće je analizirati performanse

**Rješenje:**
Integrirati logging framework (npr. Serilog, NLog) i dodati logove za:

- Dodavanje/uklanjanje/ažuriranje stavki
- Greške i izuzetke
- Važne operacije
- Korisničke akcije

**Acceptance Criteria:**

- [ ] Logging framework je integriran
- [ ] Logovi se generišu za sve važne operacije
- [ ] Greške se loguju sa detaljnim informacijama
- [ ] Logovi su konfigurabilni (nivo, output, format)

---

### ISSUE-007: Nedostaju unit testovi

**Tip:** Enhancement  
**Ozbiljnost:** 🟠 High  
**Prioritet:** P1 - High  
**Checklist:** Testiranje, Kvalitet Koda

**Opis:**
Projekt ne sadrži unit testove. Nemoguće je automatski provjeriti ispravnost koda ili osigurati da promjene ne razbiju postojeću funkcionalnost.

**Problem:**

- Nemoguće je automatski provjeriti ispravnost koda
- Rizik od regresije pri promjenama
- Nemoguće je mjeriti code coverage

**Rješenje:**

- Kreirati test projekt (npr. `RestaurantInventory.Tests`)
- Dodati unit testove za sve servise
- Integrirati u CI/CD pipeline
- Postaviti cilj za code coverage (npr. 80%)

**Acceptance Criteria:**

- [ ] Test projekt je kreiran
- [ ] Unit testovi postoje za sve servise
- [ ] Testovi su integrirani u CI/CD pipeline
- [ ] Code coverage je najmanje 70%

---

### ISSUE-008: Nedostaje dokumentacija

**Tip:** Enhancement  
**Ozbiljnost:** 🟠 High  
**Prioritet:** P1 - High  
**Checklist:** Dokumentacija

**Opis:**
Projektu nedostaje dokumentacija:

- Nema XML komentara na javnim metodama i klasama
- Nema README.md fajla
- Nema dokumentacije o arhitekturi

**Problem:**

- Otežava razumijevanje koda novim developerima
- Otežava održavanje
- Otežava korištenje API-ja

**Rješenje:**

- Dodati XML komentare na sve javne API-je
- Kreirati README.md sa uputstvima za instalaciju i korištenje
- Dokumentovati arhitekturu i dizajn odluke

**Acceptance Criteria:**

- [ ] Svi javni API-ji imaju XML komentare
- [ ] README.md postoji sa uputstvima
- [ ] Dokumentovana je arhitektura aplikacije
- [ ] Dokumentacija je ažurna

---

### ISSUE-009: Nekonzistentno imenovanje (miješanje jezika)

**Tip:** Code Smell  
**Ozbiljnost:** 🟠 High  
**Prioritet:** P2 - Medium  
**Checklist:** Konvencije Imenovanja, Održivost

**Opis:**
Kod miješa hrvatski i engleski jezik u nazivima metoda i klasa:

- Hrvatski: `DodajStavku`, `PrikazSaSortiranjem`, `ProvjeriMinimalne`
- Engleski: `InventarService`, `StavkaInventara`, `Filter`

**Problem:**

- Otežava čitljivost koda
- Nekonzistentnost otežava održavanje
- Ne prati se industry standard (engleski jezik)

**Rješenje:**
Usvojiti jedan jezik (preporučeno engleski) za sve nazive u kodu, a hrvatski koristiti samo za UI poruke.

**Acceptance Criteria:**

- [ ] Svi nazivi u kodu su na engleskom jeziku
- [ ] UI poruke mogu biti na hrvatskom
- [ ] Nema miješanja jezika u kodu
- [ ] Refaktorisani su svi nazivi

---

### ISSUE-010: Hardcoded vrijednosti (magic numbers)

**Tip:** Code Smell  
**Ozbiljnost:** 🟠 High  
**Prioritet:** P2 - Medium  
**Checklist:** Konfiguracija, Održavnost

**Opis:**
U kodu postoje hardcoded vrijednosti (magic numbers) koje bi trebale biti u konfiguraciji ili konstantama:

- `Services/StavkaInventaraService.cs:12`: `-30` dana
- `Services/StavkaInventaraService.cs:21`: `60` dana

**Problem:**

- Otežava održavanje
- Nemoguće je mijenjati bez rekompajliranja
- Nije jasno šta te vrijednosti predstavljaju

**Rješenje:**
Premjestiti u konstante ili konfiguraciju:

```csharp
private const int KRITICNI_PERIOD_DANA = 30;
private const int MAKSIMALNI_PERIOD_DANA = 60;
```

**Acceptance Criteria:**

- [ ] Svi magic numbers su zamijenjeni konstantama
- [ ] Konstante imaju jasne nazive
- [ ] Razmotriti premještanje u konfiguraciju

---

## 🟡 Srednji Issues (Medium)

---

### ISSUE-011: Nedostaje dependency injection

**Tip:** Enhancement  
**Ozbiljnost:** 🟡 Medium  
**Prioritet:** P2 - Medium  
**Checklist:** Arhitektura, Testabilnost

**Opis:**
Direktno instanciranje servisa u `Main` metodi otežava testiranje i zamjenu implementacija.

**Lokacija:**
`Program.cs:15`

**Rješenje:**
Integrirati DI container (npr. Microsoft.Extensions.DependencyInjection).

**Acceptance Criteria:**

- [ ] DI container je integriran
- [ ] Servisi su registrovani u DI containeru
- [ ] Testiranje je olakšano

---

### ISSUE-012: Nema konfiguracijskog fajla

**Tip:** Enhancement  
**Ozbiljnost:** 🟡 Medium  
**Prioritet:** P2 - Medium  
**Checklist:** Konfiguracija

**Opis:**
Svi parametri su hardcoded u kodu. Nemoguće je mijenjati ponašanje aplikacije bez rekompajliranja.

**Rješenje:**
Dodati `appsettings.json` i koristiti `IConfiguration`.

**Acceptance Criteria:**

- [ ] `appsettings.json` je kreiran
- [ ] Konfiguracija se učitava iz fajla
- [ ] Hardcoded vrijednosti su premještene u konfiguraciju

---

### ISSUE-013: Edge case - više stavki sa istim nazivom

**Tip:** Enhancement  
**Ozbiljnost:** 🟡 Medium  
**Prioritet:** P3 - Low  
**Checklist:** Edge Cases, User Experience

**Opis:**
U `Program.AzurirajStavku`, ako pretraga vrati više stavki sa istim nazivom, uzima se samo prva (`FirstOrDefault`), što može biti neočekivano ponašanje.

**Lokacija:**
`Program.cs:236`

**Rješenje:**
Dodati provjeru i upozorenje ako postoji više stavki sa istim nazivom.

**Acceptance Criteria:**

- [ ] Korisnik je upozoren ako postoji više stavki sa istim nazivom
- [ ] Jasno je koja stavka će biti ažurirana

---

## 🟢 Niski Issues (Minor)

---

### ISSUE-014: Nekonzistentno formatiranje

**Tip:** Code Style  
**Ozbiljnost:** 🟢 Low  
**Prioritet:** P4 - Very Low  
**Checklist:** Stil Koda

**Opis:**
Nekonzistentno korištenje praznih linija i indentacije kroz kod.

**Rješenje:**
Konfigurisati `.editorconfig` i primijeniti auto-formatiranje.

**Acceptance Criteria:**

- [ ] `.editorconfig` je kreiran
- [ ] Kod je formatiran prema standardima
- [ ] Auto-formatiranje je konfigurisano

---

### ISSUE-015: Nekorišćena klasa FilterService

**Tip:** Cleanup  
**Ozbiljnost:** 🟢 Low  
**Prioritet:** P4 - Very Low  
**Checklist:** Cleanup Koda

**Opis:**
`FilterService` postoji ali se ne koristi nigdje u aplikaciji (duplikat sa `InventarService.Pretrazi`).

**Rješenje:**
Ili koristiti `FilterService` ili ga ukloniti.

**Acceptance Criteria:**

- [ ] `FilterService` se koristi ili je uklonjen
- [ ] Nema nekoristećeg koda

---

## Sažetak

- **Kritični (P0):** 3 issues
- **Visoki (P1-P2):** 7 issues
- **Srednji (P3):** 3 issues
- **Niski (P4):** 2 issues
- **Ukupno:** 15 issues

**Preporuka:** Riješiti sve P0 i P1 issues prije puštanja u produkciju.
