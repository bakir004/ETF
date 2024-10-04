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
    assertz(posjecen(First)),
    findall(povezani(First, Grad, Distance), povezani(First, Grad, Distance), MoguceVeze),
    length(MoguceVeze, Count),
    format("Susjedne veze: ~d~n", [Count]),
    % Pocetak algoritma, imamo veze
    mst_step(MoguceVeze),

    % findall(mst(Grad1, Grad2, Distance), mst(Grad1, Grad2, Distance), MstVeze),

    % Brisanje najmanje veze
    % retract(povezani(Grad1, Grad2, Udaljenost)),
    % retract(povezani(Grad2, Grad1, Udaljenost)),

    % write('Nova lista'), nl,
    % print_susjedi(MoguceVeze2),

    print_posjeceni.

mst_step(MoguceVeze) :-
    length(MoguceVeze, Count),
    format("STEPPER: ~d~n", [Count]),

    % Pronalazak najmanje veze
    sortiraj_veze(MoguceVeze, SortiraneVeze),
    nth0(0, SortiraneVeze, NajblizaVeza),
    NajblizaVeza = povezani(Grad1, Grad2, Udaljenost),
    write('Najblizi grad: '),
    write(Grad1), nl,

    % Dodavanje veze u MST
    assertz(mst(Grad1, Grad2, Udaljenost)),
    assertz(posjecen(Grad2)),
    count_posjecen(CityCount),
    (CityCount >= 10 ->
        % Base case: Stop recursion if five cities have been visited
        format("Five cities visited. Stopping recursion.~n");
        % Recursive step: Continue recursion with updated connections list
        findall(povezani(Grad2, Nesta, Udaljenost), povezani(Grad2, Nesta, _), MoguceNoveVeze),
        append(MoguceVeze, MoguceNoveVeze, NovaListaMogucihVeza),
        delete(NovaListaMogucihVeza, povezani(Grad1, Grad2, _), UpdatedNovaListaMogucihVeza),
        delete(UpdatedNovaListaMogucihVeza, povezani(Grad2, Grad1, _), UpdatedNovaListaMogucihVeza2),

        findall(Veza, member(Veza, UpdatedNovaListaMogucihVeza2), MoguceVeze2),
        mst_step(MoguceVeze2)
    ).


print_posjeceni :-
    findall(X, posjecen(X), Posjeceni),
    forall(member(X, Posjeceni),
           format('Posjecen: ~w~n', [X])).

print_susjedi([]).
print_susjedi([Susjed|Rest]) :-
    write(Susjed), nl,  
    print_susjedi(Rest).

print_connections :-
    forall(povezani(Grad1, Grad2, Distance),
           format('Connection: ~w - ~w, Distance: ~2f~n', [Grad1, Grad2, Distance])).

print_mst_connections :-
    forall(mst(Grad1, Grad2, Distance),
           format('Connection: ~w - ~w, Distance: ~2f~n', [Grad1, Grad2, Distance])).
print_mst_size :-
    findall(_, posjecen(_), PosjecenList),
    length(PosjecenList, Count),
    format("Number: ~d~n", Count).

print_to_screen :-
    write('\nOK 200'), nl.

?- uvezi.
?- mst.
?- print_mst_connections.
?- print_to_screen.
