#include <iostream>
#include<string.h>
#include<fstream>
using namespace std;
//un sring + cat de lung e stringul
class cuvant
{
public:
    char c[100];
    int n;
    cuvant(){};
    cuvant(char *cu)
    {
        strcpy(c,cu);
        n=strlen(cu);
    }


};
class Regula
{
public:
    cuvant t[100];
    char lit[2];
    int n=0;
    Regula(){};
    Regula(char *ch)
    {
        n=1;
        strcpy(t[0].c,ch);
        n=strlen(ch);
    }
    void add(cuvant cu)
    {
        int ok=0;
        for(int i=0;i<n;i++)
        {
            if(strstr(cu.c,t[i].c) && cu.n==t[i].n)
                ok=1;
        }
        if(ok==0)
        {
            t[n]=cu;
            t[n].n=strlen(cu.c);
            n++;
        }
    }
    Regula& operator=(const Regula &re)
    {
        int i;
        for(i=0;i<re.n;i++)
            t[i]=re.t[i];
        n=re.n;
    }
    bool caut(char cuv)
    {
        int i;
        for(i=0;i<n;i++)
        {
            if(strchr(t[i].c,cuv))
                return 1;
        }
        return 0;
    }
    void elimnul()
    {
        int i;
        for(i=0;i<n;i++)
        {
            if(strchr(t[i].c,'#'))
            {
                strcpy(t[i].c,t[i].c+1);
                break;
            }
        }
        while(i<n-1)
        {
            t[i]=t[i+1];
            i++;
        }
        n--;
    }
    void revnul(char l)
    {
        int i,j;
        char cuvantul[40];
        cuvant *cuv;
        for(i=0;i<n;i++)
        {
            for(j=0;j<t[i].n;j++)
            {
                if(t[i].c[j]==l && t[i].n==1)
                {
                    cuv=new cuvant("#");
                    add(*cuv);

                }
                else
                if(t[i].c[j]==l)
                {
                    strcpy(cuvantul,t[i].c);
                    strcpy(cuvantul+j,cuvantul+j+1);
                    add(cuvantul);
                }
            }
        }
    }
    void dellit(char ch)//sterge o litera
    {
        int i,ok=0;
        for(i=0;i<n;i++)
        {
            if(strchr(t[i].c,ch) && t[i].n==1)
            {
                strcpy(t[i].c,t[i].c+1);
                ok=1;
                break;
            }
        }
        while(i<n-1)
        {
            t[i]=t[i+1];
            i++;

        }
        if(ok==1)
            n--;
    }
};
class Gramatica
{
public:
    Regula r[100];
    int n=1;
    Gramatica(){};
    void nous()
    {
        //verifica daca S se afla in partea stanga si daca da creaza o satre nou S0 pe pozitia 0
        int i,ok=0;
        for(i=1;i<=n;i++)
        {
            if(r[i].caut('S'))
                ok=1;
        }
        if(ok==1)
        {
            r[0].lit[0]='S';
            r[0].lit[1]='0';
            r[0].t[0].c[0]='S';
            r[0].n=1;
            r[0].t[0].n=1;
        }
    }
    void citire()
    {
        FILE *f;
        f=fopen("f.txt","r");
        if(f==NULL)
            cout<<"PROB LA DESCHIDERE FIDIER";
        int nr,i;
        char c[100],*p;
        Regula reg;
        cuvant *cuv;
        fscanf(f,"%d",&nr);
        fgets(c,10,f);
        for(i=1;i<=nr;i++)
        {
            fgets(c,30,f);
            p=strtok(c," |\n");
            r[i].lit[0]=p[0];
            p=strtok(NULL," |");
            while(p!=NULL)
            {
                cuv=new cuvant(p);
                r[i].add(*cuv);
                p=strtok(NULL," |\n");
            }
        }
        n=nr;
    }
    void remuveNUL()
    {
        int i,j,k,chr,ok=0;
        char litera,cuvant[30];
        while(ok==0)
        {
            ok=1;
            for(i=1;i<=n;i++)
            {
                if(r[i].caut('#'))
                {
                    ok=0;
                    litera=r[i].lit[0];
                    r[i].elimnul();
                    for(j=1;j<=n;j++)
                    {
                        if(r[j].caut(litera))
                        {
                            r[j].revnul(litera);
                        }
                    }
                }
            }
        }
    }
    void remuvUNIT()
    {
        int i,j,k,l,ok=1;
        char ch;
        //verific daca X->X daca da il sterg
        for(i=1;i<=n;i++)
        {
            if(r[i].caut(r[i].lit[0]))
            {

               r[i].dellit(r[i].lit[0]);
            }

        }
        //inlocuiesc s in s0
        r[0]=r[1];
        r[0].lit[1]='0';

        for(i=1;i<=n;i++)
        {

            for(j=0;j<=r[i].n;j++)
            {
                //resetez
                if(ok==0)
                {
                    j=0;
                    ok=1;
                }

                if(r[i].t[j].n==1 && r[i].t[j].c[0]>='A' && r[i].t[j].c[0]<='Z')
                {
                    ch=r[i].t[j].c[0];
                    r[i].dellit(ch);
                    //caut starea scoasa
                    for(k=1;k<=n;k++)
                        if(r[k].lit[0]==ch)
                            break;
                    //adaug toate elementele din starea gasita
                    for(l=0;l<r[k].n;l++)
                    {
                        r[i].add(r[k].t[l]);
                    }
                    ok=0;
                }
            }
        }
    }
    void Replaceproduction()
    {
        int i,j,l,k,exista;
        cuvant *cuv;
        char ch[100];
        for(i=0;i<=n;i++)
        {
            for(j=0;j<r[i].n;j++)
            {
                //cauta o productie mai mare cac 3
                if(r[i].t[j].n>=3)
                {
                    exista=0;
                    strcpy(ch,r[i].t[j].c+1);
                    //cauta daca productia este in gramatica
                    for(k=0;k<=n;k++)
                    {
                        if(r[k].n==1 && strcmp(ch,r[k].t[0].c)==0)
                        {
                            exista=1;
                            break;
                        }
                    }
                    if(exista==0)
                    {
                        cuv=new cuvant(ch);
                        n++;
                        r[n].add(*cuv);
                        r[n].lit[0]='A'+n-2;
                        r[n].lit[1]=NULL;
                        strcpy(r[i].t[j].c+1,r[n].lit);
                        r[i].t[j].n=2;
                    }
                    else
                    {
                        strcpy(r[i].t[j].c+1,r[k].lit);
                        r[i].t[j].n=2;
                    }
                }
            }
        }
    }
    void Replaceterminal()
    {
        int i,j,k,exista;
        cuvant *cuv;
        char ch[2];
        for(i=0;i<=n;i++)
        {
            for(j=0;j<r[i].n;j++)
            {
                if(r[i].t[j].n==2)
                {
                    //cautare pentu prima pozitie
                    if(r[i].t[j].c[0]>='a' && r[i].t[j].c[0]<='z')
                    {
                        exista=0;
                        ch[0]=r[i].t[j].c[0];
                        //cautare daca exista deja in gramatica
                        for(k=0;k<=n;k++)
                        {
                            if(r[k].n==1 && ch[0]==r[k].t[0].c[0] && r[k].t[0].n==1)
                            {
                                exista=1;
                                break;
                            }
                        }
                        if(exista==0)
                        {
                            cuv=new cuvant(ch);
                            n++;
                            r[n].add(*cuv);
                            r[n].lit[0]='A'+n-2;
                            r[n].lit[1]=NULL;
                            r[i].t[j].c[0]=r[n].lit[0];
                        }
                        else
                        {
                            r[i].t[j].c[0]=r[k].lit[0];
                        }
                    }
                    //cautare pt a doua pozitie
                    if(r[i].t[j].c[1]>='a' && r[i].t[j].c[1]<='z')
                    {
                         exista=0;
                        ch[0]=r[i].t[j].c[1];
                        for(k=0;k<=n;k++)
                        {
                            if(r[k].n==1 && ch[0]==r[k].t[0].c[0] && r[k].t[0].n==1)
                            {
                                exista=1;
                                break;
                            }
                        }
                        if(exista==0)
                        {
                            cuv=new cuvant(ch);
                            n++;
                            r[n].add(*cuv);
                            r[n].lit[0]='A'+n-2;
                            r[i].t[j].c[1]=r[n].lit[0];
                        }
                        else
                        {
                            r[i].t[j].c[1]=r[k].lit[0];
                        }
                    }
                }
            }
        }
    }
    void afisare()
    {
        int i,j;
        for(i=0;i<=n;i++)
        {
            cout<<r[i].lit<<": ";
            for(j=0;j<r[i].n;j++)
                cout<<r[i].t[j].c<<" | ";
            cout<<"\n";
        }

    }
};
int main()
{
    Gramatica g;
    g.citire();

    cout<<"Initial:\n";
    g.afisare();
    cout<<"\n";

    g.nous();

    cout<<"Pas 1:\n";
    g.afisare();
    cout<<"\n";

    g.remuveNUL();

    cout<<"Pas 2:\n";
    g.afisare();
    cout<<"\n";

    g.remuvUNIT();

    cout<<"Pas 3:\n";
    g.afisare();
    cout<<"\n";

    g.Replaceproduction();

    cout<<"Pas 4:\n";
    g.afisare();
    cout<<"\n";

    g.Replaceterminal();
    cout<<"Final:"<<endl;
    g.afisare();

}
