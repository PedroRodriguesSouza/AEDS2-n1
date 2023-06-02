/*
4 - Inserção ordenada
5 - Remoção
6 - Pesquisa
8 - Impressão dos registros livre
*/
#include<iostream>
#include<fstream>

using namespace std;

/*defini��o da estrutura a ser armazenado no arquivo*/
union dado
{
    struct
    {
        int quant, first, last, free;
    } cab;//cabeçalho
    struct
    {
        int chave, next, prev;
    } reg;//registro
};

void inicializar(fstream &arq)
{
    /*Inicializando a estrutura*/

    cout<<endl<<endl<<"Inicializando a estrutura de registros"<<endl<<endl;

    dado d;
    /*Posicionando o ponteiro de escrita no in�cio do arquivo*/
    arq.seekp (0,arq.beg);

    /*preenchendo o cabe�alho*/
    d.cab.quant=0;
    d.cab.first=-1;
    d.cab.last=-1;
    d.cab.free=1;

    /*Gravando o cabe�alho*/
    arq.write((char*)&d, sizeof(d));

    /*Gravando os 8 registros*/
    for (int i = 1; i <= 8; i++)
    {
        if(i==8)
            d.reg.next=-1;
        else
            d.reg.next=i+1;

        arq.write((char*)&d, sizeof(d));
    }
}


void imprimir(fstream &arq)
{
    dado d;

    cout<<endl<<endl<<"Imprimindo a estrutura de registros"<<endl;

    /*posicionando o ponteiro de leitura no in�cio do arquivo*/
    arq.seekg (0,arq.beg);

    /*lendo o cabe�alho*/
    arq.read((char*)&d, sizeof(d));

    /*imprimindo o cabe�alho*/
    cout<<"cab\n";
    cout<<"Quant: "<<d.cab.quant<<endl;
    cout<<"First: "<<d.cab.first<<endl;
    cout<<"Last: "<<d.cab.last<<endl;
    cout<<"Free: "<<d.cab.free<<endl;

    /*lendo e imprimindo os 8 registros*/
    cout<<"Registros\n";
    for (int i = 1; i <= 8; i++)
    {
        arq.read((char*)&d, sizeof(d));
        cout<<i<<"- Chave: "<<d.reg.chave<<endl;
        cout<<i<<"- Next: "<<d.reg.next<<endl;
        cout<<i<<"- Prev: "<<d.reg.prev<<endl<<endl;

    }
}

void imprimirSeq(fstream &arq)
{
    dado c,r;

    cout<<endl<<endl<<"Imprimindo registros validos"<<endl;

    /*posicionando o ponteiro de leitura no in�cio do arquivo*/
    arq.seekg (0,arq.beg);

    /*lendo o cabe�alho*/
    arq.read((char*)&c, sizeof(c));
    /*imprimindo o cabe�alho*/
    cout<<"cab\n";
    cout<<"Quant: "<<c.cab.quant<<endl;
    cout<<"First: "<<c.cab.first<<endl;
    cout<<"Last: "<<c.cab.last<<endl;
    cout<<"Free: "<<c.cab.free<<endl;

    /*lendo e imprimindo os registros com dados*/
    cout<<"\nRegistros\n\n";
    if(c.cab.first==-1)
    {
        cout<<"N�o existe registros";
        return;
    }

    /*posiciona o ponteiro de leitura no primeiro registro apontado por first*/
    arq.seekg(c.cab.first*sizeof(c),arq.beg);
    int pos = r.reg.next;
    do
    {
        /*lendo o registro*/
        arq.read((char*)&r, sizeof(r));
        pos = r.reg.next;
        /*imprimindo o registro*/
        cout<<"Chave: "<<r.reg.chave<<endl;
        cout<<"Next: "<<r.reg.next<<endl;
        cout<<"Prev: "<<r.reg.prev<<endl<<endl;
        /*localizando o pr�ximo registro*/
        if(r.reg.next!=-1)
            arq.seekg(r.reg.next*sizeof(r),arq.beg);
    }
    while(r.reg.next!=-1);
}


