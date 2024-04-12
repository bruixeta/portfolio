
function [F, FF, FFF] = CALCfun(X)
    fun = @(x) 1/exp(x^2);
    syms x;

    f(x) = fun(x);
    F = f(X);

    ff(x) = diff(f);
    FF = ff(X);

    fff(x) = diff(ff);
    FFF = fff(X);
end
