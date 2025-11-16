# Komentari za Pull Request - RestaurantInventory

## Komentari po Fajlovima

---

### `Program.cs`

#### **Komentar 1: Linija 134**

**Ozbiljnost:** 🔴 **Kritična**  
**Checklist:** Error Handling

```csharp
double cijena = string.IsNullOrWhiteSpace(unos) ? 0 : double.Parse(unos, CultureInfo.InvariantCulture);
```

**Problem:** `double.Parse` može baciti `FormatException` ako korisnik unese nevalidan string. Ova greška nije uhvaćena i može dovesti do crash-a aplikacije.

**Preporuka:**

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

#### **Komentar 2: Linija 83-87**

**Ozbiljnost:** 🟠 **Visoka**  
**Checklist:** Validacija Inputa

```csharp
Console.Write("Naziv: ");
string naziv = Console.ReadLine();

Console.Write("Opis: ");
string opis = Console.ReadLine();
```

**Problem:** Nema validacije da li su `naziv` i `opis` prazni. Ako korisnik unese prazan string, stavka će biti kreirana sa praznim nazivom, što može dovesti do problema.

**Preporuka:** Dodati validaciju:

```csharp
string naziv;
do
{
    Console.Write("Naziv: ");
    naziv = Console.ReadLine();
    if (string.IsNullOrWhiteSpace(naziv))
        Console.WriteLine("Naziv ne može biti prazan!");
} while (string.IsNullOrWhiteSpace(naziv));
```

---

#### **Komentar 3: Linija 221-224**

**Ozbiljnost:** 🟠 **Visoka**  
**Checklist:** Validacija Inputa, User Experience

```csharp
Console.Write("Unesi naziv stavke za uklanjanje: ");
string naziv = Console.ReadLine();
inventarService.UkloniStavku(naziv);
Console.WriteLine("Ako je stavka postojala, uklonjena je.");
```

**Problem:**

1. Nema provjere da li je `naziv` prazan
2. Poruka "Ako je stavka postojala, uklonjena je." je loša UX - korisnik ne zna da li je stavka stvarno uklonjena

**Preporuka:**

```csharp
Console.Write("Unesi naziv stavke za uklanjanje: ");
string naziv = Console.ReadLine();
if (string.IsNullOrWhiteSpace(naziv))
{
    Console.WriteLine("Naziv ne može biti prazan!");
    return;
}

var stavka = inventarService.Pretrazi(new Filter(naziv: naziv)).FirstOrDefault();
if (stavka == null)
{
    Console.WriteLine($"Stavka '{naziv}' nije pronađena.");
    return;
}

inventarService.UkloniStavku(naziv);
Console.WriteLine($"Stavka '{naziv}' je uspješno uklonjena.");
```

---

#### **Komentar 4: Linija 236**

**Ozbiljnost:** 🟡 **Srednja**  
**Checklist:** Edge Cases

```csharp
var stavka = inventarService.Pretrazi(new Filter(naziv: naziv)).FirstOrDefault();
```

**Problem:** Ako postoji više stavki sa istim nazivom, uzima se samo prva. To može biti neočekivano ponašanje.

**Preporuka:** Dodati provjeru i upozorenje:

```csharp
var stavke = inventarService.Pretrazi(new Filter(naziv: naziv)).ToList();
if (stavke.Count == 0)
{
    Console.WriteLine($"Stavka '{naziv}' nije pronađena.");
    return;
}
if (stavke.Count > 1)
{
    Console.WriteLine($"Upozorenje: Pronađeno je {stavke.Count} stavki sa nazivom '{naziv}'. Ažuriraće se prva.");
}
var stavka = stavke.First();
```

---

#### **Komentar 5: Linija 288-294**

**Ozbiljnost:** 🟡 **Srednja**  
**Checklist:** Validacija Inputa

```csharp
Console.Write("Unesi početni datum (yyyy-MM-dd) ili prazno: ");
if (DateTime.TryParse(Console.ReadLine(), out DateTime dtPocetak))
    izvjestaj.PocetakPerioda = dtPocetak;

Console.Write("Unesi krajnji datum (yyyy-MM-dd) ili prazno: ");
if (DateTime.TryParse(Console.ReadLine(), out DateTime dtKraj))
    izvjestaj.KrajPerioda = dtKraj;
```

