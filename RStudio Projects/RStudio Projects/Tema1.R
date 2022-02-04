ex1a=function(k,l,lambda)
{
  y=dpois(k:l,lambda)
  z=max(y)
  print(z)
  return(barplot(y))
}
ex1a(3,7,0.6)


ex1b=function(k,l,p)
{
  x=seq(k,l,1)
  y=dgeom(x,p)
  print(y)
  barplot(y)
  return(sum(y[k:l-1]))
}
ex1b(2,6,0.2)

#ex 2

x=scan("sample.txt")
mean(x)
median(x)
sd(x)

ex2a=function(file)
{
  x=scan(file)
  print(summary(x))
  v=sd(x)
  print(v)
}
ex2a("sample.txt")

ex2b=function(file)
{
  x=scan(file)
  m = mean(x)
  s = sd(x)
  outliers = vector()
  j = 0
  for(i in 1:length(x))
  {
    if(x[i] < m - 2*s || x[i] > m + 2*s)
    {
      j = j + 1
      outliers[j] = x[i]
    }
  }
  outliers
  x[! x %in% outliers]
}

ex2b("sample.txt")

ex2c=function(file)
{
  x=scan(file)
  m = mean(x)
  s = sd(x)
  outliers = vector()
  j = 0
  for(i in 1:length(x))
  {
    if(x[i] < m - 2*s || x[i] > m + 2*s)
    {
      j = j + 1
      outliers[j] = x[i]
    }
  }
  outliers
  a=x[! x %in% outliers]
  interval=seq(30,100,10)
  hist(a,breaks=interval,right=F,freq=T)
}
ex2c("sample.txt")

