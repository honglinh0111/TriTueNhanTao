sendmoremoney(Vars) :-
    Vars = [Y,O,U,R,H,E,A,T],
    Vars ins 0..9,
    Y #\= 0,
    H #\= 0,
    all_different(Vars),
                 1000*Y + 100*O + 10*U + R
						+ 100*Y + 10*O + U
    #= 10000*H + 1000*E + 100*A + 10*R + T.