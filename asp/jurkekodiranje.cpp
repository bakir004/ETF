// Implementacija postupaka za statističko kodiranje.
// (C) 2019 by Željko Jurić, ETF Sarajevo

// Eventualne bagove možete prijavljivati na email zjuric@etf.unsa.ba

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

// Neki korisni aliasi...

typedef std::vector<double> RealVector;
typedef std::vector<std::string> StringVector;

// Funkcija "ShannonFano" konstruira binarni Shannon-Fano kod za poruke čije
// su vjerovatnoće ili učestanosti pojavljivanja (prihvata se oboje) zadane u
// vektoru "p". Kao rezultat dobija se vektor odgovarajućih kodova u vidu
// stringova koji se sastoje samo od simbola '0' i '1', pri čemu "i"-ti
// kod odgovara poruci čija se vjerovatnoća ili učestanost pojavljivanja
// nalazi na "i"-toj poziciji u vektoru "p". Algoritam za konstrukciju koda
// implementiran je rekurzivno, pri čemu je unutar ove funkcije definirana
// i pomoćna lokalna funkcija "DoRec" koja se poziva rekurzivno.

StringVector ShannonFano(const RealVector &p) {
  int n = p.size();
  std::vector<std::pair<double, int>> t;
  for(int i = 0; i < n; i++) t.emplace_back(-p[i], i);
  std::sort(t.begin(), t.end());
  StringVector c(n);
  std::function<void(int, int)> DoRec = [&c, &t, &DoRec](int u, int v) {
    if(v - u <= 1) return;
    int lp = v, hp = u;
    double ls = 0, hs = 0;
    while(hp != lp)
      if(hs > ls) {
        ls -= t[--lp].first; c[t[lp].second] += '1';
      }
      else {
        hs -= t[hp].first; c[t[hp++].second] += '0';
      }
    DoRec(u, lp); DoRec(lp, v);
  };
  DoRec(0, n);
  return c;
}

// Funkcija "Huffman" konstruira "m"-arni Huffmanov kod za poruke čije su
// vjerovatnoće ili učestanosti pojavljivanja (prihvata se oboje) zadane u
// vektoru "p", dok se broj simbola zadaje putem parametra "m". Kao rezultat
// dobija se vektor odgovarajućih kodova u vidu stringova koji se sastoje od
// simbola '0', '1', '2' itd. Pri tome, "i"-ti kod odgovara poruci čija se
// vjerovatnoća ili učestanost pojavljivanja nalazi na "i"-toj poziciji u
// vektoru "p". Implementiran je optimalni algoritam za konstrukciju koji radi
// u vremenu "O(n log n)" gdje je "n" broj poruka, a koji se zasniva na
// konstrukciji odgovarajućeg stabla i strukturi podataka poznatoj kao hrpa
// ili gomila (engl. heap) za efikaanu manipulaciju, Dinamička alokacija
// memorije i pokazivači su izbjegnuti tako što se čvorovi stabla čuvaju u
// vektoru "tree", a umjesto pokazivača na roditeljski čvor u čvoru se čuva
// odgovarajući indeks roditeljskog čvora u vektoru. Za realizaciju hrpe
// iskorištene su odgovarajuće manje poznate funkcije iz biblioteke
// "algorithm". Konkretno, funkcija "make_heap" reorganizira u vremenu "O(n)"
// zadani opseg u hrpu, tako da se "najbolji" element u hrpi nalazi na njenom
// početku (vrhu), pri čemu se kao posljednji parametar zadaje funkcija
// kriterija koja vraća "true" ako i samo ako je njen drugi argument "bolji"
// od prvog (u odsustvu funkcije kriterija, "bolji" znači "veći"). Funkcija
// "push_heap" vrši reorganizaciju hrpe nakon što se u nju doda novi element,
// dok funkcija "pop_heap" vrši prebacivanje "najboljeg" elementa na kraj
// hrpe, te reorganizira ostatak hrpe tako da "sljedeći najbolji" element dođe
// na vrh (početak) hrpe. Obje ove funkcije rade u vremenu "O(log n)".

