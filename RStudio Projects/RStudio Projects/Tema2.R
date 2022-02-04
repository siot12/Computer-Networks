#b1

elipsoid = function(N,a,b,c) 
{
  count=0
  for(i in 1:N)
  {
    x=runif(1,-a,a)
    y=runif(1,-b,b)
    z=runif(1,-c,c)
    if((x*x)/(a*a)+(y*y)/(b*b)+(z*z)/(c*c) <= 1)
    {
      count=count+1
    }
  }
  
  return(2*a*2*b*2*c*count/N)
}
a=3
b=2
c=4
elipsoid(10000,a,b,c)

#b2

arie_triunghi=function(N)
{
  c=0
  for(i in 1:N)
  {
    x=runif(1,0,2)
    y=runif(1,0,4/3)
    if(y<=x&2*x+y<=4)
    {
      c=c+1
    }
  }
  return((4/3)*2*c/N)
}
arie_triunghi(30000)
#b3

MC_improved_integration = function(N)
  {
  sum = 0;
  for(i in 1:N)
  {
    x=runif(1,0,1)
    sum = sum + 1/(1+sqrt(x));
  }
  return(sum/N);
}


MC_improved_integration(10000)
actual_val=2-2*log(2)
cat(actual_val)


MC_improved_integration2 = function(N)
{
  sum = 0;
  for(i in 1:N)
  {
    x=runif(1,0,10000)
    sum = sum + 1/(1+x*x);
  }
  return(10000*sum/N);
}

MC_improved_integration2(10000)
actual_val2=pi/2
cat(actual_val2)

#b4

timp_asteptare=function()
{
  r=runif(1,0,1)
  u=rexp(1,4)
  server=0
  if(r<0.35)
  {
    server=dgamma(5,3)
  }
  else
  {
    if(r>=0.35 & r<5)
    {
      server=dgamma(5,2)
    }
    else
    {
      if(r>=0.5 & r<0.7)
      {
        server=dgamma(4,3)
      }
      else
      {
        server=dgamma(6,4)
      }
    }
  }
  return(server+u) 
}

timp_mediu_asteptare=function(N)
{
  s=0
  for(i in 1:N)
  {
    s=s+timp_asteptare()
  }
  return(s/N)
}
t=timp_mediu_asteptare(1000)
cat("Timpul mediu de asteptare este: ", t)


#B5

vector_initial = function(n) 
{
  #creez un vector bool
  i=1
  infection=vector(mode="logical",length = n)
  while(i<=3)
  {
    pos=floor(runif(1,1,n+1))
    if(infection[pos] == FALSE)
    { #infectez 3 calculatoare 
      infection[pos] = TRUE
      i=i+1
    }
  }
  return(infection)
}


computers_infected=function(vec,N)
{
  c=0
  for(i in 1:N)
  {
    if(vec[i]==TRUE)
    {
      c=c+1
    }
  }
  return(c)
}


clean= function(vec,k,q)
{
  curatat = vector()
  j= 1;
  for (i in 1:length(vec))
  {
    if (vec[i]==TRUE) 
    {
      curatat[j] = i;
      j = j + 1;
    }
  }
  l=length(curatat)
  if(l>0){
    h = k
    if(k > l)
      h = l
    index = sample(l, h, replace = FALSE)
    for(i in 1:length(index)){
      p=runif(1)
      if(p <= q)
        vec[curatat[index[i]]]=FALSE
    }
  }
  return(vec);
}


infection = function(vec,p)
{
  l = length(vec);
  for (i in 1:l)
  {
    if (vec[i]==TRUE)
    {
      for (j in 1:l)
      {
        if (vec[j]==FALSE)
        {
          x = runif(1);
          if (x <= p)
          { 
            vec[j] = TRUE;
          }
        }
      }
    }
  }
  return(vec);
}

#a)
B5a = function(p,q,k){
  day = 1;
  calculatoare = vector_initial(40);
  m = 1;
  while(m < 40){
    calculatoare = infection(calculatoare, p);
    calculatoare = clean(calculatoare, k, q);
    m = computers_infected(calculatoare, 40);
    day = day+1;
  }
  return(day);
}
B5a(0.2,0.2,3)

