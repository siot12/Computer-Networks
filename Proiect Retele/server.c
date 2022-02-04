#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

/* portul folosit */

#define PORT 2728
int v[8];
int before_street=-1;
extern int errno;		/* eroarea returnata de unele apeluri */

typedef struct thData{
	int idThread; //id-ul thread-ului tinut in evidenta de acest program
	int cl; //descriptorul intors de accept
}thData;

struct clients{
    int  clientfd;
    int id;

}c[20];

int clients=0;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);

int max(int x,int y)
{
    if(x>y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

void checkspeedandstreet(char vitspd[ ],char rasp[ ])
{
    char vit[100];
    char street[100];
    bzero(vit,100);
    for(int i=0;i<strlen(vitspd);i++)
    {
        if(vitspd[i]==' ')
        {
            strncpy(vit,vitspd,i);
            strcpy(vitspd,vitspd+i+1);
            break;
        }
    }
    //printf("%s\n",vit);
    //printf("%s\n",vitspd); //Aleea-5 Strada-6 Bulevard-8
    strcpy(street,vitspd);

    // Verificam vitezele
    int ok=0;

    if(strncmp(street,"Aleea",5)==0) 
    {
        if(atoi(vit)<=30)
        {
            strcpy(rasp,"Viteza legala!\n");
        }
        else
        {
            strcpy(rasp,"Ati intrecut limita de viteza! Limita pe aceasta portiune de drum este 30km/h!\n");
        }
    }
    else
    {
        if((strncmp(street,"Strada",6)==0))
        {
            if(atoi(vit)<=50)
            {
                strcpy(rasp,"Viteza legala!\n");
            }
            else
            {
                strcpy(rasp,"Ati intrecut limita de viteza! Limita pe aceasta portiune de drum este 50km/h!\n");
            }
        }
        else
        {
            if((strncmp(street,"Bulevardul",10)==0))
            {
                if(atoi(vit)<=60)
                {
                    strcpy(rasp,"Viteza legala!\n");
                }
                else
                {
                    strcpy(rasp,"Ati intrecut limita de viteza! Limita pe aceasta portiune de drum este 60km/h!\n");
                }
            }
        }
    }
}

void checktraffic(int v[],int n, char msg[]) // v-vectorul de frecventa
{                                // n-numarul de strazi din zona
    int i;
    FILE *fp;
    int id;
    char line[256];

    for(i=0;i<n;i++)
    {
        if(i<1 && v[i]>=3)
        {
            fp=fopen("strazi.xml","r");
            char line[256];
            fseek(fp,8,SEEK_SET);
            while(fgets(line,256,fp)!=NULL)
            {
                //printf("%s",line);
                strcpy(line,line+8);
                //printf("%s",line);
                if(strncmp(line,"<id>",4)==0)
                {
                    strcpy(line,line+4);
                    char aux[2];
                    strncpy(aux,line,1);
                    //printf("%s\n",aux);
                    id=atoi(aux);
                    //printf("%d\n",id);
                }
                if(strncmp(line,"<nume>",6)==0)
                {
                    if(i==id)
                    {
                        strcpy(line,line+6);
                        char aux2[20];
                        int k=strlen(line);
                        strncpy(aux2,line,k-8);
                        //printf("%s\n",aux2);
                        strcat(msg,aux2);
                        strcat(msg,"\n");
                        bzero(aux2,sizeof(aux2));
                    }
                }
            }
        }
        if((i>=1 && i<=6) && v[i]>=5)
        {
            fp=fopen("strazi.xml","r");
            char line[256];
            fseek(fp,8,SEEK_SET);
            while(fgets(line,256,fp)!=NULL)
            {
                //printf("%s",line);
                strcpy(line,line+8);
                //printf("%s",line);
                if(strncmp(line,"<id>",4)==0)
                {
                    strcpy(line,line+4);
                    char aux[2];
                    strncpy(aux,line,1);
                    //printf("%s\n",aux);
                    id=atoi(aux);
                    //printf("%d\n",id);
                }
                if(strncmp(line,"<nume>",6)==0)
                {
                    if(i==id)
                    {
                        strcpy(line,line+6);
                        char aux3[20];
                        int k=strlen(line);
                        strncpy(aux3,line,k-8);
                        //printf("%s\n",aux3);
                        strcat(msg,aux3);
                        strcat(msg,"\n");
                        bzero(aux3,sizeof(aux3));

                    }
                }
            }

        }
        if((i>=7) && v[i]>=10)
        {
            fp=fopen("strazi.xml","r");
            char line[256];
            fseek(fp,8,SEEK_SET);
            while(fgets(line,256,fp)!=NULL)
            {
                //printf("%s",line);
                strcpy(line,line+8);
                //printf("%s",line);
                if(strncmp(line,"<id>",4)==0)
                {
                    strcpy(line,line+4);
                    char aux[2];
                    strncpy(aux,line,1);
                    //printf("%s\n",aux);
                    id=atoi(aux);
                    //printf("%d\n",id);
                }
                if(strncmp(line,"<nume>",6)==0)
                {
                    if(i==id)
                    {
                        strcpy(line,line+6);
                        char aux2[20];
                        int k=strlen(line);
                        strncpy(aux2,line,k-8);
                        //printf("%s\n",aux2);
                        strcat(msg,aux2);
                        strcat(msg,"\n");
                        bzero(aux2,sizeof(aux2));
                    }
                }
            }
        }
    }


}

void addcars(char vspd[])
{

    char street[100];
    /*for(int i=0;i<strlen(vspd);i++)
    {
        if(vspd[i]==' ')
        {
            
            strcpy(vspd,vspd+i+1);
            break;
        }
    }*/
    
    printf("%s\n",vspd); 
    strcpy(street,vspd);

    if(strcmp(street,"Aleea Sucidava")==0)
    {
        v[0]++;
        before_street=0;
    }
    if(strcmp(street,"Strada Sucidava")==0)
    {
        v[1]++;
        before_street=1;
    }
    if(strcmp(street,"Strada Amurgului")==0)
    {
        v[2]++;
        before_street=2;
    }
    if(strcmp(street,"Strada Sarmisegetuza")==0)
    {
        v[3]++;
        before_street=3;
    }
    if(strcmp(street,"Strada Cazaban")==0)
    {
        v[4]++;
        before_street=4;
    }
    if(strcmp(street,"Strada Buridava")==0)
    {
        v[5]++;
        before_street=5;
    }
    if(strcmp(street,"Strada Tabacului")==0)
    {
        v[6]++;
        before_street=6;
    }
    if(strcmp(street,"Bulevardul Dacia")==0)
    {
        v[7]++;
        before_street=7;
    }
}

int main()
{
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
    pthread_t th[100];
    int tcpfd,udpfd,nfds;
    fd_set actfds;
    int len;
    char buffer[100];
    char rasp[100];
    int i=0; //nr de thread-uri

    if ((tcpfd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server] Eroare la socket().\n");
      return errno;
    }
    int on=1;
    setsockopt(tcpfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)); 

    if ((udpfd = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
    {
      perror ("[server] Eroare la socket().\n");
      return errno;
    }

    bzero (&serveraddr, sizeof (serveraddr));
    bzero(&clientaddr,sizeof(clientaddr));

  /* umplem structura folosita de server */
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl (INADDR_ANY);
  serveraddr.sin_port = htons (PORT);

    if (bind (tcpfd, (struct sockaddr *) &serveraddr, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server] Eroare la bind().\n");
      return errno;
    }

    if (bind (udpfd, (struct sockaddr *) &serveraddr, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server] Eroare la bind().\n");
      return errno;
    }

     if (listen (tcpfd, 5) == -1)
    {
      perror ("[server] Eroare la listen().\n");
      return errno;
    }

    FD_ZERO(&actfds);
    nfds=max(tcpfd,udpfd);
    while(1)
    {
        FD_SET(tcpfd,&actfds);
        FD_SET(udpfd,&actfds);
        if (select (nfds+1, &actfds, NULL, NULL,NULL) < 0)
	    {
	        perror ("[server] Eroare la select().\n");
	        return errno;
	    }

        int connfd;
        thData * td;
        len=sizeof(clientaddr);

        if(FD_ISSET(tcpfd,&actfds))
        {
            
            
                connfd= accept(tcpfd,(struct sockaddr*)&clientaddr,&len);
                td=(struct thData*)malloc(sizeof(struct thData));	
                c[i].clientfd=connfd;
                c[i].id=i;
                clients++;
	            td->idThread=i++;
	            td->cl=connfd;
                pthread_create(&th[i], NULL, &treat, td);
        }

        if(FD_ISSET(udpfd,&actfds))
        {
            len=sizeof(clientaddr);
            bzero(buffer,sizeof(buffer));
            recvfrom(udpfd,buffer,sizeof(buffer),0,(struct sockaddr*)&clientaddr,&len);
            printf("[UDP]Am primit viteza si locatia. Facem verificarea!\n");
            checkspeedandstreet(buffer,rasp);
            v[before_street]--;
            addcars(buffer);
            printf("[UDP]Am terminat verificarea! Trimitem raspunsul!\n");
            //strcpy(buffer,"UDP");
            sendto(udpfd,rasp,sizeof(rasp),0,(struct sockaddr*)&clientaddr,sizeof(clientaddr));
            printf("[UDP]Raspuns trimis!\n");
        }
    }
}


static void *treat(void *arg)
{
    struct thData tdL;
    tdL= *((struct thData*)arg);
    printf("[thread]-%d- Astept comanda...\n", tdL.idThread);
    fflush(stdout);
    pthread_detach(pthread_self());
    raspunde((struct thData*)arg);
    close((intptr_t)arg);
    return(NULL);
}

void raspunde(void *arg)
{
    char comanda[300];
    int i=0;
    struct thData tdL;
    tdL= *((struct thData*)arg);
    while(1)
    {
        if(read(tdL.cl,comanda,sizeof(comanda))<=0)
        {
            printf("[Thread %d]\n",tdL.idThread);
            perror("Eroare la read() de la client.\n");
        }
        printf("[Thread %d]Mesajul a fost receptionat...%s\n",tdL.idThread,comanda);
         
        //comanda quit  
        if(strcmp(comanda,"quit\n")==0)
        {
            if(write(tdL.cl,comanda,sizeof(comanda))<=0)
            {
                printf("[Thread %d]\n",tdL.idThread);
                perror("Eroare la write() catre client.\n");
            }
            else
            {
                printf("[Thread %d]Mesajul a fost transmis cu succes.\n",tdL.idThread);
            }
            
            printf("[Thread %d]Inchidem conexiunea cu clientul!\n",tdL.idThread);
            break;
        }

        //parte din comanda options!
        if(strcmp(comanda,"Da\n")==0)
        {
            bzero(comanda,sizeof(comanda));
            strcpy(comanda,"Vremea este buna, Poli Iasi castiga in deplasare cu Botosani, 6.10 lei/litru Benzina, 6.05 lei/litru Motorina");
        }
        if(strcmp(comanda,"Nu\n")==0)
        {
            bzero(comanda,sizeof(comanda));
            strcpy(comanda,"Aveti selectat NU!");
        }

        //comanda traffic
        if(strcmp(comanda,"traffic\n")==0)
        {
            char msg[300];
            bzero(msg,300);
            checktraffic(v,8,msg);
            bzero(comanda,sizeof(comanda));
            strcpy(comanda,msg);
        }

        if(strncmp(comanda,"Accident",8)==0)
        {
            char msg[100];
            strcpy(msg,comanda); 
            for(int i=0;i<=clients;i++)
            {
                if(tdL.idThread!=c[i].id)
                {
                    if(write(c[i].clientfd,msg,sizeof(msg))<=0)
                    {
                        printf("EROARE!\n");
                    }
                }
            }
            bzero(comanda,sizeof(comanda));
            bzero(msg,100);
            strcpy(comanda,"Am raportat incidentul!\n");
        }

        if(strncmp(comanda,"Radar",5)==0)
        {
            char msg[100];
            strcpy(msg,comanda); 
            for(int i=0;i<=clients;i++)
            {
                if(tdL.idThread!=c[i].id)
                {
                    if(write(c[i].clientfd,msg,sizeof(msg))<=0)
                    {
                        printf("EROARE!\n");

                    }
                }
            }
            bzero(comanda,sizeof(comanda));
            bzero(msg,100);
            strcpy(comanda,"Am raportat incidentul!\n");
        }

        if(strncmp(comanda,"Ambuteiaj",9)==0)
        {
            char msg[100];
            strcpy(msg,comanda); 
            for(int i=0;i<=clients;i++)
            {
                if(tdL.idThread!=c[i].id)
                {
                    if(write(c[i].clientfd,msg,sizeof(msg))<=0)
                    {
                        printf("EROARE!\n");

                    }
                }
            }
            bzero(comanda,sizeof(comanda));
            bzero(msg,100);
            strcpy(comanda,"Am raportat incidentul!\n");
        }

        printf("[Thread %d]Trimitem mesajul inapoi...%s\n",tdL.idThread,comanda);

        if(write(tdL.cl,comanda,sizeof(comanda))<=0)
        {
            printf("[Thread %d]\n",tdL.idThread);
            perror("Eroare la write() catre client.\n");
        }
        else
        {
            printf("[Thread %d]Mesajul a fost transmis cu succes.\n",tdL.idThread);
        }
    }
}



