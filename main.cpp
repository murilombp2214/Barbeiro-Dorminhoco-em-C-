#include<iostream>
#include<semaphore.h>
#include <unistd.h> 
#include <pthread.h>    
using namespace std;


static sem_t objSemafaro; //objeto semafaro

int countSemafaro = 3; //quantidade de maxima no semafao
int qtdCliente = 0;

class Semafaro{
    public:
        //cria um semafaro
        static sem_t criar(int qtdMax,sem_t& s){
           sem_init(&s, //ponteiro para o semafaro
                    0, //todos os recursos serão do mesmo processo
                    qtdMax ); //quantidade maxima que o semafatro vai suportar
           return s;
        }
};



void iniciarBarbeiro(){
    while(true){
        sleep(5);
        cout << qtdCliente << endl;
        if(qtdCliente > 0){
            cout << "Cortando cabelo....." << endl;
            sem_post(&objSemafaro);
            cout << "Indo embora....." << endl;
            qtdCliente--;
        }else{
            cout << "O barbeiro esta dormindo....." << endl;
        }
    }
}


void criarCliente(string nome){
    cout << "Atual " <<  qtdCliente << endl;
    if(countSemafaro == qtdCliente){
        cout << "O Cliente "+nome+" foi embora pois não havia lugar para esperar...." << endl;     
    }else{
        cout << "Aguardando para corte de cabelo....." << endl;
        qtdCliente++; //atualiza a quantidqade de cliente
        sem_wait(&objSemafaro); //poem cliente no semafarto
    }

}

int main(){
    Semafaro::criar(countSemafaro,objSemafaro);
    int pid = fork(); 
    if (pid == 0)  //iniciar o barbeito em um processo
    { 
        iniciarBarbeiro();
    } 
    else //inicia barbearia (È O PROCESSO PRINCIPAL)
    {
        while(true){
            string cliente = "";
            cin >> cliente;

            if(fork() == 0){ //se o processo for o processo filho cria o cliente
                criarCliente(cliente);
                break;
            }

        }
    }
    
    return 0; 
}
