# LV2 - Odbrana: Uvod u Python (2/2)

---

## Zadatak 1 - Obrada podataka kroz Pandas

### a) Ucitavanje CSV-a i prikaz podataka
`pd.read_csv('izvjestaj.csv')` ucitava fajl i automatski uzima prvi red kao nazive kolona.
`data.head(5)` vraca prvih 5 redova, `data.tail(10)` vraca posljednjih 10.

### b) Prikaz kolona Ispit1 i Ispit2
`data[['Ispit1', 'Ispit2']]` — dvostruke uglate zagrade znace da se bira lista kolona, ne samo jedna. Rezultat je DataFrame, ne Series.

### c) Studenti koji su izgubili prisustvo
`data.loc[data['Prisustvo'] == 0]` — `.loc` prima logicki uslov. Vraca sve redove gdje je Prisustvo nula.

### d) Studenti sa ocjenom 8 ili visom
Problem: kolona `Ocjena` je tipa `object` jer sadrzi i znak `/`. Prije poredjenja konvertujemo je pomocnom varijablom:
```python
ocjena_num = pd.to_numeric(data['Ocjena'], errors='coerce')
```
`errors='coerce'` znaci da sve sto ne moze biti broj postaje `NaN`. Originalna kolona se ne mijenja — ovo radimo samo za filtriranje.

### e) Zamjena '/' sa np.nan
```python
data.replace('/', np.nan, inplace=True)
```
Pandas koristi `np.nan` kao standardnu oznaku nedostajuce vrijednosti. Znak `/` je samo konvencija u ovom CSV-u. `inplace=True` znaci da se mijenja originalni DataFrame, bez potrebe za novom varijablom.

### f) Odbacivanje studenata bez ocjene
```python
data.dropna(subset=['Ocjena'], inplace=True)
data.reset_index(drop=True)
```
`dropna(subset=['Ocjena'])` brise samo redove gdje je `Ocjena` NaN. `reset_index(drop=True)` vraca indekse na 0,1,2,... jer nakon brisanja redova indeksi ostaju "rupe" (npr. 0,1,5,6,...). `drop=True` znaci da stari indeks ne postaje nova kolona.

### g) Kolona Ispit1_final — bolji rezultat od dva ispita
```python
data['Ispit1_final'] = data[['Ispit1', 'Ispit1_popravni']].max(axis=1)
```
`.max(axis=1)` racuna maksimum po redu (ne po koloni). Ako je jedan od dva NaN, `max` automatski ignorise NaN i uzima onu vrijednost koja postoji. Ako su oba NaN, rezultat je NaN.

### h) Isti postupak za Ispit2
Analogno: `data['Ispit2_final'] = data[['Ispit2', 'Ispit2_popravni']].max(axis=1)`

### i) Brisanje starih kolona za ispite
```python
data.drop(columns=['Ispit1', 'Ispit2', 'Ispit1_popravni', 'Ispit2_popravni'], inplace=True)
```
Stare kolone vise nisu potrebne jer smo informaciju sacuvali u `_final` kolonama.

### j) Cuvanje kao CSV sa separatorom tacka-zarez
```python
data.to_csv('izvjestaj_modificirano.csv', sep=';', index=False)
```
`sep=';'` — separator je tacka-zarez. `index=False` sprecava pisanje Pandas indeksa kao extra kolone u fajl.

### k) Cuvanje kao Pickle
```python
pickle.dump(data, open('izvjestaj_modificirano_pickle.p', 'wb'), protocol=pickle.HIGHEST_PROTOCOL)
```
Pickle serijalizira Python objekt (ovdje DataFrame) u binarni format. `'wb'` = write binary. `HIGHEST_PROTOCOL` koristi najbrzi dostupni protokol. Korisno za brzo ponovo ucitavanje bez parsiranja CSV-a.

---

## Zadatak 2 - Normalizacija podataka pomocu Sklearn

### b) Zamjena NaN u redovnim ispitima sa median
```python
si_median = SimpleImputer(strategy='median')
data2[['Ispit1', 'Ispit2']] = si_median.fit_transform(data2[['Ispit1', 'Ispit2']])
```
`fit_transform` u jednom koraku uci medianu iz podataka i primjenjuje zamjenu. Mediana je robusnija od srednje vrijednosti jer nije osjetljiva na ekstremne vrijednosti (outliere).

