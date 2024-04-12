f = @(x) -x*exp(-0.2*x);
[xsol2, ysol2] = fminbnd(f,0,100)

