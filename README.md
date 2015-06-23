# match-the-light-game
This game was created for an embedded systems course. It uses 8 LED lights, a keypad, and an LCD to implement a hybrid game of Simon Says and Mega Stacker. There are 8 LED lights lined up side-by-side. The computer will flash one of these 8 for 5 seconds or less. Following shortly after is the player's turn. The LED lights will light up one at a time back and forth as if a light were bouncing between them. The player must press the "*" button on the keypad to stop the bouncing and freeze the light at the current LED. If the light stops at the same LED the computer had chosen earlier, the player gains a point, displayed on the LCD. If they miss, the computer gains the point. The process repeats and after each round wherein a player gains a point, the speed of the bouncing light increases. The game ends when either player receives a total of 5 points. The name of the winner is displayed and a new game is prepared to begin.