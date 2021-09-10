monitor M {

    struct process{
        Condition c;
        int timeout;
    }

    int blocked;
    queue<process> queue;

    entry int count(void){
        return blocked;
    }

    // Viene chiamata ogni millisecondo
    entry void tick(void){
        for p in queue{
            p.timeout--;
            if (p.timeout <= 0)
                p.c.signal();
        }
    }

    entry boolean delay(int timeout){
        ProcessQueue processBlocked = {new Condition, timeout};
        queue.append(processBlocked);
        blocked++;

        processBlocked.wait();
        
        queue.remove(processBlocked);
        blocked--;
        return true;
    }
}

/* Sia dato un sistema di processi a memoria privata che fornisca servizio di message passing asincrono.
Siano date inoltre la funzione call4fun ed il processo server definite come segue:
    def call4fun (x):
        asend(server, (getpid(), x, 0, 1))
        return(arecv(server))
    process server:
        forever:
            (s, n, r, v) = arecv(*)
            if (n > 0)
                asend(server, (s, n-1, r+v, v+2))
            else
                asend(s, r)
Cosa calcola la funzione call4fun e come viene calcolato questo valore? */

// Calcola il quadrato di x-1 (nel caso x=0 -> 0, oppure nel caso x=1 -> 1)
// x
0, 1
1, 3
4, 5
9, 7
16, 9
25, 11
36, 13
49, 15
// x = 0