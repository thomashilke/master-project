function b = g(x)


N = length(x)/2;
R = 10;
r = linspace(0,R,N+2);
r = r(2:N+1);

h = R/(N+1.);

b = zeros(2*N, 1);

b(1) = r(1)^2/(h*h)*(x(2)-2.*x(1)) ...
       + L(r(1),x(1),x(N+1));
   
b(N) = r(N)^2/(h^2)*(1.-2.*x(N)+x(N-1)) ...
       + L(r(N),x(N),x(2*N));

b(N+1) = r(1)^2/(h*h)*(x(N+2)-2.*x(N+1)) ...
       + r(1)/h*x(N+2) ...
       + M(r(1),x(1),x(N+1));

b(2*N) = r(N)^2/(h*h)*(1.-2.*x(2*N) + x(2*N-1)) ...
         + r(N)/h*(1.-x(2*N-1)) ...
         + M(r(N), x(N), x(2*N));

for i=2:N-1
   b(i) = r(i)^2/(h^2)*(x(i+1)-2.*x(i)+x(i-1)) ...
       + L(r(i),x(i),x(N+i)); 
end

for i = N+2:2*N-1
   b(i) = r(i-N)^2/(h*h)*(x(i+1)-2.*x(i) + x(i-1)) ...
          + r(i-N)/h*(x(i+1)-x(i-1)) ...
          + M(r(i-N), x(i-N), x(i));
end

