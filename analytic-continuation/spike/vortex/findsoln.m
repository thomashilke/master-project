N=400;
R = 7;

x0 = [tanh(linspace(-2.5, 2.5, N)) tanh(linspace(-2.5, 2.5, N))];
r = linspace(0,R,N+2);

z = fsolve(@g, x0, optimset('MaxIter', 100000, 'MaxFunEvals', 100000));

figure;
hold on;
axis([0,10,0,10])
plot(r, [0. z(1:N) 1.], 'b+-');
plot(r, [0. z(N+1:2*N) 1.], 'r+-');

legend('A', 'F');

A = [r' [0. z(1:N) 1.]'];
F = [r' [0. z(N+1:2*N) 1.]'];

save('func_a.m', 'A', '-ASCII');
save('func_f.m', 'F', '-ASCII');