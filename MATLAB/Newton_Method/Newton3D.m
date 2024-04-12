function [sol, Fsol, error, NTiter, V, Error] = Newton3D(x0, y0, MAXit, tol)
    NTiter = 1;
    V = zeros(3, MAXit);
    x = x0;
    y = y0;
    [z, g, h] = CALCfun3D(x,y);
    Error = [];
    while norm(g) > tol && NTiter < MAXit
        V(1,NTiter) = x;
        V(2,NTiter) = y;
        V(3,NTiter) = z;
        NTiter = NTiter+1;

        var = (inv(h)*g).';
        x = x-var(1);
        y = y-var(2);
        [z,g,h] = CALCfun3D(x,y);

        if NTiter > 1
            Error = [Error, norm([V(1,NTiter),V(2,NTiter)]-[V(1,NTiter-1),V(2,NTiter-1)])];
    
        end
    end
    sol = [x,y];
    Fsol = norm(g);
    error = Error(NTiter-1)

    
end