StringVector Huffman(const RealVector &p, int m) {
  struct Node { double p; int code; int lnk; };
  int n = p.size();
  int mstar = 2 + (n - 2) % (m - 1);
  std::vector<Node> tree;
  std::vector<int> heap;
  auto comp = [&tree](int i, int j) { return tree[i].p > tree[j].p
    || std::fabs(tree[i].p - tree[j].p) < 1e-10 && i < j; };
  for(int i = 0; i < n; i++) {
    tree.push_back({p[i], 0, -1}); heap.push_back(i);
  }
  std::make_heap(heap.begin(), heap.end(), comp);
  while(heap.size() > 1) {
    double s = 0;
    for(int j = mstar - 1; j >= 0; j--) {
      int q = heap[0];
      s += tree[q].p; tree[q].code = j; tree[q].lnk = tree.size();
      std::pop_heap(heap.begin(), heap.end(), comp);
      heap.pop_back();
    }
    heap.push_back(tree.size()); tree.push_back({s, 0, -1});
    std::push_heap(heap.begin(), heap.end(), comp);
    mstar = m;
  }
  StringVector c(n);
  for(int i = 0; i < n; i++)
    for(int cur = i; tree[cur].lnk != -1; cur = tree[cur].lnk)
      c[i] = char('0' + tree[cur].code) + c[i];
  return c;
}

// Funkcija "SequenceProbabilities" prima kao parametar vektor vjerovatnoća
// poruka "p", a vraća kao rezultat vektor vjerovatnoća svih sekvenci poruka
// dužine "k", uz pretpostavku da je izvor bez memorije, odnosno da vrijedi
// relacija "p(xixj)=p(xi)p(xj)". Za sekvence se smatra da su sortirane u
// leksikografski poredak u odnosu na poredak izvornih poruka, odnosno ukoliko
// su "x1", "x2", ..., "xn" izvorne poruke, sekvence poruka su redom
// "x1x1x1...x1x1", "x1x1x1...x1x2", ..., "x1x1x1...x1xn", "x1x1x1...x2x1",
// ..., "x1x1x1...x2xn", ..., // "x1x1x2...x1x1", ..., "xnx1x1...x1x1", ...,
// "xnxnxn...xnxn". Ova funkcija je korisna ukoliko želimo kodirati sekvence
// poruka umjesto individualnih poruka.

RealVector SequenceProbabilities(const RealVector &p, int k) {
  int n = p.size(), ns = 1;
  for(int i = 1; i <= k; i++) ns *= n;
  RealVector ps;
  for(int i = 0; i < ns; i++) {
    double q = 1;
    int t = i;
    for(int j = 0; j < k; j++) {
      q *= p[t % n]; t /= n;
    }
    ps.push_back(q);
  }
  return ps;
}

// Funkcija "EntropyNormalized" nalazi entropiju skupa poruka čije su
// vjerovatnoće ili učestanosti pojavljivanja date u vektoru "p" (prihvata se
// oboje). Ukoliko je suma svih elemenata u vektoru "p" jednaka jedinici,
// smatra se da su zadane vjerovatnoće, a u suprotnom se smatra da su zadane
// učestanosti pojavljivanja poruka.

double EntropyNormalized(const RealVector &p) {
  double ptot = 0, sum = 0;
  for(double e : p) {
    ptot += e;
    if(e != 0) sum += e * std::log(e);
  }
  return (std::log(ptot) - sum / ptot) / std::log(2);
}

// Funkcija "AverageCodeLength" nalazi srednju dužinu kodne riječi za poruke
// čije su vjerovatnoće ili učestanosti pojavljivanja date u vektoru "p"
// (prihvata se oboje), dok su odgovarajuće kodne riječi u vektoru "c".
// Ukoliko je suma svih elemenata u vektoru "p" jednaka jedinici, smatra se da
// su zadane vjerovatnoće, a u suprotnom se smatra da su zadane učestanosti
// pojavljivanja poruka.

double AverageCodeLength(const RealVector &p, const StringVector &c) {
  double ptot = 0, sum = 0;
  for(int i = 0; i < p.size(); i++) {
    ptot += p[i];
    sum += p[i] * c[i].length();
  }
  return sum / ptot;
}