void insere(fstream &arq, dado d)
{
    dado aux, c;
    int free;
    cout<<endl<<endl<<"Inserindo registros"<<endl;

    /*posiciona o ponteiro de leitura no in�cio do arquivo*/
    arq.seekg(0,arq.beg);

    /*le o cabe�alho*/
    arq.read((char*)&c, sizeof(c));

    /*posiciona o ponteiro de leitura na primeira posi��o livre no arquivo*/
    arq.seekg(sizeof(c)*c.cab.free,arq.beg);

    /*le as informa��es do registro livre*/
    arq.read((char*)&aux, sizeof(aux));

    /*grava em free o pr�ximo registro livre*/
    free=aux.reg.next;

    /*atribuindo as informa��es do novo registro*/
    aux=d;
    aux.reg.next=-1;
    aux.reg.prev=c.cab.last;

    /*atualizando o next do registro que vai apontar para o novo registro aux*/
    if(aux.reg.prev!=-1)
    {
        dado bob;
        /*posiciona o ponteiro de leitura para o registro que vai apontar para o novo registro aux*/
        arq.seekg(sizeof(aux)*aux.reg.prev,arq.beg);
        /*lendo o registro*/
        arq.read((char*)&bob, sizeof(bob));
        /*atualizando o next*/
        bob.reg.next=c.cab.free;

        /*posiciona o ponteiro de escrita para o registro que vai apontar para o novo registro aux*/
        arq.seekp(sizeof(aux)*aux.reg.prev,arq.beg);
        /*gravando o registro*/
        arq.write((char*)&bob, sizeof(bob));
    }
    /*posiciona o ponteiro de leitura para a posi��o do novo registro aux*/
    arq.seekp(sizeof(c)*c.cab.free,arq.beg);
    /*gravando o registro*/
    arq.write((char*)&aux, sizeof(aux));

    /*Atualiza��o o cabe�alho*/
    if(c.cab.first==-1)
        c.cab.first=c.cab.free;

    c.cab.quant++;
    c.cab.last=c.cab.free;
    c.cab.free=free;
    /*posiciona o ponteiro de escrita no in�cio do arquivo*/
    arq.seekp(0,arq.beg);
    /*gravado o cabe�alho*/
    arq.write((char*)&c, sizeof(c));
}

int pesquisa(fstream &arq, int chave, bool ordenada = false)
{
    dado c, aux;
    int pos;
    arq.seekg(0,arq.beg);//le o cabeçalho
    arq.read((char*)&c, sizeof(c));
    pos = c.cab.first;
    arq.seekg(sizeof(aux)*pos,arq.beg);//le o primeiro da lista
    arq.read((char*)&aux, sizeof(aux));
    if(ordenada){//paremetro opcional da função para otimização de pesquisa
        while(aux.reg.chave < chave && aux.reg.next != -1){//percorre o vetor até achar a chave ou até o ultimo registro
            pos = aux.reg.next;
            arq.seekg(sizeof(aux)*aux.reg.next,arq.beg);
            arq.read((char*)&aux, sizeof(aux));
        }
        if(aux.reg.chave == chave)//encontrou
            return pos;
        return -1;
    }
    else{
        while(aux.reg.chave != chave && aux.reg.next != -1){//percorre o vetor até achar a chave ou até o ultimo registro
            pos = aux.reg.next;
            arq.seekg(sizeof(aux)*aux.reg.next,arq.beg);
            arq.read((char*)&aux, sizeof(aux));
        }
        if(pos == aux.cab.last+1)//chegou ao registro da lista
            if(aux.reg.chave != chave)
                return -1;
        return pos;
    }
}

