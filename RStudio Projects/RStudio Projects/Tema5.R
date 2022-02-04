
#E1

E1=function(ipoteza,n,population_mean,sample_mean,sigma,alfa)
{
  t_score=(sample_mean-population_mean)/(sigma/sqrt(n))
  if(ipoteza == 'left')
  {
    criticalt=qt(alfa,n-1)
    if(t_score<criticalt)
    {
      cat("Respingem ipoteza nula => ipoteza alternativa.\n")
    }
    else
    {
      cat("Nu avem dovezi pentru a respinge ipoteza nula.\n")
    }
  }
  if(ipoteza == 'right')
  {
    criticalt=qt(1-alfa,n-1)
    if(t_score>criticalt)
    {
      cat("Respingem ipoteza nula => ipoteza alternativa.\n")
    }
    else
    {
      cat("Nu avem dovezi pentru a respinge ipoteza nula.\n")
    }
  }
  if(ipoteza == 'sim')
  {
    criticalt=qt(1-alfa/2,n-1)
    if(abs(t_score)>abs(criticalt))
    {
      cat("Respingem ipoteza nula => ipoteza alternativa.\n")
    }
    else
    {
      cat("Nu avem dovezi pentru a respinge ipoteza nula.\n")
    }
  }
  cat("Scorul:",t_score," Valoarea critica: ",criticalt)
}

E1("left", 50, 30, 27, 3.5, 0.01) 

#E2

E2=function(n, z_score, populationa_mean, alfa)
{
  critical_z=qnorm(1-alfa/2,0,1)
  if(abs(z_score)>abs(critical_z))
  {
    cat("Respingem ipoteza nula => ipoteza alternativa.\n")
  }
  else
  {
    cat("Nu avem dovezi pentru a respinge ipoteza nula.\n")
  }
  cat("Scorul:",z_score," Valoarea critica: ",critical_z)
}


#semnificatie 1%
E2(70, 2.05, 4, 0.01) 

#semnificatie 5%
E2(70, 2.05, 4, 0.05) 



#E3

E3=function(ipoteza,m0,n1,n2,sample1,sample2,sigma1,sigma2,alfa)
{
  z_score=(sample1-sample2-m0)/sqrt((sigma1^2)/n1+(sigma2^2)/n2)
  if(ipoteza == 'left')
  {
    critical_z=qnorm(alfa,0,1)
    if(z_score<critical_z)
    {
      cat("Respingem ipoteza nula => ipoteza alternativa.\n")
    }
    else
    {
      cat("Nu avem dovezi pentru a respinge ipoteza nula.\n")
    }
  }
  if(ipoteza == 'right')
  {
    critical_z=qnorm(1-alfa,0,1)
    if(z_score>critical_z)
    {
      cat("Respingem ipoteza nula => ipoteza alternativa.\n")
    }
    else
    {
      cat("Nu avem dovezi pentru a respinge ipoteza nula.\n")
    }
  }
  if(ipoteza == 'sim')
  {
    critical_z=qnorm(1-alfa/2,0,1)
    if(abs(z_score)>abs(critical_z))
    {
      cat("Respingem ipoteza nula => ipoteza alternativa.\n")
    }
    else
    {
      cat("Nu avem dovezi pentru a respinge ipoteza nula.\n")
    }
  }
  cat("Scorul:",z_score," Valoarea critica: ",critical_z)
}


#semnificatie 1%
E3("left", 0, 200, 224, 7.8, 8.1, 1.15, 0.92, 0.01)

#semnificatie 5%
E3("left", 0, 200, 224, 7.8, 8.1, 1.15, 0.92, 0.05)

#E4

E3("right", 0, 50, 50, 102, 109, 8.3, 7.5, 0.01)

#E5

E5=function(ipoteza,n1,n2,sigma1,sigma2,alfa)
{
  F_score=(sigma1^2)/(sigma2^2)
  if(ipoteza == 'right')
  {
    critical_F=qf(1-alfa,n1-1,n2-1)
    if(F_score>critical_F)
    {
      cat("Respingem ipoteza nula => ipoteza alternativa.\n")
    }
    else
    {
      cat("Nu avem dovezi pentru a respinge ipoteza nula.\n")
    }
    cat("Scorul:",F_score," Valoarea critica: ",critical_F)
  }
  if(ipoteza == 'sim')
  {
    critical_Fs=qf(alfa/2,n1-1,n2-1)
    critical_Fd=qf(1-alfa/2,n1-1,n2-1)
    if((F_score<critical_Fs)||(F_score>critical_Fd))
    {
      cat("Respingem ipoteza nula => ipoteza alternativa.\n")
    }
    else
    {
      cat("Nu avem dovezi pentru a respinge ipoteza nula.\n")
    }
    cat("Scorul:",F_score," Fs: ",critical_Fs, "Fd: ",critical_Fd)
  }
}

E5("right", 22, 22, 2.15, 1.95, 0.01)

