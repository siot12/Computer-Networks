

#D1

confidence_interval_1=function(n,xn,alfa,dispersion)
{
  critical=qnorm(1-alfa/2,0,1)
  sigma=sqrt(dispersion)
  x=xn-critical*sigma/(sqrt(n))
  y=xn+critical*sigma/(sqrt(n))
  cat("Intervalul de incredere este:(",x,",",y,").\n")
  return(c(x,y))
}
confidence_interval_1(150, 490, 1-0.95, 85)

#D2

confidence_interval_2=function(n,xn,alfa,deviatiastand)
{
  critical=qt(1-alfa/2,n-1)
  x=xn-critical*deviatiastand/(sqrt(n))
  y=xn+critical*deviatiastand/(sqrt(n))
  cat("Intervalul de incredere este:(",x,",",y,").\n")
  return(c(x,y))
}
confidence_interval_2(25, 101.5, 1-0.99, 5^2)


#D3

test=function(n,p0,succes,alfa,tip_ipoteza)
{
  pprim=succes/n
  zscore=(pprim-p0)/sqrt(p0*(1-p0)/n)
  if(tip_ipoteza=='left')
  {
    cat("Asimetrica la stanga:\n")
    criticalz=qnorm(alfa,0,1)
    if(zscore<criticalz)
    {
      cat("Ipoteza nula respinsa -> ipoteza alternativa\n")
    }
    else
    {
      cat("Nu avem dovezi.\n")
    }
  }
  if(tip_ipoteza=='right')
    {
    cat("Asimetrica la dreapta:\n")
    criticalz=qnorm(1-alfa,0,1)
    if(zscore>criticalz)
    {
      cat("Ipoteza nula respinsa -> ipoteza alternativa\n")
    }
    else
    {
      cat("Nu avem dovezi.\n")
    }
  }
  if(tip_ipoteza=='sim')
  {
    cat("Simetrica:\n")
    criticalz=qnorm(alfa/2,0,1)
    if(abs(zscore)>abs(criticalz))
    {
      cat("Ipoteza nula respinsa -> ipoteza alternativa\n")
    }
    else
    {
      cat("Nu avem dovezi.\n")
    }
  }
  cat("Zscore=",zscore,"iar Zcritic=",criticalz)
}


#D3
#alfa=0.01
test(1179, 0.08, 100, 0.01, "left")


#alfa=0.05
test(1179, 0.08, 100, 0.05, "left")



#Locurile de munca

#alfa=0.05
test(125, 0.171, 24, 0.05, "left")

#alfa=0.01
test(125, 0.171, 24, 0.01, "left")

#D4

#alfa=0.05
test(250, 0.4, 95, 0.05, "sim")

#alfa=0.01
test(250, 0.4, 95, 0.01, "sim")

