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

distance((_, X1, Y1), (_, X2, Y2), Distance) :-
    Distance is sqrt((X2 - X1)^2 + (Y2 - Y1)^2).

total_distance([], 0).
total_distance([_], 0). % Handle single city case
total_distance([City1, City2 | Rest], TotalDistance) :-
    distance(City1, City2, Distance),
    total_distance([City2 | Rest], RestDistance),
    TotalDistance is Distance + RestDistance.

update_min_distance(Path, Distance) :-
    ( min_distance(_, MinDist) ->
        ( Distance < MinDist ->
            retractall(min_distance(_, _)),
            assert(min_distance(Path, Distance))
        ; true
        )
    ; assert(min_distance(Path, Distance))
    ).

tsp_shortest_path(Cities, Path, Distance) :-
    permutation(Cities, Path),
    Path = [Start | _],
    append(Path, [Start], FullPath),
    total_distance(FullPath, Distance),
    update_min_distance(Path, Distance),
    fail.
tsp_shortest_path(_, Path, Distance) :- % Handle no solution case
    min_distance(Path, Distance). % Use the best path found so far

solve_tsp :-
    cities_coordinates(Cities),
    tsp_shortest_path(Cities, Path, Distance),
    format('Optimal Path: ~w~n', [Path]),
    format('Minimal Distance: ~w~n', [Distance]),
    retractall(min_distance(_, _)).

:- solve_tsp.
