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

road_costs(autoput, 5).
road_costs('brza cesta', 3).
road_costs(makadam, 1).

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

prim_mst([StartCity|Cities], MST) :-
    prim_mst_helper([(0, StartCity, StartCity)], Cities, [], MST).

prim_mst_helper(_, [], MST, MST).
prim_mst_helper(Edges, Cities, Tree, MST) :-
    select((Cost, U, V), Edges, RestEdges),
    \+ member((_, V, _), Tree), 
    findall((D, V, W), (member(W, Cities), povezani(V, W, D)), NewEdges), 
    append(RestEdges, NewEdges, UpdatedEdges), 
    sort(UpdatedEdges, SortedEdges), 
    prim_mst_helper(SortedEdges, Cities, [(Cost, U, V) | Tree], MST). 

generate_and_print_mst :-
    cities_coordinates(CitiesList),
    findall(City, member(grad(City, _, _), CitiesList), Cities), % Get all cities
    prim_mst(Cities, MST), % Generate the MST
    forall(member((Cost, U, V), MST), % Print each edge in the MST
           format('Edge: ~w - ~w, Cost: ~2f~n', [U, V, Cost])).

print_connections :-
    forall(povezani(Grad1, Grad2, Distance),
           format('Connection: ~w - ~w, Distance: ~2f~n', [Grad1, Grad2, Distance])).
generate_and_print_connections :-
    uvezi,
    generate_and_print_mst.

?- generate_and_print_connections.
