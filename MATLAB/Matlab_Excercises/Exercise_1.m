f = [-200 -600];

A = [1 1; 1 -1 ; 1 4];

b = [80 30 160];

lb = zeros(1,2);

[xsol , Fsol] = linprog(f, A, b, [], [], lb)
