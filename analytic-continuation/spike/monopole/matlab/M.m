function m = M(r,x,y)

alpha = 1.;
beta = 1.;

%Dans les equations que j'ai trouvée, le signe devant le beta est '-'!!!

m = 2.*y*(x-1.)+2.*x*y*(1.-x) + beta*r^2*y*(1.-y^2);