// "GetInt" je pomoćna funkcija namijenjena za unos cijelih brojeva sa
// tastature, uz kontrolu grešaka pri unosu. Uneseni broj se smješta u
// parametar "x", a sama funkcija vraća kao rezultat "true" ili "false",
// ovisno da li je unos bio uspješan ili ne.

bool GetInt(int &x) {
  std::cin >> x;
  if(std::cin && std::cin.peek() == '\n') return true;
  std::cin.clear(); std::cin.ignore(10000, '\n');
  return false;
}

// "GetReal" je pomoćna funkcija namijenjena za unos realnih brojeva sa
// tastature, uz kontrolu grešaka pri unosu. Također, podržano je i da se
// brojevi mogu unositi u formi razlomaka, kao "p/q". Uneseni broj se smješta
// u parametar "x", a sama funkcija vraća kao rezultat "true" ili "false",
// ovisno da li je unos bio uspješan ili ne.

bool GetReal(double &x) {
  std::cin >> x;
  if(std::cin.peek() == '/') {
    double y;
    std::cin.get();
    std::cin >> y;
    x /= y;
  }
  if(std::cin && std::cin.peek() == '\n') return true;
  std::cin.clear(); std::cin.ignore(10000, '\n');
  return false;
}

// "DisplaySequence" je pomoćna funkcija za ispis sekvenci poruka. Parametar
// "k" predstavlja dužinu poruke, "n" je broj različitih poruka koje se
// koriste za formiranje sekvenci, dok je "i" indeks (redni broj) sekvence u
// leksikografskom poretku, numeriran od nule nadalje. Tako sekvenca
// "x1x1x1...x1x1" ima indeks "0", sekvenca "x1x1x1...x1x2" indeks "1",
// sekvenca "x1x1x1...x1xn" indeks "n-1", sekvenca "x1x1x1...x2x1" indeks "n",
// itd. Rad funkcije zasniva se na činjenici da ukoliko indeks "i" rastavimo
// na cifre u bazi "m", individualne cifre predstavljaju redne brojeve
// individualnih poruka, numeriranih od "0" nadalje.

void DisplaySequence(int i, int n, int k) {
  int r = 0;
  for(int j = 1; j <= k; j++) {
    r = n * r + i % n; i /= n;
  }
  for(int j = 1; j <= k; j++) {
    std::cout << "x" << r % n + 1;
    r /= n;
  }
}

// Demonstracioni program...

int main() {
  int opt, m = 2, n, k;
  std::cout << "STATISTICKO KODIRANJE\n\n"
    << "1. Shannon-Fano kod\n"
    << "2. Huffmanov kod\n\n"
    << "Odaberite opciju: ";
  GetInt(opt);
  std::cout << std::endl << "Broj poruka: ";
  GetInt(n);
  if(opt == 2) {
    std::cout << "Broj simbola: ";
    GetInt(m);
  }
  std::cout << std::endl;
  std::cout << "Duzina sekvence (1 ako se kodiraju individualne poruke): ";
  GetInt(k);
  std::cout << std::endl << "Unesite vjerovatnoce ili ucestanosti "
    << "pojavljivanja poruka\n(vjerovatnoce se mogu zadavati i kao "
    << "razlomci u formatu p/q):" << std::endl << std::endl;
  RealVector p(n);
  for(int i = 0; i < n; i++) {
    std::cout << "p(x" << i + 1 << ")=";
    GetReal(p[i]);
  }
  std::cout << std::endl;
  p = SequenceProbabilities(p, k);
  StringVector c;
  if(opt == 2) c = Huffman(p, m);
  else c = ShannonFano(p);
  for(int i = 0; i < c.size(); i++) {
    DisplaySequence(i, n, k);
    std::cout << " -> " << c[i] << std::endl;
  }
  std::cout << std::endl;
  double H = EntropyNormalized(p);
  double navg = AverageCodeLength(p, c);
  double I = H / navg, Imax = std::log(m) / std::log(2);
  std::cout << "Entropija skupa poruka (sekvence): " << H << std::endl
    << "Srednja duzina kodne rijeci: " << navg << std::endl
    << "Brzina prenosa informacija: " << I << "/tau" << std::endl
    << "Efikasnost kodiranja: " << 100 * I / Imax << "%" << std::endl;
}
