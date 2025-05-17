# SOLID principi dijagrama klasa

## S - single responsibility princip

*Svaka metoda/klasa mora imati samo jednu odgovornost*

Svaka klasa je cjelina za sebe, i svaka metoda unutar pojedine klase ima jednu svrhu.
Svaka metoda unutar klasa poštuje objektno orijentisanu paradigmu u smislu da se programer preko te metode obraća instanci klase nad kojom se ista metoda nalazi.
Ovo je razlog postojanja klase MailService, kako bi se funkcionalnost slanja mejla tretirala kao propratna pojava, i ne kao odgovornost ostalih funkcija koji je mogu pozvati kao neuspojavu (side effect).

## O - open/closed princip

*Klase moraju biti otvorene za proširenja, a zatvorene za modifikaciju*

Klase sadržavaju osnovne atribute i metode, čime su lahke za proširiti.
Primjer toga su enumeracije, koje je lahko proširiti dodatnim ulogama, statusima ili stanjima.

## L - Liskov substitution princip

*Izvedene klase se trebaju moći proslijediti svugdje gdje može i bazna*

Postojanje generalizacijske veze između admina, kurirske službe i korisnika nam to dozvoljava.
Primjer toga je funkcionalnost praćenja korisnika, što je radnja nad korisnicima, ali se može primijeniti na kurirske službe i čak i admina.

## I - Interface segregation princip

*Klijent ne treba ovisiti od interfejsa sa metodama koje nikada neće koristiti*

Kurirska služba je jedina klasa koja će implementirati slanje maila, pa nema potrebe da ostale klase znaju i vežu se za MailServis.
Isto tako, umjesto da čitav korisnik implementira funkcije obrade narudžbi, odvojeno su te metode implementirane u klasi kurirske službe.
Na taj način je izbjegnuto pretrpavanje korisnika nepotrebnim funkcionalnostima.

## D - Dependency inversion princip

*Umjesto da konkretne klase ovise o konkretnim klasama, trebaju ovisiti o interfejsima ili apstraktnim klasama*

Ovim se postiže decoupling klasa i olakšava povezivanje funkcionalnosti.
Ovo se jedino na našem primjeru može vidjeti na klasi MailServis.
Ovdje se samo exposea funkcionalnost slanja maila, bez ulaženja u detalje izvedbe slanja maila i mail providera koji se može koristiti.
Također kao primjer open/closed principa, mail provider se lahko može dodati pored postojećih, i promijeniti vrlo lahko, ako se stvari lijepo implementiraju.



