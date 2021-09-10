/* ************ ES 1 ************
Scopo di questo esercizio è di scrivere un monitor pairbuf che implementi un buffer illimitato ad
accoppiamento. Ogni processo che vuole scrivere un elemento nel buffer chiama la funzione di entrata:
void put(T x)
mentre ogni processo che vuole leggere un elemento dal buffer chiama la funzione:
T get(void)
Gli elementi devono essere consegnati in ordine FIFO. Sia la get sia la put sono bloccanti. Solo quando il numero degli
scrittori che hanno chiamato la put è uguale a quello dei lettori che hanno chiamato la get tutti i processi in attesa
vengono sbloccati.
Quindi per esempio, se un solo scrittore ha chiamato la put, quando arriva un lettore entrambi vengono sbloccati, e il
lettore riceverà il valore passato come parametro dallo scrittore. Se vi fossero 10 lettori in attesa occorrono 10 scrittori
perche' i 20 processi continuino l'esecuzione. Chi ha chiamato la get per primo riceverà il dato passato dal primo
chiamante della put e così via in ordine FIFO. 
******************************** */

monitor pairbuf:

    Queue<T> buffer = new Queue();    
    int wr = 0;
    int ww = 0;
    condition equal;

    void put(T x){
        ww++;
        buffer.enqueue(x);

        if(ww != wr)
            equal.wait();

        ww--;
        equal.signal();
    }

    T get(void){
        wr++;

        if(ww != wr)
            equal.wait();

        x = buffer.dequeue();

        wr--;
        equal.signal();

        return x;
    }

/* ************ ES 2 ************
fun dilemma(x):
    asend((getpid(), x, ""), serverproc)
    return arecv(serverproc)

serverproc: process
    while True:
        (pid, x, y) = arecv(None)
        if x == "":
            asend(y, pid)
        else:
            asend((pid, x[1:], y + x[0]), getpid())

Cosa calcola la funzione dilemma tramite il processo serverproc? Descrivere come avviene il calcolo.
Se più processi chiamano la funzione dilemma in concorrenza il calcolo avviene in modo corretto? Perché?
Note: asend/arecv sono le primitive di un sistema di message passing asincrono. La funzione asend ha due parametri: il
messaggio da inviare e l'identità del processo destinatario. Il parametro della funzione arecv indica il mittente atteso,
None indica che vengono accettati messaggi da ogni mittente. La funzione getpid restituisce l'identità del processo chiamante.
******************************** */

// _ Esempio
dilemma("Ciao");

asend((getpid(), "Ciao", ""), serverproc)

(pid, "iao", "" + "C") // asend -> serverproc
(pid, "ao", "C" + "i") // asend -> serverproc
(pid, "o", "Ci" + "a") // asend -> serverproc
(pid, "", "Cia" + "o") // asend -> serverproc
// if x == ""
("Ciao", pid) // -> dilemma()

/* YES! Non ci sono variabili condivise */