### c) Zamjena NaN u popravnim ispitima sa mean
```python
si_mean = SimpleImputer(strategy='mean')
```
Za popravne ispite koristimo srednju vrijednost (mean). Razlika u strategiji je namjerna — zadatak eksplicitno trazi razlicite strategije za redovne i popravne.

### d) Z-score normalizacija za redovne ispite
```python
data2['Ispit1'] = scale(data2['Ispit1'])
```
Z-score formula: `Z = (x - srednja_vrijednost) / standardna_devijacija`
Nakon transformacije, kolona ima srednju vrijednost 0 i standardnu devijaciju 1. Vrijednosti su negativne ako su ispod prosjeka, pozitivne ako su iznad.

### e) MinMax normalizacija za popravne ispite
```python
mm = MinMaxScaler()
data2[['Ispit1_popravni', 'Ispit2_popravni']] = mm.fit_transform(...)
```
MinMax formula: `x_novo = (x - min) / (max - min)`
Sve vrijednosti su svedene na interval [0, 1]. Za razliku od Z-score-a, MinMax cuva distribuciju, ali je osjetljiv na outliere.

**Zasto normalizacija uopste?** Algoritmi masinstog ucenja (posebno neuralne mreze i k-NN) mjere udaljenosti ili gradijenate. Ako jedna kolona ima vrijednosti 0-20 a druga 0-100, ona veca dominira — normalizacija to izjednacava.

### f) Preostali NaN -> 0
```python
data2.fillna(0, inplace=True)
```
Npr. `Prisustvo` i `UKUPNO` mogu imati NaN. Popunjavanje nulom je konzervativna strategija.

### g) Izdvajanje kolone Ocjena
```python
ocjena = data2['Ocjena'].copy()
data2.drop(columns=['Ocjena'], inplace=True)
```
`.copy()` je vazno — bez njega `ocjena` bi bila referenca na kolonu u DataFrame-u, i brisanjem kolone bi se izgubila i vrijednost.

### h) Konverzija u NumPy
```python
X = data2.to_numpy()
y = ocjena.to_numpy()
```
Sklearn i TensorFlow rade sa NumPy nizovima. DataFrame ima overhead (labele, indeksi) koji nije potreban za trening.

### i) Podjela na trening i testni skup
```python
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
```
`test_size=0.2` — 20% podataka ide u testni skup, 80% u trening.
`random_state=42` — fiksira generator slucajnih brojeva, garantuje iste rezultate pri svakom pokretanju.
Trening skup koristi algoritam za ucenje, testni skup za evaluaciju (simulira nevidjene podatke).

---

## Zadatak 3 - Klasicni algoritam masinskog ucenja (k-NN)

### a) Iris skup podataka
`load_iris()` vraca objekat sa:
- `iris.data` — matrica 150x4 (4 znacajke: duzina/sirina lapice i cašice)
- `iris.target` — niz od 150 labela (0, 1, 2 za tri vrste)

### b) Podjela 70/30
`test_size=0.3` — 30% ide u testni skup (45 uzoraka), 70% u trening (105 uzoraka).
`random_state=1` — reproducibilnost.

Da li je ovo veliki skup? Iris ima samo 150 uzoraka — veoma mali. U praksi bi trebalo vise podataka.

### c) Treniranje k-NN sa k=3
```python
classifier_knn = KNeighborsClassifier(n_neighbors=3)
classifier_knn.fit(X_train, y_train)
```
**Kako radi k-NN:** Za novi podatak, algoritam trazi k najblizih susjedau trening skupu (mjeri Euklidsku udaljenost u 4D prostoru znacajki). Klasa koja se najcesce pojavljuje medu k susjedima postaje predikcija.
Ovdje je k=3, dakle gleda se 3 najbliza susjeda.
`fit` je trivijalan za k-NN — samo "pamti" trening podatke (lazy learning).

