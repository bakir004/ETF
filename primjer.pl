koordinate_gradova([
    grad(sarajevo, 0, 8),
    grad(mostar, 10, 10),
    grad(banjaluka, 0, 15),
    grad(tuzla, 0, 3),
    grad(zagreb, 0, 0),
    grad(beograd, 4, 0),
    grad(podgorica, 4, 3),
    grad(skopje, 10, 0),
    grad(zenica, 10, 8),
    grad(split, 12, 0)
]).

daj_tezinsku_konstantu(Konstanta) :-
    Konstanta is 40.

generisi_varijacije(0, []).
generisi_varijacije(N, [X|Ostalo]) :-
    N > 0,
    member(X, [1, 3, 5]), 
    NoviN is N - 1,
    generisi_varijacije(NoviN, Ostalo).

mapiraj_vrijednosti([], [], _). 
mapiraj_vrijednosti([X|Xs], [Y|Ys], Map) :-
    member(X-Y, Map), 
    mapiraj_vrijednosti(Xs, Ys, Map). 

izvedi_mapiranje(UlaznaLista, IzlaznaLista) :-
    Preslikavanje = [1-0.1, 3-0.7, 5-1.2], 
    mapiraj_vrijednosti(UlaznaLista, IzlaznaLista, Preslikavanje). 

udaljenost_izmedju_gradova(Grad1, Grad2, Udaljenost) :-
    koordinate_gradova(Gradovi),
    member(grad(Grad1, X1, Y1), Gradovi),
    member(grad(Grad2, X2, Y2), Gradovi),
    Udaljenost is sqrt((X2 - X1)^2 + (Y2 - Y1)^2).

uvezi :-
    koordinate_gradova(Gradovi),
    findall((Grad1, Grad2), 
        (member(grad(Grad1, _, _), Gradovi), member(grad(Grad2, _, _), Gradovi), Grad1 \= Grad2), 
        Parovi),
    kreiraj_veze(Parovi).

kreiraj_veze([]).
kreiraj_veze([(Grad1, Grad2)|Ostalo]) :-
    udaljenost_izmedju_gradova(Grad1, Grad2, Udaljenost),
    assertz(povezani(Grad1, Grad2, Udaljenost)),
    assertz(povezani(Grad2, Grad1, Udaljenost)),
    kreiraj_veze(Ostalo).

uporedi_udaljenosti(Red, povezani(_, _, Udaljenost1), povezani(_, _, Udaljenost2)) :-
    compare(Red, Udaljenost1, Udaljenost2).

sortiraj_veze(Veze, SortiraneVeze) :-
    predsort(uporedi_udaljenosti, Veze, SortiraneVeze).

izbroj_posjecene(BrojGradova) :-
    findall(Grad, posjecen(Grad), Gradovi),
    length(Gradovi, BrojGradova).

mst :-
    koordinate_gradova([grad(Prvi,_,_) | _]),
    assertz(tuple(0, 0, 0)),
    assertz(posjecen(Prvi)),
    findall(povezani(Prvi, Grad, Udaljenost), povezani(Prvi, Grad, Udaljenost), MoguceVeze),
    % Pocetak algoritma, imamo veze
    mst_korak(MoguceVeze).

nisu_posjeceni(povezani(Grad1, Grad2, _)) :-
    (\+ posjecen(Grad1) ; \+ posjecen(Grad2)).

mst_korak(MoguceVeze) :-
    % Pronalazak najmanje veze
    sortiraj_veze(MoguceVeze, SortiraneVeze),
    findall(Veza, (
       member(Veza, SortiraneVeze),
       nisu_posjeceni(Veza)
    ), FiltriraneVeze),
    nth0(0, FiltriraneVeze, NajblizaVeza),

    NajblizaVeza = povezani(Grad1, Grad2, Udaljenost),

    % Dodavanje veze u MST
    assertz(mst(Grad1, Grad2, Udaljenost)),
    (posjecen(Grad2) ->  true ; assertz(posjecen(Grad2))),
    koordinate_gradova(Gradovi),
    izbroj_posjecene(BrojPosjecenih),
    length(Gradovi, Velicina),
    (BrojPosjecenih >= Velicina ->
        format("Svi gradovi povezani"), nl;
        findall(povezani(Grad2, Nesta, Udaljenost2), povezani(Grad2, Nesta, Udaljenost2), MoguceNoveVeze),
        append(MoguceVeze, MoguceNoveVeze, NovaListaMogucihVeza),

        delete(NovaListaMogucihVeza, povezani(Grad1, Grad2, _), UpdatedNovaListaMogucihVeza),
        delete(UpdatedNovaListaMogucihVeza, povezani(Grad2, Grad1, _), UpdatedNovaListaMogucihVeza2),

        findall(Veza, member(Veza, UpdatedNovaListaMogucihVeza2), MoguceVeze2),
        mst_korak(MoguceVeze2)
    ).


