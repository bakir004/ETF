# VI Lab 5 - Obrazlozenje izbora

Ovaj dokument sluzi kao kratko obrazlozenje odluka koje nisu bile striktno zadate, da mozes lakse braniti rjesenje.

## 1) Zasto MinMaxScaler i zasto `fit` samo na trening skupu

- U zadatku je eksplicitno trazen MinMaxScaler, pa je koristen taj pristup.
- `fit_transform` je uradjen nad `train_data`, a samo `transform` nad `test_data` da ne dodje do curenja informacija (data leakage) iz testnog skupa u proces treniranja.
- Ovim simuliramo realan scenario: model tokom ucenja ne zna statistike buducih/nevidjenih podataka.

## 2) Zasto ova arhitektura mreze

- Koristena je funkcija `model_mreze()` koja vraca model sa:
  - 2 skrivena `Dense` sloja po 64 neurona,
  - `relu` aktivacijom u skrivenim slojevima,
  - izlaznim slojem od 1 neurona bez aktivacijske funkcije.
- Ovo je direktno uskladjeno sa zahtjevom zadatka za regresiju.
- Bez aktivacijske funkcije na izlazu model moze predvidjati kontinuiranu realnu vrijednost.

## 3) Zasto `mse` loss i `mae` metrika

- `mse` je standardna loss funkcija za regresiju i penalizuje vece greske jace (kvadrat greske).
- `mae` je intuitivna za interpretaciju u ovom zadatku jer target predstavlja hiljade dolara.
- Ako je npr. `MAE = 3.8`, to znaci prosjecno odstupanje oko `$3800`.

## 4) Zasto batch size = 1 i validation_split = 0.1

- Ovo je uradjeno zbog zahtjeva zadatka (batch velicina 1 i 10% validacijskog skupa).
- `validation_split=0.1` omogucava pracenje generalizacije tokom treninga bez dodatnog rucnog cijepanja podataka.

## 5) Zasto 100 i 500 epoha

- U zadatku je trazeno prvo treniranje na 100 epoha, a zatim na 500 epoha.
- Ideja poredjenja je da se vidi da vise epoha ne mora nuzno znaciti bolji rezultat na validacijskom/test skupu.
- Cesto se desi da train greska opada, dok validacijska stagnira ili raste (overfitting).

## 6) Zasto je uradjen smoothed prikaz validation MAE

- Validation MAE zna imati vecu varijansu od epohe do epohe.
- `smooth_curve` (eksponencijalno usrednjavanje) daje citljiviji trend.
- Ignorisanje prvih 10 epoha pomaze jer su pocetne vrijednosti cesto na drugacijoj skali i mogu "sakriti" detalje ostatka krive.

## 7) Kako tumaciti finalnu evaluaciju

- Na kraju se model evaluira na testnom skupu preko `MSE` i `MAE`.
- MAE u hiljadama dolara je preveden u dolare (`MAE * 1000`) radi prakticne interpretacije.
- Kao "bolji" model uzima se onaj sa manjim test MAE, jer direktno oznacava manju prosjecnu gresku predikcije.

## 8) Napomena o reproduktivnosti

- Postavljen je `np.random.seed(42)` radi stabilnijeg ponasanja.
- Ipak, zbog stohasticnosti treniranja neuronskih mreza i razlika u runtime okruzenju (npr. Colab), rezultati mogu blago varirati.
