# AI-banqi
It's a simple code from NTNUCSIE programming homework 5.

## How To Use

### Step 1(include):
Include auto_banqi.h, and they are only one function below:  
```
int* auto_move(int32_t player, char chess[][8], char each_color[]);
```
### Step 2(Input format):
They are three parameter in the function:  
* player: which player is going now. 0 for player1, 1 for player2;  
* chess[][8]: input 4\*8 array with char type, which stand for the current board before moving, and every char in the array should contains only the characters{\*,1\~7,A\~G,(space)}.  
* each_color[]: to set up the color for every player. each_color[0] is player1, each_color[1] is player2. Every char in the array should contains only{1,A}.(For instance, if each_color[] = {A,1}, meaning player1 owns A\~G, player2 owns 1\~7.)

### Step 3(Output format):
The output format is a 1D-array which has 4 elements. And the first two are picking chess's coordinate x and y, and the last two are Moving coordinate x and y after picking chess.
You can also think as the first two are "Player1 (x,y):" from the problem description, and the following are "To (x,y):" from the problem description.  
Remind that if there's no moving("To (x,y):") situation, you may ignore the last two from function's output.  

### Step 4:
Link it to your program, and enjoy!
