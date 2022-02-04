#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int guard(int n, char * err)
{
   if(n==1)
   {
     perror(err);
     exit(1);
   }
   else
   {
     return n;
   }
}

int main (int argc, char *argv[])
{
  int sdTCP;	
  int sdUDP;		// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char msgTCP[300];
  char msgUDP[100];
  char vitstrUDP[100];	
  char comandaTCP[100];
  int nfds;
  fd_set readfds;
  fd_set writefds;
  //fd_set actfds;
  	// mesajul trimis

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("[client] Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sdTCP = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
      perror ("[client] Eroare la socket() TCP.\n");
      return errno;
  }
  if((sdUDP=socket(AF_INET,SOCK_DGRAM,0))==-1)
  {
    perror("[client] Eroare la socket() UDP.\n");
    return errno;
  }
  
  //int flags= guard(fcntl(sdTCP,3),"could not get flags on TCP listening socket");
  //guard(fcntl(sdTCP,4,flags | 04000),"Could not set file flags");
  //fcntl(sdTCP,4,fcntl(sdTCP,3) | 04000);
  
  // pentru simulare traseu
  FILE* fp;
  char line[256];
  char ids[256];
  fp=fopen("carjourney.xml","r");
  fgets(line,256,fp);
  strcpy(line,line+4);
  int n=strlen(line);
  n=n-4;
  strncpy(ids,line,n);
  char id[2]; // id-ul strazii

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
   int ok=0;
  pid_t pid;
  pid=fork();
  if(pid==-1)
  {
    printf("Eroare la fork in client!\n");
    exit(1);
  }
  if(pid==0) //copil [UDP]
  {
    
    bzero (vitstrUDP, 100);
    printf ("[clientUDP]Introduceti viteza si strada : ");
    fflush (stdout);
    read (0, vitstrUDP, 100);
    while(1)
    {
      printf("[clientUDP]Va aflati pe: ");
      printf("%s\n",vitstrUDP);
      int length,msglen;
      /* trimiterea mesajului la server */
      length=sizeof(server);
      if (sendto (sdUDP, vitstrUDP, sizeof(vitstrUDP),0, (struct sockaddr*)&server, length) <= 0)
      {
          perror ("[clientUDP]Eroare la sendto() spre server.\n");
          return errno;
      }
       /*citirea raspunsului dat de server */
       /* (apel blocant pina cind serverul raspunde) */
      if ( (msglen=recvfrom (sdUDP, msgUDP, sizeof(msgUDP),0,(struct sockaddr*)&server, &length)) < 0)
      {
          perror ("[clientUDP]Eroare la recvfrom() de la server.\n");
          return errno;
      }
       /*afisam mesajul primit */
      printf ("[clientUDP]Mesajul primit este: %s\n", msgUDP);
  

      //alegerea unei noi viteze random 
      bzero(vitstrUDP,100);
      bzero(id,2);
      
      srand(time(0));
      int r=rand()%60;
      //printf("%d\n",r);
      char spd[10];
      sprintf(spd,"%d",r);
      strcpy(vitstrUDP,spd);
      strcat(vitstrUDP," ");

      //alegerea unei strazi dintr-un fisier xml

      
      strncpy(id,ids,1);
      //printf("%s\n",id);
      strcpy(ids,ids+1);

      //printf("%s\n",id);
      // verificare strada

      if(strcmp(id,"0")==0)
      {
         strcat(vitstrUDP,"Aleea Sucidava");
      }
      if(strcmp(id,"1")==0)
      {
        strcat(vitstrUDP,"Strada Sucidava");
      }
      if(strcmp(id,"2")==0)
      {
        strcat(vitstrUDP,"Strada Amurgului");
      }
      if(strcmp(id,"3")==0)
      {
        strcat(vitstrUDP,"Strada Sarmisegetuza");
      }
      if(strcmp(id,"4")==0)
      {
        strcat(vitstrUDP,"Strada Cazaban");
      }
      if(strcmp(id,"5")==0)
      {
        strcat(vitstrUDP,"Strada Buridava");
      }
      if(strcmp(id,"6")==0)
      {
        strcat(vitstrUDP,"Strada Tabacului");
      }
      if(strcmp(id,"7")==0)
      {
        strcat(vitstrUDP,"Bulevardul Dacia");
      }

      if(ids[0]=='\0')
      {
        ok=1;
      }
      if(ok==1)
      {
        printf("Am ajuns la destinatie!\n");
        break;
      }
      sleep(60);
      
    }
      /*inchidem socket-ul, am terminat */
      close (sdUDP);
      exit(1);
      

  }
  else //parintele [TCP]
  {
    sleep(10);
    if (connect (sdTCP, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[clientTCP]Eroare la connect().\n");
      return errno;
    }

    pid_t pidd;
    pidd=fork();
    if(pidd==-1)
    {
      printf("Eroare la fork in client!\n");
      exit(1);
    }
    if(pidd==0) //copil trimite
    {
      //introducem o comanda
      bzero(comandaTCP,100);
      printf("[clientTCP]Introduceti o comanda: ");
      fflush(stdout);
      read(0,comandaTCP,100);

      while(1)
      {
        if(strcmp(comandaTCP,"options\n")==0)
        {
          printf("[clientTCP]Doriti informari despre vreme,evenimente sportive sau preturi?\n");
          bzero(comandaTCP,100);
          printf("[clientTCP]Introduceti raspunsul: ");
          fflush(stdout);
          read(0,comandaTCP,100);
        }

        if(strcmp(comandaTCP,"report\n")==0)
        {
          printf("[clientTCP]Ce doriti sa raportati? Incidente pe care le puteti raporta: Accident, Radar, Ambuteiaj.\n");
          printf("[clientTCP]Sintaxa: Incident Locatie\n");
          bzero(comandaTCP,100);
          printf("[clientTCP]Introduceti incidentul si locatia: ");
          fflush(stdout);
          read(0,comandaTCP,100);
        }

        if (write (sdTCP, comandaTCP, sizeof(comandaTCP)) <= 0)
        {
            perror ("[client]Eroare la write() spre server.\n");
            return errno;
        }

        sleep(1);
        bzero(comandaTCP,100);
        printf("[clientTCP]Introduceti o comanda: ");
        fflush(stdout);
        read(0,comandaTCP,100);
      }
    }
    else  //parintele receptioneaza
    {
      while(1)
      {
        if (read (sdTCP, msgTCP, sizeof(msgTCP)) < 0)
        {
            perror ("[clientTCP]Eroare la read() de la server.\n");
            return errno;
        }
        printf ("[clientTCP]Mesajul primit este: %s\n", msgTCP);

        if(strcmp(msgTCP,"quit\n")==0)
        {
          kill(pid,SIGTERM);
          kill(pidd,SIGTERM);
          close(sdTCP);
          break;
        }

      }
      wait(NULL);
    }
  }
}