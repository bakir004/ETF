# Struktura Bachelor Rada: Spektralna Teorija Grafova

## 1. Uvod

U uvodnom poglavlju se obrazlaže tema rada, motivacija za proučavanje
spektralne teorije grafova i daje se pregled strukture cijelog rada.

### 1.1. Obrazloženje teme

Ova sekcija objašnjava zašto su spektralna teorija grafova, Laplasijan
grafa i spektralno klasterisanje važne teme za matematičku analizu
grafova i praktičnu analizu podataka.

### 1.2. Struktura rada

Ova sekcija ukratko opisuje sadržaj svakog poglavlja i objašnjava kako
se teorijski dio rada povezuje sa eksperimentalnim dijelom.

## 2. Teoretske osnove

U ovom poglavlju se uvode osnovni pojmovi linearne algebre, teorije
grafova i matrica povezanih sa grafovima, koji su potrebni za
razumijevanje spektralne teorije grafova.

### 2.1. Osnove linearne algebre

Ova sekcija uvodi vektore, matrice, svojstvene vrijednosti, svojstvene
vektore, spektar matrice, pozitivnu semidefinitnost i spektralni teorem
za simetrične matrice.

### 2.2. Elementi teorije grafova

Ova sekcija uvodi osnovne pojmove teorije grafova, uključujući graf,
neusmjereni i usmjereni graf, stepen čvora, put, ciklus, podgraf,
povezanost i komponente povezanosti.

### 2.3. Matrice vezane za grafove

Ova sekcija definiše matricu susjedstva, matricu stepena i Laplasijan
grafa kao osnovne matrice pomoću kojih se struktura grafa može
analizirati algebarski.

### 2.4. Osobine Laplasijana i njegovog spektra

Ova sekcija pokazuje da je Laplasijan neusmjerenog grafa simetrična i
pozitivno semidefinitna matrica, te objašnjava zašto su njegove
svojstvene vrijednosti realne i nenegativne.

### 2.5. Nulprostor Laplasijana i povezane komponente

Ova sekcija objašnjava vezu između nulprostora Laplasijana i povezanih
komponenti grafa, odnosno činjenicu da je multiplicitet svojstvene
vrijednosti nula jednak broju komponenti povezanosti.

### 2.6. Fiedlerova vrijednost i Fiedlerov vektor

Ova sekcija uvodi drugu najmanju svojstvenu vrijednost Laplasijana kao
algebraičku povezanost grafa i objašnjava ulogu Fiedlerovog vektora u
particionisanju grafa.

### 2.7. Spektralno particionisanje grafa

Ova sekcija objašnjava kako se svojstveni vektori Laplasijana koriste
za približno rješavanje problema podjele grafa na dobro povezane
cjeline.

### 2.8. RatioCut kriterij

Ova sekcija uvodi RatioCut kao kriterij particionisanja koji minimizira
broj presječenih grana uz kažnjavanje nebalansiranih podjela.

### 2.9. Primjena spektralne teorije na klasterisanje

Ova sekcija objašnjava kako se rezultati spektralne teorije grafova
koriste za otkrivanje klastera u podacima. Skup podataka se posmatra
kao graf sličnosti, a svojstveni vektori Laplasijana daju novu
reprezentaciju podataka u kojoj je klastere lakše razdvojiti.

### 2.10. Konstrukcija grafa sličnosti

Ova sekcija opisuje osnovne načine konstrukcije grafa sličnosti,
uključujući epsilon-susjedstvo, graf k-najbližih susjeda i potpuno
povezani graf sa Gaussovom funkcijom sličnosti.

### 2.11. Spektralni embedding

Ova sekcija objašnjava kako se pomoću nekoliko svojstvenih vektora
Laplasijana svaki čvor grafa, odnosno svaka podatkovna tačka,
preslikava u novi niskodimenzionalni prostor. U tom prostoru geometrija
podataka odražava povezanost u grafu sličnosti, pa klasteri često
postaju lakše razdvojivi.

### 2.12. Algoritam spektralnog klasterisanja

Ova sekcija daje osnovne korake spektralnog klasterisanja: konstrukciju
grafa sličnosti, računanje Laplasijana, formiranje spektralnog
embeddinga pomoću svojstvenih vektora i primjenu metode k-sredina u
spektralnom prostoru.

### 2.13. Normalizovano spektralno klasterisanje

Ova sekcija uvodi normalizovane Laplasijane i objašnjava zašto su
korisni kod grafova sa čvorovima različitih stepena.

