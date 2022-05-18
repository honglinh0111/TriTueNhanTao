%movie(american_beauty, X).
%movie(Y,2000).
%movie(X, Y),(Y<2000).
%movie(X, Y),(Y>1990).
%actor(M1, A, _), actor(M2, A, _), M1 @> M2.
%director(M, D), actress(M, scarlett_johansson, _).
%actor(M,A,_),director(M,A).
%(actor(_,A,_);actress(_,A,_)),director(_,A).
%actor(M,john_goodman,_),actor(M,jeff_bridges,_).