print_posjeceni :-
    findall(X, posjecen(X), Posjeceni),
    forall(member(X, Posjeceni),
           format('Posjecen: ~w~n', [X])).

print_veze([]).
print_veze([Veza|Ostalo]) :-
    write(Veza), nl,
    Veza = putevi(_, Cijena, Propusnost),
    tezinska_funkcija(Cijena, Propusnost, _),
    % write(Tezina), write(' '),
    print_veze(Ostalo).

print_mst_veze :-
    forall(mst(Grad1, Grad2, Udaljenost),
           format('Veza: ~w - ~w, Udaljenost ~2f~n', [Grad1, Grad2, Udaljenost])).

krajnja_poruka :-
    write('\nKraj programa'), nl.

generisi_tipove_veza([],[],[]).
generisi_tipove_veza([D1|Udaljenosti], [M1|MultiplikatoriCijene], [R1|MultiplikatoriPropusnosti]) :-
    assertz(tuple(D1, M1, R1)),
    generisi_tipove_veza(Udaljenosti, MultiplikatoriCijene, MultiplikatoriPropusnosti).

proizvod_udaljenosti_cijene(tuple(D, P, _), Product) :-
    Product is D * P.

izmnozi_udaljenosti_cijene(Sum) :-
    findall(Product, (tuple(D, P, _), proizvod_udaljenosti_cijene(tuple(D, P, _), Product)), Products),
    sumlist(Products, Sum).

proizvod_udaljenosti_propusnosti(tuple(D, _, R), Product) :-
    Faktor is D ** 2,
    Product is Faktor * R.

izmnozi_udaljenosti_propusnosti(Sum) :-
    findall(Product, (tuple(D, _, R), proizvod_udaljenosti_propusnosti(tuple(D, _, R), Product)), Products),
    sumlist(Products, Sum).

generisi_puteve(Udaljenosti, Varijacije, FaktoriPropusnosti) :-
    izmnozi_udaljenosti_cijene(Cijena),
    izmnozi_udaljenosti_propusnosti(Prolaznost),
    assertz(putevi(Varijacije, Cijena, Prolaznost)),
    retractall(tuple(_,_,_)),
    assertz(tuple(0, 0, 0)),
    generisi_tipove_veza(Udaljenosti, Varijacije, FaktoriPropusnosti).

probaj_sve_puteve :-
    write('Pronalazim najbolje tipove puteva...'), nl, nl,
    write('Ignorisati error ispod...\n'),
    findall(D, mst(_, _, D), Udaljenosti),
    length(Udaljenosti, Broj),
    generisi_varijacije(Broj+1, Varijacija), 
    izvedi_mapiranje(Varijacija, Mapirano),
    generisi_puteve(Udaljenosti, Varijacija, Mapirano).

tezinska_funkcija(Cijena, Propusnost, Tezina) :-
    Cijena =\= 0,
    daj_tezinsku_konstantu(Konstanta),
    Tezina is Konstanta / Cijena * Propusnost.
tezinska_funkcija(0, _, 0).

uporedi_puteve(Red, putevi(_, Cijena1, Propusnost1), putevi(_, Cijena2, Propusnost2)) :-
    tezinska_funkcija(Cijena1, Propusnost1, Tezina1),
    tezinska_funkcija(Cijena2, Propusnost2, Tezina2),
    compare(Red, Tezina1, Tezina2).

ukloni_zadnji_element([_], []).  
ukloni_zadnji_element([H|T], [H|NoviT]) :- 
    ukloni_zadnji_element(T, NoviT).

pronadji_najbolji_put :-
    findall(putevi(A, B, C), putevi(A, B, C), Putevi),
    predsort(uporedi_puteve, Putevi, [_|SortiraniPutevi]),
    % print_veze(SortiraniPutevi),
    length(SortiraniPutevi, Duzina),
    ZadnjiIndeks is Duzina - 1,
    nth0(ZadnjiIndeks, SortiraniPutevi, Najbolji),
    Najbolji = putevi(Niz, _, _),
    ukloni_zadnji_element(Niz, KonacniNiz),
    write('Najbolji odabir puteva: '), write(KonacniNiz), nl.

?- uvezi.
?- mst.
?- print_mst_veze.
?- probaj_sve_puteve.
?- pronadji_najbolji_put.
?- krajnja_poruka.

