men(john).
men(fred).
men(harry).
women(mary).
women(julie).
women(susan).
women(anne).
hair(john,blonde).
hair(fred,dark).
hair(harry,dark).
hair(julie,blonde).
hair(susan,blonde).
hair(mary,brunette).
hair(anne,brunette).
rich(X) :- have(X,gold).
have(fred,gold).
have(julie,gold).
have(anne,house).
have(john,car).
like(john,X) :- rich(X), hair(X,blonde), women(X).
like(harry,X) :- rich(X), women(X).
like(fred,X) :- hair(X,brunette), women(X).
like(mary,X) :- hair(X,dark), men(X).
like(julie,X) :- rich(X), hair(X,dark), men(X).
like_each_other(X,Y) :- like(X,Y), like(Y,X).
 