**Problem:** Nema provjere da li je početni datum prije krajnjeg datuma.

**Preporuka:**

```csharp
if (izvjestaj.PocetakPerioda.HasValue && izvjestaj.KrajPerioda.HasValue &&
    izvjestaj.PocetakPerioda.Value > izvjestaj.KrajPerioda.Value)
{
    Console.WriteLine("Greška: Početni datum mora biti prije krajnjeg datuma!");
    return;
}
```

---

### `Services/InventarService.cs`

#### **Komentar 6: Linija 22-24**

**Ozbiljnost:** 🔴 **Kritična**  
**Checklist:** Null Safety, Validacija Inputa

```csharp
if (inventar.Stavke.Any(s => s.Naziv.Equals(stavka.Naziv, StringComparison.OrdinalIgnoreCase) &&
                             s.Dobavljac.Equals(stavka.Dobavljac, StringComparison.OrdinalIgnoreCase)))
```

**Problem:** Ako `stavka.Dobavljac` ili postojeća stavka u listi ima `null` vrijednost za `Dobavljac`, poziv `Equals` će baciti `NullReferenceException`.

**Preporuka:**

```csharp
if (inventar.Stavke.Any(s => s.Naziv.Equals(stavka.Naziv, StringComparison.OrdinalIgnoreCase) &&
                             (s.Dobavljac ?? "").Equals(stavka.Dobavljac ?? "", StringComparison.OrdinalIgnoreCase)))
```

Ili još bolje:

```csharp
if (inventar.Stavke.Any(s =>
    string.Equals(s.Naziv, stavka.Naziv, StringComparison.OrdinalIgnoreCase) &&
    string.Equals(s.Dobavljac ?? "", stavka.Dobavljac ?? "", StringComparison.OrdinalIgnoreCase)))
```

---

#### **Komentar 7: Linija 47-82**

**Ozbiljnost:** 🟠 **Visoka**  
**Checklist:** DRY Princip, Održivost Koda

**Problem:** Metoda `Pretrazi` ima identičnu logiku kao `FilterService.PrimijeniFilter`. Ovo je duplikacija koda koja otežava održavanje.

**Preporuka:** Refaktorisati da koristi `FilterService`:

```csharp
public List<StavkaInventara> Pretrazi(Filter filter)
{
    if (filter == null) throw new ArgumentNullException(nameof(filter));
    return FilterService.PrimijeniFilter(inventar.Stavke, filter);
}
```

---

### `Services/StavkaInventaraService.cs`

#### **Komentar 8: Linija 8-24**

**Ozbiljnost:** 🔴 **Kritična**  
**Checklist:** Logika Aplikacije, Code Quality

**Problem:** Metoda `JeKriticna` ima logičku grešku. Ako je `stavka.Kolicina < stavka.MinKolicina`, metoda uvijek vraća `true` bez obzira na ostale uslove. Linija 18 (`return true;`) je unreachable code jer se uvijek izvršava.

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

**Preporuka:** Refaktorisati logiku:

```csharp
public static bool JeKriticna(StavkaInventara stavka)
{
    if (stavka == null) return false;

    // Stavka je kritična ako je količina ispod minimuma
    if (stavka.Kolicina < stavka.MinKolicina)
    {
        // Posebno kritično za hranu i piće
        if (stavka.Kategorija == Kategorija.Hrana || stavka.Kategorija == Kategorija.Pice)
            return true;

        // Kritično ako je stara više od 30 dana
        if (stavka.DatumNabavke < DateTime.Now.AddDays(-30))
            return true;

        // Inače je kritična jer je ispod minimuma
        return true;
    }

    // Stavka je kritična ako je stara više od 60 dana
    if ((DateTime.Now - stavka.DatumNabavke).TotalDays > 60)
        return true;

    return false;
}
```

Ili još jasnije:

```csharp
public static bool JeKriticna(StavkaInventara stavka)
{
    if (stavka == null) return false;

    const int KRITICNI_PERIOD_DANA = 30;
    const int MAKSIMALNI_PERIOD_DANA = 60;

    // Kritično ako je količina ispod minimuma
    bool ispodMinimuma = stavka.Kolicina < stavka.MinKolicina;

    // Posebno kritično za hranu i piće
    bool hranaIliPice = stavka.Kategorija == Kategorija.Hrana ||
                        stavka.Kategorija == Kategorija.Pice;

    // Kritično ako je stara
    bool stara = (DateTime.Now - stavka.DatumNabavke).TotalDays > MAKSIMALNI_PERIOD_DANA;
    bool staraIKriticna = stavka.DatumNabavke < DateTime.Now.AddDays(-KRITICNI_PERIOD_DANA);

    return ispodMinimuma && (hranaIliPice || staraIKriticna) || stara;
}
```

