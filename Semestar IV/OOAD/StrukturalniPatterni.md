# Adapter pattern

Adapter pattern se koristi kada želite da omogućite interakciju između dva nespojiva interfejsa. 
Ovaj obrazac omogućava da se postojeći kod koristi bez izmjene, dok se istovremeno omogućava nova funkcionalnost.

### Gdje bismo ga mi koristili

U slučaju da imamo klasu koja koristi neki stari API, a želimo da pređemo na novi API koji ima drugačiji interfejs.
U tom slučaju bismo napravili adapter koji će prevesti pozive iz starog API-ja u novi API.
Nigdje u trenutnoj implementaciji postoji potreba za adapterima.

# Facade pattern 

Facade pattern se koristi kada želite da pojednostavite interakciju između klijenta i složenog sistema.
Ovaj obrazac omogućava da se klijent poveže sa složenim sistemom putem jednostavnog interfejsa, čime se smanjuje složenost i povećava čitljivost koda.

### Gdje bismo ga mi koristili

Možda bi se mogla napraviti klasa koja bi bila fasada za neke složenije operacije.
Recimo, pošto neke naše akcije uključuju i slanje maila, u tom slučaju vrijedi razviti 
fasadnu klasu koja će pored tražene akcije uključiti i slanje maila.

# Decorator pattern

Decorator pattern se koristi kada želite da dodate novu funkcionalnost postojećem objektu bez izmjene njegovog koda.
Ovaj obrazac omogućava da se objekat "ukrasi" novim funkcionalnostima bez potrebe za stvaranjem novih klasa.
Ovim se pazi na Open/Closed princip, jer se postojeći kod ne mijenja, već se dodaju nove klase koje proširuju funkcionalnost.

### Gdje bismo ga mi koristili

Da nismo obvezani smještanjem svih funkcionalnosti u jednog ASP.NET korisnika,
stavili bismo da je kurirska služba dekorator koji će dodati funkcionalnosti 
koje samo kurirska služba može imati.

# Bridge pattern

Bridge pattern se koristi kada želite da odvojite apstrakciju od njene implementacije.
Ovaj obrazac omogućava da se apstrakcija i njena implementacija razvijaju nezavisno, čime se povećava fleksibilnost i proširivost sistema.

### Gdje bismo ga mi koristili

Da smo imali više tipova korisnika, ili više tipova artikala sa vrlo različitim funkcionalnostima,
onda bi nam bridge pomogao pri spajanju klasa između korisnika i artikala.

# Proxy pattern

Proxy pattern se koristi kada želite da obmotate neki objekat i kontrolišete pristup njemu.
Tu proxy služi kao parent objekat koji barata svojim djetetom, i kontroliše ga.
Primjer toga je klasa slika koja ima funkciju učitavanja. Proxy bi tu valjao
da ne učitava sliku ako je već učitana u memoriju.

### Gdje bismo ga mi koristili

Možemo koristiti proxy klasu da ne ponovno učitavamo artikle koji su već učitani između navigiranja.
Također, proxy se može koristiti za kontrolu pristupa određenim funkcionalnostima ili resursima.

# Composite pattern

Composite pattern se koristi kada želite da tretirate pojedinačne objekte i grupe objekata na isti način.
Ovaj obrazac omogućava da se kreiraju složeni objekti koji se sastoje od drugih objekata, 
čime se pojednostavljuje rad sa hijerarhijskim strukturama.

### Gdje bismo ga mi koristili

U slučaju da imamo više tipova artikala koji se mogu grupisati u neku grupu.
Time bismo mogli omogućiti batch operacije nad grupom artikala, kao što su zaprimanje i obrađivanje artikala od strane kurirske službe.

# Flyweight pattern
Flyweight pattern se koristi kada želite da smanjite potrošnju memorije stvaranjem dijeljenih objekata.
Ovaj obrazac omogućava da se kreiraju objekti koji dijele zajedničke karakteristike, 
čime se smanjuje broj instanci koje se moraju čuvati u memoriji.

### Gdje bismo ga mi koristili

Možda bismo mogli da enum vrijednosti atributa čuvamo kao pointere na stvarne enum objekte.
Mada nismo sigurni da li je to podrazumijevano ponašanje u C#.






