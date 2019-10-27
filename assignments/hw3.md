Heesang Mark Whang

CSCI 311: Operating Systems

Fall 2019

Assignment 2

1. **(6.7) in Textbook: [1.0 p]**
* The pseudocode of Figure 6.15 illustrate the basic push() and pop() operations of an array-based stack. Assuming that this algorithm could be used in a concurrent environment, answer the following questions:

```C++
push(item) {
    if (top < SIZE) {
        stack[top] = item;
        top++;
    }
    else
        ERROR
}

pop() {
    if (!is_empty()) {
        top--;
        return stack[top];
    }
    else
        ERROR
}
```

* A) For problem a, show a scenario to demonstrate how a race condition can occur.

*Answer:--------------------------------------*
*Assume initial top value was 5, and given item parameter was "item6"*

![](.\hw3_prob1.png)

*with push() and pop() running concurrently, could cause race condition as above*
*pop() would return stack[4], while push() increments next top index at stack[6]*

* B) For problem b, copy functions push(), pop(), and is_empty() and then add your solution to them. 
  **Hint:** what happens if push() and pop() are executed concurrently?

*Answer:--------------------------------------*

```C++
push(item) {
    wait(S); // Semaphore to busy wait until other process calls signal()
    if (top < SIZE) {
        stack[top] = item;
        top++;
        signal(Q); // Releae for other process to exit busy wait
    }
    else
        ERROR
}

pop() {
    if (!is_empty()) {
    	wait(Q); // Semaphore to busy wait until other process calls signal()
        top--;
        signal(S); // Releae for other process to exit busy wait
        return stack[top];
    }
    else
        ERROR
}

is_empty() { // Returns true if stack top is -1 (empty)
	if (top == -1)
		return true;
	else
		return flase;
}
```




2. **(6.8) in Textbook: [1.0 p]**
* Race conditions are possible in many computer systems. Consider an online auction system where the current highest bid for each item must be maintained. A person who wishes to bid on an item calls the *bid(amount)* function, which compares the amount being bid to the current highest bid. IF the amount exceeds the current highest bid, the highest bid is set to the new amount. This is illustrated below:

```C++
void bid(double amount) {
    if (amount > highestBid)
        highestBid = amount;
}
```

* Show a scenario to demonstrate a race condition can occur. Also, copy function bid() and then add your own solution to it. 
  **Hint:** What happens if two bidders place two bids higher than the current highest bid at the same time?

*Answer:--------------------------------------*
  *Possible Race Condition:*

![](.\hw3_prob2.png)

  // Race condition where 98 become highest bid while  bid(100)
  *To solve this:*

```C++
void bid(double amount) {
    mutex_lock lock; // Utilize mutex lock to prevent concurrent access to the CS
    
    lock.acquire();
    if (amount > highestBid)
        highestBid = amount; // Critical Section
    lock.release();
}
```


3. **(6.12) in Textbook [1.0 p]**
* Some semaphores implementations provide a function *getValue()* that returns the current value of a semaphore. This function may, for instance be invoked prior to calling *wait()* so that process will only call *wait()* if the value of the semaphore is > 0, thereby preventing blocking while waiting for the semaphore. For example:

```C++
if (getValue(&sem) > 0)
    wait(&sem);
```
* Many developers argue against such a function and discourage its use. Describe a potential problem that could occur when using the function *getValue()* in this scenario.

*Answer:--------------------------------------*
*Using such implementation (preventing busy waiting by checking semaphore value prior to wait()) can result in an liveness error of priority inversion. For instance when two processes are trying to enter critical section concurrently, both process will enter if(getValue(&sem) > =0) statement, and pass the busy waiting stage. This could result in both process entering the critical section concurrently, which can cause race condition.*

4. **(6.19) in Textbook [1.0 p]**
* Assume that a system has multiple processing cores. For each of the following scenarios, describe which is a better locking mechanism -- a spinlock or mutex lock where waiting processes sleep while waiting for the lock to become available:
  * The lock is to be held for a short duration
  * The lock is to be held for a long duration
  * A thread may be put to sleep while holding the lock.

*Answer:--------------------------------------*
*[1] **Spinlock** would fit the first scenario because it locks threads for the shortest duration*
*[2] **Mutex lock** would be the best for the second scenario because mutex lock can hold long term wait*
*[3] **Mutex lock** would fit the third scenario because it allows threads to go to sleep*

5. **(6.24) in Textbook [1.0 p]**
* In Section 6.7, we use the following illustration as an incorrect use of semaphores to solve the critical-section problem:
  
```pseudocode
wait(mutex);
    ...
critical seciton
    ...
wait(mutex);
```

* Explain why this is an example of a liveness failure.

*Answer:--------------------------------------*
*As the process enters the critical section, the second call to wait() is permanently blocked without signal() resulting in an liveness failure of deadlock*