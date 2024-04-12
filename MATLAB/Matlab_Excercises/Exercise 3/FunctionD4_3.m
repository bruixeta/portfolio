function[fx] = FUN(xv)
    p1 = xv(4);
    p2 = xv(5);
    p3 = xv(6);
    C1 = 3*(p1)^1.3;
    C2 = 5*(p2)^1.2;
    C3 = 6*(p3)^1.15;
    V1 = 12-p1;
    V2 = 20-1.5*p2;
    V3 = 28-2.5*p3;

    fx = - p1*(V1) - p2*(V2) - p3*(V3) + C1 + C2 + C3;
end