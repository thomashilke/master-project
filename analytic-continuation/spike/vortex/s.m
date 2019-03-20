%%% Build the jacobian:

%parameters:
alpha = 1.; beta = 1.;

% position of last point:
R = 10.;

% number of points
N = 1000;
r = linspace(0,10.,N+2);
r = r(2:N+1);
h=R/(N+1);

%let x be the vector containing the values of function F and A:
% TODO: It will require proper initialisation.
x = ones(2*N,1);

plot(x);

hold on;

for k = 1:5

%% COMPUTE THE JACOBIAN:
j = zeros(2*N,2*N);

%fill d1, d5:
for i = 1:N
    j(i, N+i) = 2*x(i)*(x(N+i)-1.); %%d1
    j(N+i, i) = -beta*j(i,N+i); %%d5
end


%fill d2, d4:
for i = 1:N-1
    j(i, i+1) = r(i)*r(i)/(h*h)+r(i)/(2.*h); %%g_i, d2
    j(i+1, i) = r(i)*r(i)/(h*h)-r(i)/(2.*h); %%g_{i+1}, d4
end
%j(N,N+1) = 0;
%j(N+1,N) = 0;
for i = N+1:2*N-1
    j(i, i+1) = r(i-N)/(h*h)-1./(2.*h); %%g_i, d2
    j(i+1, i) = r(i-N)/(h*h)+1./(2.*h); %%g_{i+1}, d4
end

%fill d3:
for i = 1:N
    j(i,i) = -2.*r(i)*r(i)/(h*h)+alpha*r(i)*r(i)*(3.*x(i)*x(i)-1.)+(x(i+N)-1.)*(x(i+N)-1.);
    j(N+i, N+i) = -2.*r(i)/(h*h)-beta*x(i)*x(i);
end


%% EVALUATE THE FUNCTION:
b = zeros(2*N,1);

b(1) = r(1)*r(1)/(h*h)*(-2*x(1)+x(2))+r(1)/(2.*h)*x(2)+alpha*r(1)*r(1)*(x(1)*x(1)-1.)*x(1)+x(1)*(x(N+1)-1)*(x(N+1)-1);
for i=2:N-1
    b(N) = r(i)*r(i)/(h*h)*(x(i-1)-2.*x(i)+x(i+1))+r(i)/(2.*h)*(x(i+1)-x(i-1))+alpha*r(i)*r(i)*(x(i)*x(i)-1.)*x(i)+x(i)*(x(N+i)-1)*(x(N+i)-1);
end
b(N) = r(N)*r(N)/(h*h)*(x(N-1)-2.*x(N)+1.)+r(N)/(2.*h)*(1.-x(N-1))+alpha*r(N)*r(N)*(x(N)*x(N)-1.)*x(N)+x(N)*(x(2*N)-1)*(x(2*N)-1);

b(N+1) = r(1)/(h*h)*(x(N+2)-2.*x(N+1))-1./(2.*h)*x(N+2)+beta*x(1)*x(1)*(1.-x(N+1));
for i=2:N-1
    b(i) = r(i)/(h*h)*(x(N+i-1)-2.*x(N+i)+x(N+i+1.))-1./(2.*h)*(x(N+i+1)-x(N+i-1))+beta*x(i)*x(i)*(1.-x(N+i));
end
b(2*N) = r(N)/(h*h)*(x(2*N-1)-2.*x(N)+1.)-1./(2.*h)*(1.-x(2*N-1))+beta*x(N)*x(N)*(1.-x(2*N));


%% SOLVE THE SYSTEM:

 y = j\b;
 
 x = x - y;
    plot(x);

end

