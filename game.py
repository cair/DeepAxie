from sys import platform
if platform == "linux" or platform == "linux2":
    import DeepAxieLinux as axie
elif platform == "darwin":
    import DeepAxie as axie
elif platform == "win32":
    # Windows...
    pass

display_game_window = False
try:
    import turtle as t
    display_game_window = False
except:
    print(" Game-window is not loadable, problem loading turtle (tkinter package)")

import random
import numpy as np

ENERGY = 42

    
class DeepAxie():
    def __init__(self, player_0, player_1):
        self.done = False
        self.energy = 0
        self.roundCounter = 0
        self.player_0 = player_0
        self.player_1 = player_1

        # init game
        self.GameState = axie.GameState(player_0, player_1)
        
        if display_game_window:
            # window setup
            self.window = t.Screen()
            self.window.title('DeepAxie')
            self.window.bgcolor('black')
            self.window.setup(width=900, height=400)

            # print game board
            self.board = t.Turtle()
            self.board.speed(0)
            self.board.color('white')
            self.board.penup()
            self.board.hideturtle()
            self.board.goto(0, 0)
            self.board.write("{}".format(self.GameState.printGameBoard(self.roundCounter)), align='center', font=('Courier', 24, 'normal'),)
        # print("hold on")

        self.axie = []

    def axie_live(self):
        
        boardStat = self.GameState.playersMatrixDecimal()
        types = [boardStat[16],boardStat[37],boardStat[60],boardStat[81]]

        pos = [[-50,-20],[-50,-120],[50,-120],[50,-20]]
        positions = [boardStat[17],boardStat[38],boardStat[61],boardStat[82]]
        health = [boardStat[20],boardStat[41],boardStat[64],boardStat[85]]
        for i in range(4):
            if positions[i] == 1:
                if i < 2:
                    pos[i][0] = -150
                else:
                    pos[i][0] = 150

            self.axie.append(t.Turtle())
            self.axie[i].goto(pos[i])
            self.axie[i].speed(100000000)
            self.axie[i].shape('circle')
            self.axie[i].shapesize(stretch_wid=4, stretch_len=4)
            if types[i] == 0:
                self.axie[i].color('green')
            elif types[i] == 1:
                self.axie[i].color('blue')
            elif types[i] == 2:
                self.axie[i].color('orange')

        
    def set_terminal(self, dones, value):
        for agent in dones.keys():
            dones[agent] = value
        
    def round(self, dones, rewards, infos):
        self.roundCounter += 1
        # attack
        previous_state = self.get_state()
        winner = self.GameState.attack()
        state = self.get_state()
        has_winner = False
        player_id = 0
        for (agent, previous_state), (_, state) in zip(previous_state.items() , state.items() ):
            player_id += 1 # todo null indexing
            
            # check if the defender axie is defeated then add 1 to the reward
            # if (state[62] == 0 and previous_state[62] == 1) or (state[83] == 0 and previous_state[83] == 1):
            #     rewards[agent] += 10

            # check if the oponents axie's health has decreased and reward
            # if (state[64] < previous_state[64]) or (state[85] < previous_state[85]):
            #     rewards[agent] += 10

            # check if player axie's health has adjusted and reward
            # if (state[20] >= previous_state[20]) or (state[42] >= previous_state[42]):
                # rewards[agent] += 1
            # else:
                # rewards[agent] -= 1

            # check if player spends energy each round
            # if state[ENERGY] <= previous_state[ENERGY]:
                # rewards[agent] += 1
            # else:
                # rewards[agent] -= 1
                

            if winner == player_id:
                # rewards[agent] += 10
                self.set_terminal(dones, True) 
                has_winner = True
                self.axie.clear()
                #  f(x) = (1000 /(x+10) ) - 1  okok, promising
                #  f(x) = (100 /(x+1) ) + 0.1 
                #  f(x) = 100-self.roundCounter
                rewards[agent] += (100 / (self.roundCounter-5.00001))+0.1


            # if has_winner:
            # TODO implement if negative rewards is needed for losing
    
            # minus reward if energy increases?
            # kanskje vi må endre slik at den ikke trekker energy for "feil" kort
            # Legge inn if forrige == next energy gi minus poeng
            # self.roundCounter = 1

            # if self.roundCounter % 1 == 0 and self.done == False:
            #     rewards[agent] -= 1 #* self.roundCounter
            
        if display_game_window:
            self.board.clear()
            self.board.write("{}".format(self.GameState.printGameBoard(self.roundCounter)), align='center', font=('Courier', 24, 'normal'),)
            boardStat = self.GameState.playersMatrixDecimal()
            health = [boardStat[20],boardStat[41],boardStat[64],boardStat[85]]
            if any(n <= 0 for n in health):
                for i in range(4):
                    if health[i] <= 0:
                        try:
                            self.axie[i].color('grey')
                        except:
                            print("dead")


        return dones, rewards, infos

    def get_state(self):
        
        state_0 = self.GameState.playersMatrixDecimal()
        state_1 = state_0[207:]+state_0[:207]
        
        return dict(
                player_0 = state_0,
                player_1 = state_1 )
    
    
    def reset(self):
        # "restarts" the game
        if display_game_window:
            self.axie_live()
        self.roundCounter = 1
        self.GameState = axie.GameState(self.player_0, self.player_1)
            
        return self.get_state()


    def pickCards(self, player, action):
        # agent has to be able to chose all cards everytime, 
        # the pickcards array should be the same every round, even if the cards cannot be chosen

        # This function checks how good the agent has understood what cards can be chosen
        reward = 0
        selectable_cards = self.GameState.drawCardsFromDeck(player)
        action = [int(a) for a in str(action)]

        action_array = np.array([0,1,2,3,4,5,6,7,8])
        selectable_mask_array = np.zeros(9)
        action_mask_array = np.zeros(9)

        for i in range(len(action_array)):
            for j in range(len(selectable_cards)):
                if selectable_cards[j] == action_array[i]:
                    selectable_mask_array[i] = 1

        for i in range(len(action_array)):
            for j in range(len(action)):
                if action[j] == action_array[i]:
                    action_mask_array[i] = 1
        
        #print(selectable_cards)
        #print(selectable_mask_array)
        #print(action_mask_array)
        
        # for i in range(len(action_array)):
        #     if action_mask_array[i] == 1 and selectable_mask_array[i] == 1:
        #         reward += 1

        
        # not that many zeroes are chosen, reason for many rounds is choosing cards that are not available
                
        self.GameState.chooseCards(player, action)

        return reward
    

    def action_2(self):
        array = [0, 1, 2, 3, 4, 5, 6, 7, 8]
        ints = random.sample(array, 2)
        string_ints = [str(int) for int in ints]  # Convert each integer to a string
        str_of_ints = "".join(string_ints)  # Combine each string with a comma
        return int(str_of_ints)  # Output: 1,2,3


    def step(self, actions):
        #  action is an array of two actions
        
        rewards = dict()
        dones = dict()
        terminals = dict()
        infos = dict()
        
        for i, (agent, action) in enumerate(actions.items()):
            dones[agent] = 0
            rewards[agent] = self.pickCards(i+1, action)
            infos[agent] = dict()

        #  attack is run inside round()
        self.round(dones, rewards, infos)
        # self.roundCounter += 1
        state = self.GameState.playersMatrixDecimal()
        # state = self.GameState.playersMatrixBits()

        return self.get_state(), rewards, dones, infos


# env = DeepAxie(1,5)



# print("Done!")