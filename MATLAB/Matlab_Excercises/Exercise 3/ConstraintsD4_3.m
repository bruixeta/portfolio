function[C,Ceq] = ConstraintsD4_3(xc)
    C(1) = - 0.4*xc(1)^(0.9) + xc(4);
    C(2) = - 0.5*xc(2)^(0.8) + xc(5);
    C(3) = - 0.6*xc(3)^(0.7) + xc(6);

    Ceq = [];
end
