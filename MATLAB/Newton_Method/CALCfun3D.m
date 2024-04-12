function [z,g,h] = CALCfun3D(x0,y0)

    fun = @(x,y) -1/exp(x^2+0.2*y^2);
    syms x y;
    f(x,y) = fun(x,y);
    %X=[x0;y0];
    z = double(f(x0,y0));
    
    grad(x,y) = gradient(f,[x,y]);
    g = double(grad(x0,y0));
    hess(x,y) = hessian(f,[x,y]);
    h = double(hess(x0,y0));
    

end
