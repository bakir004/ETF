cities_coordinates([
    grad(sarajevo, 3, 5),
    grad(mostar, 9, 2),
    grad(banjaluka, 7, 10),
    grad(tuzla, 12, 8),
    grad(zagreb, 5, 15),
    grad(beograd, 11, 12),
    grad(podgorica, 6, 3),
    grad(skopje, 8, 6),
    grad(zenica, 10, 13),
    grad(split, 4, 7)
]).

daj_tezinsku_konstantu(Konstanta) :-
    Konstanta is 40.

generate_variations(0, []).
generate_variations(N, [X|Rest]) :-
    N > 0,
    member(X, [1, 3, 5]), 
    NewN is N - 1,
    generate_variations(NewN, Rest).

map_values([], [], _). 
map_values([X|Xs], [Y|Ys], Map) :-
    member(X-Y, Map), 
    map_values(Xs, Ys, Map). 

perform_mapping(InputList, OutputList) :-
    Mapping = [1-0.1, 3-0.7, 5-1.2], 
    map_values(InputList, OutputList, Mapping). 

udaljenost_izmedju_gradova(Grad1, Grad2, Udaljenost) :-
    cities_coordinates(Cities),
    member(grad(Grad1, X1, Y1), Cities),
    member(grad(Grad2, X2, Y2), Cities),
    Udaljenost is sqrt((X2 - X1)^2 + (Y2 - Y1)^2).

uvezi :-
    cities_coordinates(Cities),
    findall((Grad1, Grad2), 
        (member(grad(Grad1, _, _), Cities), member(grad(Grad2, _, _), Cities), Grad1 \= Grad2), 
        Pairs),
    create_connections(Pairs).

create_connections([]).
create_connections([(Grad1, Grad2)|Rest]) :-
    udaljenost_izmedju_gradova(Grad1, Grad2, Distance),
    assertz(povezani(Grad1, Grad2, Distance)),
    assertz(povezani(Grad2, Grad1, Distance)),
    create_connections(Rest).

compare_distances(Order, povezani(_, _, Distance1), povezani(_, _, Distance2)) :-
    compare(Order, Distance1, Distance2).
sortiraj_veze(Susjedi, SortedSusjedi) :-
    predsort(compare_distances, Susjedi, SortedSusjedi).
count_posjecen(CityCount) :-
    findall(City, posjecen(City), Cities),
    length(Cities, CityCount).

mst :-
    cities_coordinates([grad(First,_,_) | _]),
    assertz(tuple(0, 0, 0)),
    assertz(posjecen(First)),
    findall(povezani(First, Grad, Distance), povezani(First, Grad, Distance), MoguceVeze),
    % Pocetak algoritma, imamo veze
    mst_step(MoguceVeze).

not_visited(povezani(Grad1, Grad2, _)) :-
    (\+ posjecen(Grad1) ; \+ posjecen(Grad2)).

mst_step(MoguceVeze) :-
    % Pronalazak najmanje veze
    sortiraj_veze(MoguceVeze, SortiraneVeze),
    findall(Veza, (
       member(Veza, SortiraneVeze),
       not_visited(Veza)
    ), FiltriraneVeze),
    nth0(0, FiltriraneVeze, NajblizaVeza),

    NajblizaVeza = povezani(Grad1, Grad2, Udaljenost),

    % Dodavanje veze u MST
    assertz(mst(Grad1, Grad2, Udaljenost)),
    (posjecen(Grad2) ->  true ; assertz(posjecen(Grad2))),
    cities_coordinates(Cities),
    count_posjecen(CityCount),
    length(Cities, Size),
    (CityCount >= Size ->
        format("Svi gradovi povezani"), nl;
        findall(povezani(Grad2, Nesta, Udaljenost2), povezani(Grad2, Nesta, Udaljenost2), MoguceNoveVeze),
        append(MoguceVeze, MoguceNoveVeze, NovaListaMogucihVeza),
        % write('Nova lista: '),
        % print_veze(NovaListaMogucihVeza),

        delete(NovaListaMogucihVeza, povezani(Grad1, Grad2, _), UpdatedNovaListaMogucihVeza),
        delete(UpdatedNovaListaMogucihVeza, povezani(Grad2, Grad1, _), UpdatedNovaListaMogucihVeza2),

        findall(Veza, member(Veza, UpdatedNovaListaMogucihVeza2), MoguceVeze2),
        mst_step(MoguceVeze2)
    ).