void remover(fstream &arq, int chave)
{
    dado c;
    int indice = pesquisa(arq, chave); //encontra onde esta o arquivo a ser apagado
    if(indice != -1){ //se encontrar o registro a ser apagado
        dado apg, prev, next;
        arq.seekg(sizeof(apg)*indice,arq.beg);//posiciona o ponteiro no registro que será apagado
        arq.read((char*)&apg, sizeof(apg));

        arq.seekg(0,arq.beg); //le o cabeçalho
        arq.read((char*)&c, sizeof(c));

        if(c.cab.quant > 1){//se este não for o unico registro da lista
            if(apg.reg.prev != -1 && apg.reg.next != -1){//se o registro possui um anterior e um sucessor
                arq.seekg(sizeof(prev)*apg.reg.prev,arq.beg);//posiciona o ponteiro no registro anterior
                arq.read((char*)&prev, sizeof(prev));
                arq.seekg(sizeof(next)*apg.reg.next,arq.beg);//posiciona o ponteiro no proximo registro
                arq.read((char*)&next, sizeof(next));
                next.reg.prev = apg.reg.prev;
                prev.reg.next = apg.reg.next;
                arq.seekp(sizeof(next)*apg.reg.next,arq.beg);//escreve os dados que foram alterados
                arq.write((char*)&next, sizeof(next));
                arq.seekp(sizeof(prev)*apg.reg.prev,arq.beg);
                arq.write((char*)&prev, sizeof(prev));
            }
            else if(apg.reg.prev == -1){//se é o primeiro da lista
                arq.seekg(sizeof(next)*apg.reg.next,arq.beg);//posiciona o ponteiro no proximo registro
                arq.read((char*)&next, sizeof(next));
                next.reg.prev = -1;
                arq.seekp(sizeof(next)*apg.reg.next,arq.beg);//escreve os dados que foram alterados
                arq.write((char*)&next, sizeof(next));
                c.cab.first = apg.reg.next;//altera o endereço do primeiro registro
            }
            else if(apg.reg.next == -1){//se é o ultimo da lista
                arq.seekg(sizeof(prev)*apg.reg.prev,arq.beg);//posiciona o ponteiro no registro anterior
                arq.read((char*)&prev, sizeof(prev));
                prev.reg.next = -1;
                arq.seekp(sizeof(prev)*apg.reg.prev,arq.beg);//escreve os dados que foram alterados
                arq.write((char*)&prev, sizeof(prev));
                c.cab.last = apg.reg.prev;//altera o endereço do ultimo registro
            }
        }

        //tornando o registro free
        apg.reg.chave = 0; //"limpando" a memória
        apg.reg.prev = -1;
        apg.reg.next = c.cab.free; //referenciando o proximo registro livre
        arq.seekp(sizeof(apg)*indice,arq.beg);//escreve os novos dados do registro apagado no arquivo
        arq.write((char*)&apg, sizeof(apg));

        c.cab.free = indice; //declarando este registro como o primeiro registro livre no cabeçalho
        c.cab.quant = c.cab.quant-1;
        arq.seekp(0,arq.beg);//escreve as novas informações do cabeçalho no arquivo
        arq.write((char*)&c, sizeof(c));

    }
}




int main()
{
    dado d;
    string nome = "pagina.dat";
    fstream arq;

    arq.open(nome,ios::binary| fstream::in | fstream::out );
    if(!arq.is_open())
    {
       arq.open(nome,ios::binary| fstream::in | fstream::out|fstream::trunc );
       if(!arq.is_open())
        {
            cout<<"Erro ao abrir o arquivo!!";
            return 0;
        }
    }

    inicializar(arq);
    /*lendo os dados para o novo registro*/
    cout<<"Inserindo novo registro:\nDigite a chave: ";
    cin>>d.reg.chave;
    //d.reg.chave = 1;
    insere(arq,d);

    cout<<"\nInserindo novo registro:\nDigite a chave: ";
    cin>>d.reg.chave;
    //d.reg.chave = 2;
    insere(arq,d);

    cout<<"\nInserindo novo registro:\nDigite a chave: ";
    cin>>d.reg.chave;
    //d.reg.chave = 3;
    insere(arq,d);

    cout << endl;
    cout << endl;

    remover(arq, 2);

    imprimirSeq(arq);
    imprimir(arq);

    arq.close();
    return 0;
}
