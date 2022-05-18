owns(jack,car(bmw)).
owns(john,car(chevy)).
owns(olivia,car(civic)).
owns(jane,car(chevy)).

sedan(car(bmw)).
sedan(car(civic)).
truck(car(chevy)).

%owns(john,X).

%owns(john,_).

%owns(Who,car(chevy)).

%sedan(owns(jane,car(chevy))).

%owns(jane,X),sedan(X).

%owns(jane,X),truck(X).