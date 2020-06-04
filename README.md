# Snakes

This is a **C++** implementation of the very well known **Snake** game. Except it's more than that.
This project uses **Neural Networks** to make the snakes intelligent, which are in turn trained using **Genetic Algorithm**.

---

##### The Neural Network
The basic concept is that each snake has *vision*, which allows it to see in 8 directions.
The snake looks in those 8 directions to see if there's food, wall or a snake body part.
These results, the snake's head direction and snake's tail direction are passed as the input to the Neural Network which then responds by choosing the best direction for the snake to move to next.

##### The Genetic Algorithm
But as you might have guessed, Neural Networks need training. Now the state-of-the-art way to train a game AI is to use Reinforcement Learning. But here I used something simpler.
First a population of random snakes is generated, and their fitness calculated.
Then we generate offsprings using crossover and mutation operators to generate a new population.
The better snakes are carried over to the next generation and this process is repeated multiple times till the best snake is achieved.

---

It is possible to achive a snake of optimal performance.