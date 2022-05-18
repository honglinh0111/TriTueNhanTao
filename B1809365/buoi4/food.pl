food(ga).
food(tao).
food(X) :-  eat(Y,X), consong(Y), !.
consong(bill).
eat(bill,dauphong).
eat(john,X) :- food(X).
eat(sue,X) :- eat(bill,X).