### d) Evaluacija tacnosti
```python
y_pred = classifier_knn.predict(X_test)
metrics.accuracy_score(y_test, y_pred)
```
`accuracy_score = broj_tacnih_predikcija / ukupan_broj`. Ocekivana tacnost je ~97%.
Je li to zadovoljavajuce? Za Iris — da, jer je to standardni benchmark i ovakvi rezultati su ocekivani.

### e) Razlicite vrijednosti k
- **Mali k (npr. k=1):** Overfitting — jako osjetljiv na sum i outliere, ali odlicno pamti trening.
- **Veliki k (npr. k=45):** Underfitting — gleda pola skupa, predikcije postaju grube.
- **Optimalni k:** Negdje u sredini; obicno se trazi kros-validacijom.
- **k = broj uzoraka u testnom skupu:** Svaki novi podatak bi imao isti glas od cijelog trening skupa — klasa sa najvise uzoraka uvijek pobijedi, ignorise lokalna svojstva.

---

## Zadatak 4 - Duboko ucenje (MNIST)

### a) MNIST skup podataka
- 60000 slika za trening, 10000 za test
- Svaka slika je 28x28 piksela u nijansama sive (0-255)
- 10 klasa (cifre 0-9)
- `x_train.shape = (60000, 28, 28)`, `y_train.shape = (60000,)`

### b) Reshape i normalizacija
```python
train_images = x_train.reshape((x_train.shape[0], 28 * 28))
train_images = train_images.astype('float32') / 255
```
`reshape`: Slika 28x28 se "razvuce" u vektor od 784 elemenata. Dense sloj ocekuje 1D ulaz.
`astype('float32')`: Pikseli su `uint8` (cijeli brojevi 0-255). Za dijeljenje i racunanje gradijenata treba float.
`/ 255`: Svodi vrijednosti na [0.0, 1.0]. Neuronske mreze konvergiraju brze sa malim vrijednostima.

Memorija se smanjuje jer float32 zauzima 4 bajta po elementu, ali broj elemenata ostaje isti.

### c) Arhitektura mreze
```python
network.add(layers.Dense(512, activation='relu', input_shape=(784,)))
network.add(layers.Dense(10, activation='softmax'))
```
- **Dense sloj:** Svaki neuron je povezan sa svakim neuronom prethodnog sloja (fully connected).
- **512 neurona + ReLU:** Skriveni sloj koji uci reprezentacije. ReLU = max(0, x), eliminise negativne vrijednosti.
- **10 neurona + Softmax:** Izlazni sloj. Softmax vraca 10 vjerovatnoca koje se sabiraju na 1. Npr. [0.02, 0.01, ..., 0.95, ...] znaci da je mreza 95% sigurna da je cifra 9.

### d) Kompajliranje
- **rmsprop optimizer:** Adaptivna stopa ucenja — dobro radi na problemima slika.
- **categorical_crossentropy:** Loss funkcija za multi-class klasifikaciju. Mjeri koliko su predikovane vjerovatnoce lose u odnosu na tacne labele.
- **metrics=['accuracy']:** Prati tacnost tokom treninga.

### e) One-hot kodiranje labela
Izlaz mreze je vektor od 10 vjerovatnoca. Da bi se poredio sa labelom, labela mora biti u istom formatu:
```
cifra 3  ->  [0, 0, 0, 1, 0, 0, 0, 0, 0, 0]
```
`to_categorical(y_train)` radi ovu konverziju automatski.

### f) Treniranje
```python
network.fit(train_images, train_labels, epochs=5, batch_size=128)
```
- **epochs=5:** Mreza prolazi kroz cijeli trening skup 5 puta.
- **batch_size=128:** Umjesto da azurira tezine nakon svake slike, skuplja 128 slika i racuna prosjecni gradijent. Brze i stabilnije.

### g) Evaluacija
- Ocekivana tacnost na testnom skupu: ~97-98%.
- Tacnost na trening skupu je obicno malo visa — ako je razlika velika, radi se o overfittingu.
- Za ovaj jednostavni model, rezultat je zadovoljavajuci.

**Zasto je tacnost na testnom skupu nesto niza?** Mreza je vidjela trening podatke 5 puta i malo ih je "zapamtila". Testni podaci su potpuno novi — to je realna provjera generalizacije.
