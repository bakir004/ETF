# LV2 - Analiza odluka u kodu (priprema za odbranu)

Ovaj fajl pokriva samo mjesta gdje si imao IZBOR - gdje kod nije morao biti
bas takav kakav jeste. Za svaki slucaj: sta si uradio, zasto je to ispravno,
i kako to objasniti ako te pitaju.

Dijelovi koji su direktno preuzeti iz PDF-a ili nemaju alternativu nisu ovdje
navedeni (tu nema sta braniti - moralo je tako).

---

## ZADATAK 1 - PANDAS

---

### [1d] Filtriranje po Ocjena PRIJE nego sto je '/' zamijenjen

```python
ocjena_num = pd.to_numeric(data['Ocjena'], errors='coerce')
display(data.loc[ocjena_num >= 8, ['Indeks', 'UKUPNO', 'Ocjena']])
```

**Zasto je ovo bila neophodna odluka:**
Zadatak ide redoslijedom a-b-c-d-e. Korak e (zamjena '/' sa NaN) dolazi NAKON
koraka d. U trenutku d, kolona Ocjena sadrzi i '/' kao string, pa je cijela
kolona tipa `object`. Direktno `data['Ocjena'] >= 8` bi puklo ili dalo
pogresne rezultate.

**Kako objasniti:**
"U ovom trenutku zamjena '/' jos nije bila urađena (to je tek korak e).
Zato sam kreirao pomocnu privremenu seriju pomocu `pd.to_numeric` sa
`errors='coerce'` - sve sto nije broj (tj. '/') postaje NaN, a NaN >= 8
je automatski False pa se ti redovi iskljucuju iz filtriranja. Originalna
kolona ostaje nepromijenjena."

---

### [Između 1f i 1g] Eksplicitna konverzija kolona u numericki tip

```python
numeric_cols = ['Ispit1', 'Ispit2', 'Ispit1_popravni', 'Ispit2_popravni', 'UKUPNO', 'Ocjena', 'Prisustvo']
for col in numeric_cols:
    data[col] = pd.to_numeric(data[col], errors='coerce')
```

**Zasto je ovo bila neophodna odluka:**
Zadatak eksplicitno ne trazi ovaj korak, ali bez njega korak g ne bi radio.
Nakon sto smo zamijenili '/' sa NaN, Pandas zadrzava originalni dtype kolona
(object), jer su one ucitane iz stringa. `.max(axis=1)` nad object kolonama
ne radi korektno s brojevima.

**Kako objasniti:**
"Nakon replace-a kolone su i dalje object dtype. Da bi max() u sljedecem
koraku ispravno poredjao numericke vrijednosti i ignorirao NaN, morao sam
eksplicitno konvertovati u float. `errors='coerce'` osigurava da eventualni
ostaci ne-numerickih podataka postanu NaN umjesto greske."

---

### [1f] reset_index nakon dropna

```python
data.dropna(subset=['Ocjena'], inplace=True)
data = data.reset_index(drop=True)   # <-- ovo je odluka
```

**Da li je moralo biti:**
`dropna` je morao biti. `reset_index` nije bio eksplicitno traizan u zadatku,
ali PDF ga eksplicitno pominje kao rijesenje za "rupe" u indeksima (Slika 15).

**Kako objasniti:**
"Nakon sto se izbace redovi, Pandas ne azurira indekse automatski - ostaju
'rupe' (npr. 0, 1, 3, 7...). `reset_index(drop=True)` vraca indekse na
kontinuirani niz od 0. `drop=True` sprecava da stari indeks postane nova
kolona u DataFrame-u - sto nam ne treba."

---

### [1g/1h] Koriscenje .max(axis=1) za Ispit_final

```python
data['Ispit1_final'] = data[['Ispit1', 'Ispit1_popravni']].max(axis=1)
```

**Da li je moralo biti ovako:**
Nije. Alternativa bi bila:
```python
data.apply(lambda row: max(row['Ispit1'], row['Ispit1_popravni']), axis=1)
```
Ali to ne bi radilo kad je jedan NaN (Python-ov max() ne ignorise NaN).

