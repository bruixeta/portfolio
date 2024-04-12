X0 = [0.2 0.4 0.4 0.1 0.15 0.2];
QR = 10;
A = [1 1 1 0 0 0];
B = QR;
LB = zeros(1,6);
NONLCON = @ConstraintsD4_3;
FUN = @FunctionD4_3;
X = fmincon(FUN,X0,A,B,[],[],LB,[],NONLCON)
B = -FunctionD4_3(X)