print_posjeceni :-
    findall(X, posjecen(X), Posjeceni),
    forall(member(X, Posjeceni),
           format('Posjecen: ~w~n', [X])).

print_veze([]).
print_veze([Veza|Rest]) :-
    write(Veza), nl,
    Veza = putevi(_, Cijena, Propusnost),
    weight_function(Cijena, Propusnost, Tezina),
    write(Tezina), write(' '),
    print_veze(Rest).

print_connections :-
    forall(povezani(Grad1, Grad2, Distance),
           format('Connection: ~w - ~w, Distance: ~2f~n', [Grad1, Grad2, Distance])).

print_mst_connections :-
    forall(mst(Grad1, Grad2, Distance),
           format('Veza: ~w - ~w, Udaljenost ~2f~n', [Grad1, Grad2, Distance])).
print_mst_size :-
    findall(_, posjecen(_), PosjecenList),
    length(PosjecenList, Count),
    format("Number: ~d~n", Count).

print_to_screen :-
    write('\nOK 200'), nl.

weighted_sum([],[],[]).
weighted_sum([D1|Distances], [M1|Multipliers], [R1|Reducers]) :-
    assertz(tuple(D1, M1, R1)),
    weighted_sum(Distances, Multipliers, Reducers).

product_of_d_and_p(tuple(D, P, _), Product) :-
    Product is D * P.

calculate_sum_of_products(Sum) :-
    findall(Product, (tuple(D, P, _), product_of_d_and_p(tuple(D, P, _), Product)), Products),
    sumlist(Products, Sum).

product_of_d_and_r(tuple(D, _, R), Product) :-
    Faktor is D ** 2,
    Product is Faktor * R.

calculate_sum_of_products2(Sum) :-
    findall(Product, (tuple(D, _, R), product_of_d_and_r(tuple(D, _, R), Product)), Products),
    sumlist(Products, Sum).

poziv(Udaljenosti, Varijacije, Reduceri) :-
    calculate_sum_of_products(Cijena),
    calculate_sum_of_products2(Prolaznost),
    assertz(putevi(Varijacije, Cijena, Prolaznost)),
    retractall(tuple(_,_,_)),
    assertz(tuple(0, 0, 0)),
    weighted_sum(Udaljenosti, Varijacije, Reduceri).

print_all_variations :-
    write('Pronalazim najbolje tipove puteva...'), nl,
    findall(D, mst(_, _, D), Udaljenosti),
    generate_variations(10, Variation),
    perform_mapping(Variation, Result),
    poziv(Udaljenosti, Variation, Result).

weight_function(Cost, Throughput, Weight) :-
    Cost =\= 0,
    daj_tezinsku_konstantu(Konstanta),
    Weight is Konstanta / Cost * Throughput.
weight_function(0, _, 0).

compare_putevi(Order, putevi(_, Cost1, Throughput1), putevi(_, Cost2, Throughput2)) :-
    weight_function(Cost1, Throughput1, Weight1),
    weight_function(Cost2, Throughput2, Weight2),
    compare(Order, Weight1, Weight2).

print_paths :-
    findall(putevi(A, B, C), putevi(A, B, C), Putevi),
    predsort(compare_putevi, Putevi, [_|SortedPutevi]),
    % print_veze(SortedPutevi),
    length(SortedPutevi, Len),
    LastIndex is Len - 1,
    nth0(LastIndex, SortedPutevi, Najbolji),
    Najbolji = putevi(Niz, _, _),
    write('Najbolji odabir puteva: '), write(Niz), nl.

?- uvezi.
?- mst.
?- print_mst_connections.
?- print_all_variations.
?- print_paths.
?- print_to_screen.