### 2.14. Normalized Cut kriterij

Ova sekcija opisuje Normalized Cut kao kriterij particionisanja koji
uzima u obzir odnos presječenih grana i ukupne povezanosti pojedinih
dijelova grafa.

### 2.15. Primjena spektralne teorije na manifold learning

Ova sekcija objašnjava kako se graf sličnosti može posmatrati kao
diskretna aproksimacija geometrijske strukture podataka i kako je
Laplasijan povezan sa metodama manifold learninga. Posebno se ističe
veza između grafa sličnosti, spektralnog embeddinga i aproksimacije
geometrije mnogostrukosti na kojoj podaci leže.

### 2.16. Zaključak poglavlja

Ova sekcija sumira teorijske rezultate i pravi prijelaz prema
implementaciji i eksperimentalnoj analizi.

## 3. Implementacija i eksperimentalna analiza

U ovom poglavlju se opisuje implementacija spektralnog klasterisanja i
poređenje sa drugim metodama klasterisanja na sintetičkim i realnim
skupovima podataka.

### 3.1. Opis korištenih metoda

Ova sekcija predstavlja metode koje se porede u eksperimentima,
prvenstveno spektralno klasterisanje i metodu k-sredina, a po potrebi i
DBSCAN ili hijerarhijsko klasterisanje.

### 3.2. Opšti tok implementacije spektralnog klasterisanja

Ova sekcija opisuje praktične korake implementacije: konstrukciju
matrice sličnosti, računanje matrice stepena, Laplasijana, svojstvenih
vektora i konačnih klastera.

### 3.3. Sintetički skupovi podataka

Ova sekcija uvodi jednostavne dvodimenzionalne skupove podataka koji
omogućavaju vizuelno poređenje metoda klasterisanja.

### 3.4. Two moons skup podataka

Ova sekcija analizira skup podataka two moons, koji je pogodan za
pokazivanje prednosti spektralnog klasterisanja nad metodom k-sredina
kod nelinearno separabilnih klastera.

### 3.5. Koncentrične kružnice

Ova sekcija analizira skup podataka sa koncentričnim kružnicama, gdje
klasteri imaju geometrijsku strukturu koju klasične centroidne metode
teško prepoznaju.

### 3.6. MNIST skup podataka

Ova sekcija opisuje primjenu metoda na podskupu MNIST skupa ručno
pisanih cifara, sa ciljem ispitivanja ponašanja spektralnog
klasterisanja na visokodimenzionalnim podacima.

### 3.7. Mjere evaluacije

Ova sekcija uvodi mjere za poređenje rezultata klasterisanja, kao što
su tačnost nakon optimalnog uparivanja klastera, Adjusted Rand Index i
Normalized Mutual Information.

### 3.8. Rezultati eksperimenata

Ova sekcija treba sadržavati tabele, grafike i numeričke rezultate
dobijene primjenom različitih metoda klasterisanja.

### 3.9. Diskusija rezultata

Ova sekcija interpretira dobijene rezultate, objašnjava prednosti i
ograničenja spektralnog klasterisanja i komentariše uticaj izbora
parametara.

### 3.10. Zaključak poglavlja

Ova sekcija sumira eksperimentalni dio rada i pravi prijelaz prema
završnom zaključku.

## 4. Zaključak

U završnom poglavlju se sumiraju najvažniji teorijski i eksperimentalni
rezultati rada, navode se ograničenja korištenog pristupa i predlažu
mogući pravci daljeg istraživanja.

### 4.1. Sažetak rada

Ova sekcija ukratko ponavlja glavnu ideju rada: vezu između Laplasijana
grafa, njegovog spektra, povezanosti grafa i spektralnog klasterisanja.

### 4.2. Glavni zaključci

Ova sekcija ističe najvažnije zaključke, posebno ulogu Fiedlerove
vrijednosti, Fiedlerovog vektora, grafa sličnosti i spektralnog
embeddinga u analizi strukture podataka.

### 4.3. Ograničenja rada

Ova sekcija navodi praktična ograničenja spektralnog klasterisanja, kao
što su računska složenost, osjetljivost na parametre i problem
konstrukcije kvalitetnog grafa sličnosti.

### 4.4. Mogući pravci daljeg istraživanja

Ova sekcija predlaže proširenja rada, poput primjene na veće skupove
podataka, detaljnijeg proučavanja normalizovanih Laplasijana, manifold
learning metoda i efikasnijih numeričkih algoritama.