**Kako objasniti:**
"`.max(axis=1)` racuna maksimum po redovima i automatski ignorise NaN
vrijednosti - ako student nije pisao popravni (NaN), uzima se redovni
rezultat, i obrnuto. To je tocno ponasanje koje zadatak opisuje ('uzima
bolji rezultat'), bez ikakve dodatne if/else logike."

---

### [1j] index=False pri snimanju CSV-a

```python
data.to_csv('izvjestaj_modificirano.csv', sep=';', index=False)
```

**Da li je moralo biti ovako:**
`sep=';'` morao biti (zadatak trazi). `index=False` je odluka.

**Kako objasniti:**
"Bez `index=False`, Pandas bi u CSV upisao i automatski generisani redni
broj (0, 1, 2...) kao prvu kolonu. To bi stvorilo redundantnu kolonu koja
ne postoji u originalnim podacima - DataFrame vec ima kolone id i Indeks.
Taj nepotrebni indeks bi zbunjivao svakoga ko ucitava sacuvani fajl."

---

### [1k] protocol=pickle.HIGHEST_PROTOCOL

```python
pickle.dump(data, open('izvjestaj_modificirano_pickle.p', 'wb'), protocol=pickle.HIGHEST_PROTOCOL)
```

**Da li je moralo biti ovako:**
Nije. Zadatak samo kaze "sacuvati kao Pickle datoteku". Protokol nije
specificiran.

**Kako objasniti:**
"PDF eksplicitno kaze da je Pickle biblioteka izuzetno brza i preporucuje
koristenje za velike skupove. `HIGHEST_PROTOCOL` koristi najnoviji binarni
format koji je i brzi i kompaktniji od defaultnog. Ovo je best practice
prema Python dokumentaciji za situacije kada citas/pises isti fajl vise
puta."

---

## ZADATAK 2 - SKLEARN

---

### [2g] .copy() pri izdvajanju kolone Ocjena

```python
ocjena = data2['Ocjena'].copy()   # <-- .copy() je odluka
data2.drop(columns=['Ocjena'], inplace=True)
```

**Da li je moralo biti ovako:**
Nije nuzno, ali je ispravna praksa.

**Kako objasniti:**
"Bez `.copy()`, `ocjena` bi bila view (pogled) na kolonu unutar data2, a ne
nezavisna kopija. Kad potom dropamo tu kolonu iz data2, Pandas moze izbaciti
`SettingWithCopyWarning` ili doci do neocekivanog ponasanja. `.copy()`
garantuje da je `ocjena` potpuno nezavisna varijabla, sto je sigurnija
praksa."

---

### [2i] random_state=42 u train_test_split

```python
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
```

**Da li je moralo biti ovako:**
`test_size=0.2` morao biti (zadatak kaze 20%). `random_state=42` je odluka
- mogao biti bilo koji cijeli broj.

**Kako objasniti:**
"`random_state` fiksira seed za generator slucajnih brojeva, sto osigurava
reproduktibilnost - svaki put kad se kod pokrene, dobija se tacno ista
podjela skupa. To je standardna praksa u masinskom ucenju jer bez fiksiranog
seed-a bi razliciti pokretaji mogli dati razlicite rezultate, sto otezava
poredjenje i debugging. Vrijednost 42 nema matematicko znacenje - to je
konvencija koja je postala standard u ML zajednici."

---

## ZADATAK 3 - k-NN

---

### [3e] Odabir specificnih vrijednosti k

```python
for k in [1, 3, 5, 7, 10, 15, 20, 45]:
```

**Da li je moralo biti ovako:**
Zadatak kaze "isprobajte razlicite vrijednosti k" - nije specificirao koje.

**Kako objasniti:**
"Odabrao sam vrijednosti koje pokrivaju citav spektar ponasanja k-NN:
- k=1: ekstrem - direktno pamcenje (overfitting)
- k=3: upravo koristeni k iz prethodnog koraka, referentna tacka
- k=5,7,10,15,20: gradacija za pracenje trenda
- k=45: testni skup ima tacno 45 uzoraka (30% od 150). PDF eksplicitno pita
  'sta bi se desilo kad je k jednak broju uzoraka u testnom skupu' - ovo
  daje direktan empirijski odgovor: tacnost opada jer glasanje postaje
  previse globalno i gubi lokalnu osjetljivost."

---

## STA JE BILO STRIKTNO PROPISANO (bez odluka)

Ovi dijelovi su preuzeti direktno iz PDF-a ili nemaju smislenu alternativu:

| Zadatak | Sta | Zasto nije odluka |
|---------|-----|-------------------|
| 1a | head(5) i tail(10) | PDF kaze tacno tako |
| 1b | data[['Ispit1','Ispit2']] | jedini nacin za vise kolona |
| 1c | loc[Prisustvo == 0] | jedini logican nacin |
| 1e | replace('/', np.nan) | direktno iz PDF-a |
| 1i | drop 4 stare kolone | zadatak propisuje |
| 2b | strategy='median' | zadatak propisuje |
| 2c | strategy='mean' | zadatak propisuje |
| 2d | scale() za Z-score | zadatak propisuje |
| 2e | MinMaxScaler() | zadatak propisuje |
| 3b | test_size=0.3, random_state=1 | PDF daje tacan kod |
| 3c | n_neighbors=3 | zadatak kaze k=3 |
| 4 sve | cijeli zadatak 4 | kod preuzet direktno iz PDF-a |
