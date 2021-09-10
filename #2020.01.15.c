/* ********* ESERCIZIO c1 *********
    Il monitor "semaforo con timeout" semtimeout deve fornire tre procedure entry:
        void V(void)
        boolean P(unsigned int timeout)
        void tick(void)
    Vale l'invariante dei semafori generali. La funzione tick viene richiamata ad ogni millisecondo. Un processo messo in
    attesa a causa di una P attende al massimo il numero di millisecondi indicato nel parametro.
    Se un processo viene riattivato per timeout la P ritorna valore vero, altrimenti falso. Le operazioni V devono riattivare i
    processi in attesa in ordine FIFO.
    Scrivere semtimeout. 
    ********************************* */

monitor semtimeout{
    
    int blocked;
    int value;
    int ticks;
    bool doneV;
    condition invariante; /* (n_p + init - n_v = value) >= 0 */

    semtimeout(int init){
        value = init;
        blocked = 0;
    }

    entry void V(void){
        doneV = true;
        value++;
    }

    entry boolean P(unsigned int timeout){
        if(value == 0){
            bool precV;
            int startTick = ticks;

            blocked++;
            while(ticks - startTick <= timeout && !doneV)
                invariante.wait();

            precV = !doneV;
            blocked--;
            value--;

            return precV;
        } else {
            value--;
            return false;
        }
    }

    entry void tick(void){
        ticks++;
        for(int i = 0; i < blocked; i++)
            invariante.signal();
    }
}

/*  ESERCIZIO c2
    Dato un servizio di message passing asincrono scrivere un servizio di message passing sincrono a
    spedizione multipla (senza fare uso di processi server). Devono essere previste due funzioni:
        mulsend(pid_t destination, T msg, int times)
        T mulrecv(pid_t sender)
    La chiamata mulsend spedisce il messaggio msg al processo destination times volte. Solo quando il messaggio è
    stato ricevuto times volte da destination l'esecuzione della funzione mulsend viene completata.
    Il ricevente può indicare il valore zero come sender nella mulrecv per indicare che vuole ricevere un messaggio da
    qualsiasi mittente.
*/

mulsend(pid_t destination, T msg, int times){
    for(int i = 0, i < times; i++){
        asend(<getpid(), msg>, destination);
        areceive(destination);
    }
}

T mulrecv(pid_t sender){
    <realSender, msg> = areceive(sender);
    asend(ACK, realSender);
    return msg;
}