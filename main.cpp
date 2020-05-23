#include<iostream>
#include<semaphore.h>
#include <unistd.h> 
#include <pthread.h>    

#include <sys/mman.h>
#include<stddef.h>

#include <sys/ipc.h>
#include <sys/shm.h>
using namespace std;

static sem_t objSemafaro; //objeto semafaro
static int shm_id;
static key_t key = 1541;
static int countSemafaro; //quantidade de maxima no semafao
static int* qtdCliente;
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


//cria memoria compartilhda armaxenando em vetor
void create_shared_memory() {
     shm_id = shmget(key, 10*sizeof(int), 0666 | IPC_CREAT);
     qtdCliente = (int*)shmat(shm_id, NULL, 0);
     qtdCliente[0] = 0;
}


void iniciarBarbeiro(){
    while(true){
        sleep(5);
        cout << qtdCliente[0] << endl;
        if(qtdCliente[0] > 0){
            cout << "Cortando cabelo....." << endl;
            sem_post(&objSemafaro);
            cout << "Indo embora....." << endl;
            qtdCliente[0]--;
        }else{
            cout << "O barbeiro esta dormindo....." << endl;
        }
    }
}


void criarCliente(string nome){
    cout << "Atual " <<  qtdCliente[0] << endl;
    if(countSemafaro == qtdCliente[0]){
        cout << "O Cliente "+nome+" foi embora pois não havia lugar para esperar...." << endl;     
    }else{
        cout << "Aguardando para corte de cabelo....." << endl;
        qtdCliente[0]++; //atualiza a quantidqade de cliente
        sem_wait(&objSemafaro); //poem cliente no semafarto
    }

}

int main(){
    //inicialiaza a memoria compartilhada
    create_shared_memory();
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
                qtdCliente[0]++;
                break;
            }
        }
    }
    return 0; 
}