---

#### **Komentar 9: Linija 12, 21**

**Ozbiljnost:** 🟠 **Visoka**  
**Checklist:** Konfiguracija, Magic Numbers

```csharp
if (stavka.DatumNabavke < DateTime.Now.AddDays(-30))  // Magic number
if ((DateTime.Now - stavka.DatumNabavke).TotalDays > 60)  // Magic number
```

**Problem:** Hardcoded vrijednosti 30 i 60 dana su "magic numbers" koje otežavaju održavanje.

**Preporuka:** Premjestiti u konstante ili konfiguraciju:

```csharp
private const int KRITICNI_PERIOD_DANA = 30;
private const int MAKSIMALNI_PERIOD_DANA = 60;
```

---

### `Services/FilterService.cs`

#### **Komentar 10: Cijeli fajl**

**Ozbiljnost:** 🟡 **Srednja**  
**Checklist:** Cleanup Koda, Održivost

**Problem:** `FilterService` postoji ali se ne koristi nigdje u aplikaciji. Ista logika je implementirana u `InventarService.Pretrazi`.

**Preporuka:**

- Ili koristiti `FilterService` u `InventarService.Pretrazi` (preporučeno)
- Ili ukloniti `FilterService` ako se ne planira koristiti

---

### `Models/StavkaInventara.cs`

#### **Komentar 11: Linija 7-14**

**Ozbiljnost:** 🟡 **Srednja**  
**Checklist:** Null Safety, Validacija

**Problem:** Svi properties su nullable ili nemaju default vrijednosti, što može dovesti do problema sa null referencama.

**Preporuka:** Razmotriti dodavanje validacije u konstruktoru ili korištenje nullable reference types sa odgovarajućim anotacijama.

---

### Opšti Komentari

#### **Komentar 12: Cijeli projekt**

**Ozbiljnost:** 🟠 **Visoka**  
**Checklist:** Dokumentacija

**Problem:** Nema XML komentara na javnim metodama i klasama. Ovo otežava razumijevanje i korištenje API-ja.

**Preporuka:** Dodati XML komentare:

```csharp
/// <summary>
/// Dodaje novu stavku u inventar.
/// </summary>
/// <param name="stavka">Stavka inventara koja se dodaje.</param>
/// <exception cref="ArgumentException">Baca se ako je stavka null ili ima prazan naziv.</exception>
/// <exception cref="Exception">Baca se ako stavka već postoji.</exception>
public void DodajStavku(StavkaInventara stavka)
```

---

#### **Komentar 13: Cijeli projekt**

**Ozbiljnost:** 🟠 **Visoka**  
**Checklist:** Logging

**Problem:** Aplikacija nema logging mehanizam. Nemoguće je pratiti šta se dešava u produkciji ili debugovati probleme.

**Preporuka:** Integrirati logging framework (npr. Serilog, NLog) i dodati logove za važne operacije i greške.

---

#### **Komentar 14: Cijeli projekt**

**Ozbiljnost:** 🟠 **Visoka**  
**Checklist:** Testiranje

**Problem:** Projekt ne sadrži unit testove. Nemoguće je automatski provjeriti ispravnost koda.

**Preporuka:** Kreirati test projekt i dodati unit testove za sve servise.

---

#### **Komentar 15: Cijeli projekt**

**Ozbiljnost:** 🟡 **Srednja**  
**Checklist:** Konvencije Imenovanja

**Problem:** Kod miješa hrvatski i engleski jezik u nazivima metoda i klasa.

**Preporuka:** Usvojiti jedan jezik (preporučeno engleski) za sve nazive u kodu.

---

## Sažetak Komentara

- **Kritični problemi:** 3
- **Visoki problemi:** 7
- **Srednji problemi:** 5
- **Ukupno:** 15 komentara

**Prioritet:** Riješiti sve kritične probleme prije merge-a.
