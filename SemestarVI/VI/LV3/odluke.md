# Tehničke odluke - LV3

## train_test_split — random_state=42
Postavka kaže 80/20 podjelu, ali ne specificira seed. Koristimo `random_state=42` kako bi rezultati bili reproducibilni između pokretanja.

## model.fit — validation_split=0.2
Postavka specificira samo broj epoha i batch size. Dodali smo `validation_split=0.2` kako bismo mogli pratiti overfitting tokom treniranja (val_loss, val_accuracy po epohi).

## pd.concat — ignore_index=True
Postavka kaže samo "spojiti ih u jedan DataFrame". Koristimo `ignore_index=True` da se indeksi resetu ju u 0..N umjesto da ostanu duplirani indeksi iz originalnih DataFrame-ova.

## X i y — .values (numpy array)
Postavka ne specificira format. Konvertujemo u numpy array pozivom `.values` jer Keras bolje radi s numpy arrayima nego s pandas objektima i izbjegavamo potencijalna upozorenja.

## Izlazni sloj — bez aktivacijske funkcije
Postavka za podzadatak d) kaže samo "Dense sa samo jednim neuronom", bez navođenja aktivacije. Izostavili smo aktivacijsku funkciju na izlaznom sloju (Keras tada podrazumijeva linearnu), što u kombinaciji s `binary_crossentropy` interno primjenjuje sigmoid — ekvivalentno eksplicitnom `sigmoid`, ali numerički stabilnije.

## Skaliranje u g) — samo X_train/X_test, ne X iz wines
Scaler se fita isključivo na `X_train` (ne na cijeli dataset) kako testni skup ostao neovisan, što je i inače dobra praksa, a PDF to i naglašava.

## Histogrami — figsize=(14, 10) i tight_layout()
Postavka kaže samo nacrtati histograme. Dodali smo veličinu figure i `tight_layout()` jer dataset ima 13 kolona pa bi defaultni prikaz bio nečitljiv.
