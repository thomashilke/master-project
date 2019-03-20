%%% Build the jacobian:
function m = j(x)

%parameters:
alpha = 1.; beta = 1.;

% position of last point:
R = 100.;

% number of points
N = length(x)/2;
r = linspace(0,R,N+2);
r = r(2:N+1);
h = R/(N+1);

%% COMPUTE THE JACOBIAN:
m = zeros(2*N,2*N);

%fill d1, d5:
for i = 1:N
    m(i, N+i) = -2*x(i)*(x(N+i)-1.); %%d1
    m(N+i, i) = 2.*beta*x(i)*(1-x(N+1)); %%d5
end


%fill d2, d4:
for i = 1:N-1
    m(i, i+1) = r(i)*r(i)/(h*h)+r(i)/(2.*h); %%g_i, d2
    m(i+1, i) = r(i)*r(i)/(h*h)-r(i)/(2.*h); %%g_{i+1}, d4
end
%j(N,N+1) = 0;
%j(N+1,N) = 0;
for i = N+1:2*N-1
    m(i, i+1) = r(i-N)/(h*h)-1./(2.*h); %%g_i, d2
    m(i+1, i) = r(i-N)/(h*h)+1./(2.*h); %%g_{i+1}, d4
end

%fill d3:
for i = 1:N
    m(i,i) = -2.*r(i)*r(i)/(h*h) ...
             -alpha*r(i)*r(i)*(3.*x(i)*x(i)-1.) ... 
             -(x(i+N)-1.)*(x(i+N)-1.);
    m(N+i, N+i) = -2.*r(i)/(h*h) ...
                  -beta*x(i)*x(i)*r(i);
end