function [sol, Fsol, h, NTiter, Error] = Newton(x0, MAXit, tol2)
    NTiter = 0;
    V = zeros(1,MAXit);
    x = x0;
    [F, FF, FFF] = CALCfun(x);
    Error = []
    while abs(FF) > tol2 && NTiter < MAXit
        x = double(x-FF/FFF);
        [F, FF, FFF] = CALCfun(x);
        NTiter = NTiter+1;
        V(NTiter) = x;
        if NTiter>1
            Error = [Error, abs(V(NTiter)-V(NTiter-1))];
        end
    end
    sol = x;
    Fsol = double(FF);

    h = Error(NTiter-1)
    
end


