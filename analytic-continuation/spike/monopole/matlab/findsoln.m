function r = findsoln(varargin)

N = 300;
R = 10;

x0 = [ 0.5*tanh(linspace(-2.5,2.5,N))+0.5 0.5*tanh(linspace(-2.5,2.5,N))+0.5 ];
if(nargin == 1)
    x0 = varargin{1};
    disp('Used argument as starting point.'); disp(max(g(x0)));
end

r = linspace(0,R,N+2);

z = fsolve(@g, x0, optimset('MaxIter', 1000000, 'MaxFunEvals', 1000000));

figure;
hold on;


plot(r, [0. x0(1:N) 1. ], 'c+-');
plot(r, [0. x0(N+1:2*N) 1. ], 'c+-');

plot(r, [0. z(1:N) 1. ], 'b+-');
plot(r, [0. z(N+1:2*N) 1. ], 'r+-');

legend('f(r) - A_i^j', 'h(r) - \phi');

figure
gz = g(z);
plot(r(2:N+1), gz(1:N), 'c+-');
plot(r(2:N+1), gz(N+1:2*N), 'b+-');

F = [r' [0. z(1:N) 1.]'];
H = [r' [0. z(N+1:2*N) 1.]'];

save('func_f.m', 'F', '-ASCII');
save('func_h.m', 'H', '-ASCII');

r = z;