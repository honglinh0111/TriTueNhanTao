cat(fubby).
dog(figaro).

spots_color(fubby,black).
spots_color(figaro,white).

owns(mary,Pet) :- cat(Pet), spots_color(Pet,black).
loves(Who,Pet) :- owns(Who,Pet).

%listing(cat).
%listing(owns).
%loves(Who,Pet).
%owns(mary,Pet).


