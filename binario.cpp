/**
 * NOMES: Gabriel Bragança de Oliveira          -           RA: 2022005455
 *        Pedro Rodrigues Souza                 -           RA: 2020013082
*/
#include<iostream>
#include<fstream>
using namespace std;
int tamanho;

/*definição da estrutura a ser armazenado no arquivo*/
union dado
{
    struct
    {
        int quant, first, last, free, size;
    } cab;//cabeçalho
    struct
    {
        int chave, next, prev;
    } reg;//registro
};

void inicializar(fstream &arq)
{
    /*Inicializando a estrutura*/

    //cout<<endl<<endl<<"Inicializando a estrutura de registros"<<endl<<endl;

    dado d;
    /*Posicionando o ponteiro de escrita no in�cio do arquivo*/
    arq.seekp (0,arq.beg);

    /*preenchendo o cabe�alho*/
    d.cab.quant=0;
    d.cab.first=-1;
    d.cab.last=-1;
    d.cab.free=1;
    d.cab.size = tamanho;

    /*Gravando o cabe�alho*/
    arq.write((char*)&d, sizeof(d));

    /*Gravando os 8 registros*/
    for (int i = 1; i <= tamanho; i++)
    {
        if(i==tamanho)
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
    int tam = d.cab.size;
    for (int i = 1; i <= tam; i++)
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
        cout<<"Nao existe registros";
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

void imprimir_free(fstream &arq)
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
    if(c.cab.free==-1)
    {
        cout<<"Nao existe registros livres"<<endl;
        return;
    }

    /*posiciona o ponteiro de leitura no primeiro registro apontado por first*/
    arq.seekg(c.cab.free*sizeof(c),arq.beg);
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

bool insere(fstream &arq, dado d)
{
    dado aux, c;
    int free;
    //cout<<endl<<endl<<"Inserindo registros"<<endl;

    /*posiciona o ponteiro de leitura no in�cio do arquivo*/
    arq.seekg (0,arq.beg);

    /*le o cabe�alho*/
    arq.read((char*)&c, sizeof(c));

    if(c.cab.free == -1){ 
        return false;
    }

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
        arq.seekp (sizeof(aux)*aux.reg.prev,arq.beg);
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
    arq.seekp (0,arq.beg);
    /*gravado o cabe�alho*/
    arq.write((char*)&c, sizeof(c));
    return true;
}

int pesquisa(fstream &arq, int chave, bool ordenada = false)
{
    dado c, aux;
    int indice;
    arq.seekg(0,arq.beg);//le o cabeçalho
    arq.read((char*)&c, sizeof(c));
    indice = c.cab.first;
    arq.seekg(sizeof(aux)*indice,arq.beg);//le o primeiro da lista
    arq.read((char*)&aux, sizeof(aux));


    if(ordenada){//paremetro opcional da função para otimização da pesquisa
        while(aux.reg.chave < chave && aux.reg.next != -1){//percorre o vetor enquanto a chave atual é menor que a chave que estamos buscando ou até o ultimo registro
            indice = aux.reg.next;
            arq.seekg(sizeof(aux)*aux.reg.next,arq.beg);//posiciona o ponteiro no proximo registro
            arq.read((char*)&aux, sizeof(aux));
        }
        if(aux.reg.chave == chave)
            return indice;
        return -1;
    }
    else{
        while(aux.reg.next != -1){//percorre o vetor até achar a chave ou até o ultimo registro
            if(aux.reg.chave == chave){
                return indice;
            }
            indice = aux.reg.next;
            arq.seekg(sizeof(aux)*aux.reg.next,arq.beg);//posiciona o ponteiro no proximo registro
            arq.read((char*)&aux, sizeof(aux));
        }
        if(aux.reg.chave == chave){
            return indice;
        }
        return -1;
    }
}

void remover(fstream &arq, int chave)
{
    int indice = pesquisa(arq, chave); //encontra onde esta o arquivo a ser apagado
    if(indice != -1){ //se encontrar o registro a ser apagado
        dado c, apg, prev, next;
        arq.seekg(sizeof(apg)*indice,arq.beg);//posiciona o ponteiro no registro que será apagado
        arq.read((char*)&apg, sizeof(apg));
        arq.seekg(0,arq.beg); //le o cabeçalho
        arq.read((char*)&c, sizeof(c));

        if(c.cab.quant == 1){ //se é o unico registro dada lista
            c.cab.quant = 0;
            c.cab.first = c.cab.last = apg.reg.prev = -1;
            apg.reg.next = c.cab.free;
            apg.reg.chave = 0;
            c.cab.free = indice;

            arq.seekp(sizeof(apg)*indice,arq.beg);//escreve os novos dados do registro apagado no arquivo
            arq.write((char*)&apg, sizeof(apg));
            arq.seekp(0,arq.beg);//escreve as novas informações do cabeçalho no arquivo
            arq.write((char*)&c, sizeof(c));
        }

        else{//se este não for o unico registro da lista
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

            else{//se é o ultimo da lista
                arq.seekg(sizeof(prev)*apg.reg.prev,arq.beg);//posiciona o ponteiro no registro anterior
                arq.read((char*)&prev, sizeof(prev));
                prev.reg.next = -1;
                arq.seekp(sizeof(prev)*apg.reg.prev,arq.beg);//escreve os dados que foram alterados
                arq.write((char*)&prev, sizeof(prev));
                c.cab.last = apg.reg.prev;//altera o endereço do ultimo registro
            }

            //tornando o registro free
            apg.reg.chave = 0; //"limpando" a memória
            apg.reg.prev = -1;
            apg.reg.next = c.cab.free; //referenciando o proximo registro livre
            c.cab.free = indice; //declarando este registro como o primeiro registro livre no cabeçalho
            --c.cab.quant;
            arq.seekp(sizeof(apg)*indice,arq.beg);//escreve os novos dados do registro apagado no arquivo
            arq.write((char*)&apg, sizeof(apg));
            arq.seekp(0,arq.beg);//escreve as novas informações do cabeçalho no arquivo
            arq.write((char*)&c, sizeof(c));
        }
    }
}
bool insere_ordenada(fstream &arq, dado d)
{
    dado maior, c, prev, free;
    int indice, aux;

    arq.seekg(0,arq.beg);// le o cabeçalho
    arq.read((char*)&c, sizeof(c));

    if(c.cab.free == -1){
        return false;
    }

    if(c.cab.quant<1)// se é o primeiro a ser inserido então usamos inserção simples
        insere(arq, d);
    else{
        arq.seekg(sizeof(maior)*c.cab.first,arq.beg);//posiciona o ponteiro no primeiro registro da lista
        arq.read((char*)&maior, sizeof(maior));
        arq.seekg(sizeof(free)*c.cab.free,arq.beg);//posiciona o ponteiro no primeiro elemento free
        arq.read((char*)&free, sizeof(free));
        free.reg.chave = d.reg.chave;
        indice = c.cab.first;
        while(maior.reg.chave < d.reg.chave && maior.reg.next != -1){//percorre o vetor até achar uma chave maior(ou igual) à chave a ser iserida
            indice = maior.reg.next;
            arq.seekg(sizeof(maior)*maior.reg.next,arq.beg);//posiciona o ponteiro no proximo registro
            arq.read((char*)&maior, sizeof(maior));
        }
        if(maior.reg.next == -1 && maior.reg.chave < free.reg.chave){ //se é o ultimo da lista
            insere(arq, d);
        }
        else if(maior.reg.prev == -1){ //se é o primeiro da lista
            aux = free.reg.next;
            free.reg.prev = -1;
            free.reg.next = indice;
            maior.reg.prev = c.cab.free;
            c.cab.first = c.cab.free;
            maior.reg.prev = c.cab.first;
            c.cab.free = aux;
            ++c.cab.quant;
            arq.seekp(sizeof(free)*c.cab.first,arq.beg);//escreve os novos dados do registro apagado no arquivo
            arq.write((char*)&free, sizeof(free));
            arq.seekp(sizeof(maior)*indice,arq.beg);//reescreve os dados alterados
            arq.write((char*)&maior, sizeof(maior));
            arq.seekp(0,arq.beg);//escreve as novas informações do cabeçalho no arquivo
            arq.write((char*)&c, sizeof(c));
        }
        else{//se for no meio da lista
            arq.seekg(sizeof(prev)*maior.reg.prev,arq.beg);//posiciona o ponteiro no registro anterior
            arq.read((char*)&prev, sizeof(prev));
            int p, n;
            p = maior.reg.prev;
            n = prev.reg.next;
            maior.reg.prev = c.cab.free;
            prev.reg.next = c.cab.free;
            c.cab.free = free.reg.next;
            ++c.cab.quant;
            free.reg.prev = p;
            free.reg.next = n;
            arq.seekp(sizeof(free)*maior.reg.prev,arq.beg);//escreve os novos dados do registro inserido
            arq.write((char*)&free, sizeof(free));
            arq.seekp(sizeof(prev)*free.reg.prev,arq.beg);//atualiza os dados do anterior
            arq.write((char*)&prev, sizeof(prev));
            arq.seekp(sizeof(maior)*free.reg.next,arq.beg);//atualiza os dados do sucessor
            arq.write((char*)&maior, sizeof(maior));
            arq.seekp(0,arq.beg);//escreve as novas informações do cabeçalho no arquivo
            arq.write((char*)&c, sizeof(c));
        }
    }
    return true;
}


int main()
{
    int menu, chave;
    dado d;
    string nome = "pagina.dat";
    fstream arq;

    cout << "Digite: \n(1) Ler um arquivo existente\n(2) Criar um arquivo\n#";
    cin>>menu;
    cout << endl;
    if(menu == 1){
        cout << "Digite o nome do arquivo: ";
        cin >> nome;
        cout << endl;
        if(!arq.is_open())
        {
            arq.open(nome,ios::binary| fstream::in | fstream::out);
            if(!arq.is_open())
            {
                cout<<"Erro ao abrir o arquivo!!";
                return 0;
            }
        }
    }
    else{
        cout << "Digite o nome do arquivo: ";
        cin >> nome;
        cout << "\nDefina o tamanho do Registro: ";
        cin >> tamanho;
        cout << "Registro com tamanho = " << tamanho << endl;
        if(!arq.is_open())
        {
            arq.open(nome,ios::binary| fstream::in | fstream::out|fstream::trunc);
            if(!arq.is_open())
            {
                cout<<"Erro ao abrir o arquivo!!";
                return 0;
            }
        }
    }
    if(menu==2){
        cout << "Programa incializado" << endl;
        inicializar(arq);
        cout << endl;
    }

   while(menu!=0){
    cout << "#------------------------------------------------------#" <<endl;
    cout << "1 - Insercao"<<endl<<
    "2 - Insercao ordenada"<<endl<<
    "3 - Remocao"<<endl<<
    "4 - Pesquisa" <<endl<<
    "5 - Impressao dos registros inseridos" <<endl<<
    "6 - Impressao dos registros livre" <<endl<<
    "7 - Impressao da estrutura" << endl << 
    "0 - Finalizar programa" << endl << "#";
    cin>>menu;

    cout << "#------------------------------------------------------#" <<endl;


    switch (menu){
        case 1:
            cout << "Digite a chave que queira inserir: ";
            cin>>d.reg.chave;
            if(insere(arq, d)) cout << "Chave " << d.reg.chave << " inserida" <<  endl;
            else cout << "Arquivo sem espaco" << endl;
            break;

        case 2:
            cout << "Digite a chave que queira inserir de forma ordenada: ";
            cin>>d.reg.chave;
            if(insere_ordenada(arq, d)) cout << "Chave " << d.reg.chave << " inserida" <<  endl;
            else cout << "Arquivo sem espaco" << endl;
            break;

        case 3:
            cout << "Digite a chave que queira remover: ";
            cin>>chave;
            if(pesquisa(arq, chave) != -1)  cout << "Chave " << chave << " removida" << endl;
            else cout << "Chave nao encontrada" <<endl;
            remover(arq, chave);
            break;

        case 4:
            cout << "Digite o valor para pesquisar: ";
            cin >> chave;
            cout << endl;
            if(pesquisa(arq, chave) == -1) cout << "Valor nao encontrado" <<endl;
            else cout << "Valor se encontra na posicao: " << pesquisa(arq, chave) << endl;
            break;

        case 5:
            cout << "Sequencia: " <<endl;
            imprimirSeq(arq);
            break;

        case 6:
            cout << "Sequencia free: " <<endl;
            imprimir_free(arq);
            break;

        case 7:
            imprimir(arq);
            break;

        case 0: 
            cout << "Programa finalizado..." <<endl;
            arq.close();
            return 0;
    }
   }

    arq.close();
    return 0;
}
