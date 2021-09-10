/* *************** ES c1 ***************
Scopo di questo esercizio è di scrivere un monitor mbuf che realizzi un buffer limitato dove ogni elemento
venga inserito con una molteplicità. In particolare mbuf deve fornire le seguenti procedure entry:

void add(type data, int n);
    che inserisce il dato data con molteplicità n.
type get(void);
    che restituisce il primo dato disponibile.

Il monitor deve soddisfare i seguenti vincoli:
    • il buffer deve contenere al più MAXELEM elementi (ogni operazione add aggiunge un elemento
        indipendentemente dalla molteplicità n)
    • i dati vengono consegnati in ordine FIFO
    • un dato inserito con molteplicità n deve essere ricevuto da n processi: il monitor deve attendere che n processi
        chiamino (o abbiano chiamato) la get e a quel punto restituisce il dato a tutti e toglie l'elemento dal buffer. 
****************************************** */

int MAXELEM;

monitor mbuf{
    queue buffer; // <type data, int n>
    condition full; // buffer > 0
    condition empty; // buffer < MAXELEM

    entry void add(type data, int n){
        if(buffer.lenght >= MAXELEM)
            empty.wait();
        
        buffer.enqueue(<data, n>);
        full.signal();
    }

    entry type get(void){
        if(buffer.lenght == 0)
            full.wait();
        
        <data, n> = buffer.first();
        n--;
        if(n <= 1){
            <data, n> = buffer.dequeue();
            empty.signal();
        }

        return data;
    }
}

/* *************** ES 2 ***************
 Riscrivere bohm in bohs
*/

process bohm[i, i=0,1] {
    for (;;) {
        m.pre(i);
        print(i);
        m.post(i);
    }
}
monitor m:
    condition ok[2];
    state = 0;
    
    procedure entry pre(int n) {
        if (state != n)
            ok[n].wait();
    }
    
    procedure entry post(int n) {
        state = 1 - state;
        ok[state].signal();
    }
}

// -----------------------------
process bohs[i, i=0,1] {
    for (;;) {
        pre(i);
        print(i);
        post(i);
    }
}

int state = 0;
Semaphore mutex = new Semaphore(1);
Semaphore s[2];
s[0] = new Semaphore(1);
s[1] = new Semaphore(1);

void pre(int n) {
    mutex.P()
    if (state != n){
        mutex.V();
        s[n].P();
    } else {
        mutex.V();
    }
}

void post(int n) {
    mutex.P();
    state = 1 - state;
    if(blocked){
        s[state].V();
    } else {
        mutex.V();
    }
}