:- initialization(main).

main :-
    print_all_connections_with_costs_and_total,
    halt.

grad(sarajevo, 3, 5).
grad(mostar, 9, 2).
grad(banjaluka, 7, 10).
grad(tuzla, 12, 8).
grad(zagreb, 5, 15).
grad(beograd, 11, 12).
grad(podgorica, 6, 3).
grad(skopje, 8, 6).
grad(zenica, 10, 13).
grad(split, 4, 7).

povezani(sarajevo, split, 'autoput').
povezani(sarajevo, skopje, 'brza cesta').
povezani(tuzla, skopje, 'brza cesta').
povezani(podgorica, skopje, 'brza cesta').
povezani(mostar, skopje, 'brza cesta').
povezani(banjaluka, skopje, 'brza cesta').
povezani(banjaluka, beograd, 'brza cesta').
povezani(banjaluka, zenica, 'makadam').
povezani(zagreb, zenica, 'makadam').

udaljenost_izmedju_gradova(Grad1, Grad2, Udaljenost) :-
    grad(Grad1, X1, Y1),
    grad(Grad2, X2, Y2),
    Udaljenost is sqrt((X2 - X1)^2 + (Y2 - Y1)^2).

cena_za_put(autoput, Distance, Cost) :-
    Cost is 5 * Distance.
cena_za_put('brza cesta', Distance, Cost) :-
    Cost is 3 * Distance.
cena_za_put(makadam, Distance, Cost) :-
    Cost is 1 * Distance.

cena_za_vezu(Grad1, Grad2, Cena) :-
    povezani(Grad1, Grad2, TipPuta),
    udaljenost_izmedju_gradova(Grad1, Grad2, Udaljenost),
    cena_za_put(TipPuta, Udaljenost, Cena).

print_all_connections_with_costs_and_total :-
    forall(povezani(Grad1, Grad2, TipPuta),
           (cena_za_vezu(Grad1, Grad2, Cena),
            format('Connection: ~w - ~w, Type: ~w, Cost: ~w~n', [Grad1, Grad2, TipPuta, Cena]))),
    suma_svih_troskova(Suma),
    format('Total cost for all connections: ~w~n', [Suma]).

suma_svih_troskova(Suma) :-
    findall(Cena, cena_za_vezu(_, _, Cena), ListeCena),
    sum_list(ListeCena, Suma).
