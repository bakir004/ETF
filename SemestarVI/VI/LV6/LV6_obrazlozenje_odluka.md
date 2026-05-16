# LV6 - Obrazlozenje odluka i proizvoljnih vrijednosti

Ovaj dokument objasnjava sve kljucne odluke donesene u notebook-u `LV6_CNN_CIFAR10.ipynb`, posebno one koje nisu striktno zadane u tekstu vjezbe ili su mogle imati vise validnih varijanti.

## 1) Tehnicki izbori okoline i biblioteka

- Koristen je `tensorflow.keras` (umjesto standalone `keras`) zbog bolje kompatibilnosti sa Google Colab okolinom.
- Koristen je `matplotlib` za vizualizaciju jer je standardan alat za ovakve zadatke i radi bez dodatne konfiguracije.

## 2) Reproducibilnost

- Postavljeno je `np.random.seed(42)`.
- Vrijednost `42` je proizvoljna, ali vrlo cesto koristen "default" seed radi ponovljivosti rezultata.
- Poenta nije da je 42 "najbolji" seed, nego da isti kod daje konzistentnije rezultate izmedju pokretanja.

## 3) Prikaz prvih 9 slika

- Izabran je prikaz u mrezi 3x3 jer je trazeno prvih 9 slika, a ovaj raspored je najcistiji za vizuelnu inspekciju.
- U naslovu svake slike prikazana je klasa radi lakseg komentara o kvalitetu podataka.

## 4) Predprocesiranje podataka

- `astype('float32')`: standard za rad sa neuronskim mrezama, balans brzine i preciznosti.
- Skaliranje sa `/ 255.0`: posto su RGB pikseli u rasponu [0, 255], ova transformacija mapira ulaz u [0, 1], sto stabilizuje trening.
- `to_categorical(..., 10)`: zadatak je multiclass (10 klasa), a izlazni sloj koristi `softmax`, pa je one-hot enkodiranje prirodan izbor.

## 5) Baseline arhitektura (d i e)

Arhitektura je implementirana tacno po specifikaciji vjezbe:

- Blok 1: Conv(32) -> Conv(32) -> MaxPool
- Blok 2: Conv(64) -> Conv(64) -> MaxPool
- Zatim: Flatten -> Dense(128, relu) -> Dense(10, softmax)

Dodatne odluke:

- `padding='same'` u svim Conv slojevima, jer zadatak eksplicitno trazi ukljucen padding.
- `activation='relu'` u skrivenim slojevima jer je standardni izbor za CNN (brze i stabilnije ucenje od saturirajuceg sigmoida/tanh u vecini slucajeva).
- `softmax` u izlazu jer model predvidja distribuciju vjerovatnoca preko 10 klasa.
- `loss='categorical_crossentropy'` jer odgovara one-hot labelama i softmax izlazu.
- `metrics=['accuracy']` jer je trazena metrika tacnosti klasifikacije.

## 6) Optimizator

- Koristen je `SGD(learning_rate=0.001, momentum=0.9)` jer je to direktno zadano u tekstu vjezbe.
- Napomena: u nekim starijim primjerima stoji `lr`, ali moderni TensorFlow/Keras preporucuje `learning_rate`.

## 7) Parametri treniranja

- `epochs=15`, `batch_size=64`, `validation_data=(testX, testY)` za baseline i dropout v1 model su uzeti direktno iz zadatka.
- Za finalni model je koristen `epochs=30`, takodjer direktno prema zadatku.
- `batch_size=64` je dobar kompromis izmedju brzine i stabilnosti gradijenta na CIFAR-10.

## 8) Dropout model v1 (h)

- Dodat je `Dropout(0.5)` prije prvog Dense sloja, tacno kako je trazeno.
- Razlog ove pozicije: dense dio mreze ima mnogo parametara i skloniji je overfitting-u; dropout tu obicno donosi najveci efekat regularizacije.
- Vrijednost 0.5 je "jaca" regularizacija i moze smanjiti overfitting, ali ponekad i blago sniziti train accuracy.

## 9) Finalni model (i)

Uradjene su sve tri trazene modifikacije:

1. Dodat treci konvolucioni blok sa 128 filtera (dva Conv + jedan MaxPool);
2. Dropout prije Dense smanjen sa 0.5 na 0.2 i pomjeren iza prvog Dense sloja;
3. Nakon svakog MaxPooling sloja dodat Dropout(0.2).

Razlozi:

- Dublji model (dodani 128-filter slojevi) hvata slozenije vizuelne obrasce.
- Dropout 0.2 je umjerena regularizacija: dovoljno jaka da smanji overfitting, ali ne preagresivna kao 0.5 kroz cijeli model.
- Raspored dropout slojeva kroz mrezu (nakon poolinga + iza Dense) regularizuje i konvolucioni i potpuno povezani dio.

## 10) Vizualizacija metrika

- Implementirana je funkcija `plot_history(...)` da bi se isti kod mogao koristiti za sva tri modela bez dupliranja.
- Prikazani su i train i validation grafovi za accuracy/loss jer je to kljucno za procjenu overfitting-a.

## 11) Ocekivanja rezultata i interpretacija

- Tacna numericka vrijednost accuracy moze varirati od pokretanja do pokretanja zbog stohasticnosti treninga.
- Zato su u notebook-u komentari pisani kao "ocekivani opseg/trend", a ne kao fiksna cifra.
- Najvazniji kriterij nije samo veci train accuracy, nego balans train-vs-validation performansi (generalizacija).

## 12) Zasto je ovakav format pogodan za odbranu

- Svaki podzadatak (a-i) ima zasebnu sekciju/celiju, pa je lako pratiti da je sve iz upute ispunjeno.
- U notebook-u su dodani kratki komentari nakon kljucnih koraka (f, g, h, i), sto olaksava usmenu interpretaciju rezultata.
- Ovaj `.md` dokument daje argumente "zasto" iza svake odluke, sto direktno pomaze pri odbrani.
