% example.pl
% A simple Prolog program

likes(john, pizza).
likes(mary, sushi).
likes(jane, pizza).
likes(jane, sushi).

friend(X, Y) :- likes(X, Z), likes(Y, Z), X \= Y.
