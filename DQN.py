from game import DeepAxie
import shutil
import os
import datetime
import random
import numpy as np
import pandas as pd

from numpy import asarray
from numpy import savetxt


from tensorflow.keras import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.optimizers import SGD
from tensorflow.keras.optimizers import Adam

from collections import deque
import matplotlib.pyplot as plt

player_1 = 7
player_2 = 4

time = str(datetime.datetime.today())

env = DeepAxie(player_1, player_2)
np.random.seed(0)


class DQN:

    """ Implementation of deep q learning algorithm """

    def __init__(self, action_space, state_space, load_model):

        self.action_space = action_space
        self.state_space = state_space
        self.epsilon = 1
        self.gamma = .98
        self.batch_size = 64
        self.epsilon_min = .1
        self.epsilon_decay = .999
        self.learning_rate = 0.0001
        self.memory = deque(maxlen=100000)
        
        if load_model:
            self.model = load_model
        else:
            self.model = self.build_model()

    def build_model(self):
        #  to dense layer på siden av hverandre
        model = Sequential()
        model.add(Dense(64, input_shape=(self.state_space,), activation='relu'))
        model.add(Dense(64, activation='relu'))
        model.add(Dense(self.action_space, activation='linear'))
        model.compile(loss='mse', optimizer=Adam(learning_rate=self.learning_rate))
        return model

    def remember(self, state, action, reward, next_state, done):
        self.memory.append((state, action, reward, next_state, done))

    def act(self, state):

        if np.random.rand() <= self.epsilon:
            array = [0, 1, 2, 3, 4, 5, 6, 7, 8]
            ints = random.sample(array, 2)
            string_ints = [str(int) for int in ints]  # Convert each integer to a string
            str_of_ints = "".join(string_ints)  # Combine each string with a comma
            return int(str_of_ints)  # Output: 1,2,3

        act_values = self.model.predict(state)
        
        return np.argmax(act_values[0])

    def replay(self):

        if len(self.memory) < self.batch_size:
            return
        # print("size mem: ", len(self.memory))
        minibatch = random.sample(self.memory, self.batch_size)
        states = np.array([i[0] for i in minibatch])
        actions = np.array([i[1] for i in minibatch])
        rewards = np.array([i[2] for i in minibatch])
        next_states = np.array([i[3] for i in minibatch])
        dones = np.array([i[4] for i in minibatch])

        states = np.squeeze(states)
        next_states = np.squeeze(next_states)

        targets = rewards + self.gamma*(np.amax(self.model.predict_on_batch(next_states), axis=1))*(1-dones)
        targets_full = self.model.predict_on_batch(states)


        ind = np.array([i for i in range(self.batch_size)])
        targets_full[[ind], [actions]] = targets

        self.model.fit(states, targets_full, epochs=1, verbose=0)
        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay

    def saver(self, episode, ratio1, returnrewards1, returnrewards2, tot_rounds):
        df = pd.DataFrame({"episode" : np.arange(episode), "win-loss ratio" : ratio1, "reward p1" : returnrewards1, "reward p2" : returnrewards2, "rounds" : tot_rounds})
        df.to_csv("log/"+time +"/numbers.csv", index=False)
        self.model.save("log/"+time +"/model.h5")
        # print("file saved!")

def post_process_state(state, state_space):
    return np.reshape(state, (1, state_space))#/1200


def train_dqn(episode, load_model):

    returnrewards1 = []
    returnrewards2 = []
    ratio1 = []
    ratio2 = []
    tot_rounds = []

    # Actions are chosen as separate integers that are merged together, 
    # 00 being the lowest and 88 being the highest. (09,19,29,39,49,59,69,79 is not possible to get).
    action_space = 88

    # number of decimal numbers in the state.
    state_space = 88
    max_steps = 1000
    
    p1 = 0
    p2 = 0
    calc = 0
    # TODO sjekk om selected player ødelegger alt før det ble fikset!
    # selected_player = "player_1"
    agent = DQN(action_space, state_space, load_model)
    for e in range(episode):
        states = env.reset()
        inni = states['player_0'].__len__()
        states2 = {k: post_process_state(v, state_space) for k, v in states.items()}
        inn2 = states2['player_0'].__len__()
        score1 = 0
        score2 = 0
        for rounds in range(max_steps):
            actions = dict(
                #player_0 = random.randint(0,81),
                player_0 = agent.act(states2["player_0"]),
                player_1 = 0
                # player_1 = agent.act(states["player_1"])
            )
            # pick two random numbers between 0 and 8


            
            next_states, rewards, dones, infos = env.step(actions)
            inn3 = next_states['player_0'].__len__()

            next_states2 = {k: post_process_state(v,state_space) for k, v in next_states.items()}
            inn4 = next_states2.__len__()

            # score += rewards[selected_player]
            score1 += rewards["player_0"]
            score2 += rewards["player_1"]
            for (agent_name, state), (_, action), (_, reward), (_, next_state), (_, done) in zip(states.items(), actions.items(), rewards.items(), next_states.items(), dones.items()):  
                agent.remember(state, action, reward, next_state, done)
            states = next_states
            agent.replay()

            # TODO denne må endres til å se hvilken player som vinner, ikke høyest reward?
            if any(dones.values()):
                agent.saver(e, ratio1, returnrewards1, returnrewards2, tot_rounds)
                if rewards["player_0"] > rewards["player_1"]:
                    winner = "player_0"
                    p1 += 1
                else:
                    winner = "player_1"
                    p2 += 1
                # TODO legge til player til info i step function

                calc = round((p1+0.0000001)/(p1+p2+0.000001), 3)
                ratio1.append(calc)
                # ratio2.append(calc)
                print("episode: {}/{}, rounds: {}, score p1: {}, score p2: {}, winner: {}, win-ratio: {}".format(e, episode, rounds, round(score1,3), round(score2,3),winner, calc))
                break


            if rounds == (max_steps-1):
                calc = round((p1+0.0000001)/(p1+p2+0.000001), 3)
                ratio1.append(calc)
                # ratio2.append(calc)
                print("episode: {}/{}, rounds: {}, score p1: {}, score p2: {}, winner: {}, win-ratio: {}".format(e, episode, rounds, round(score1,3), round(score2,3),winner, calc))
                break

        returnrewards1.append(score1)
        returnrewards2.append(score2)
        tot_rounds.append(rounds)
    
    return returnrewards1, returnrewards2, ratio1, tot_rounds



def plotter(ep, type, data):
    fig = plt.figure(figsize=(12, 6))
    plt.plot([i for i in range(ep)], data, label=type)    
    plt.xlabel('episodes')
    plt.ylabel(type)
    fig.savefig("log/"+time +"/"+type+ '.jpg', bbox_inches='tight', dpi=150)


print("Let's GOOOO")

if __name__ == '__main__':
    
    # Create folder to log training session
    os.mkdir("log/"+time)
    
    # save files to store the hyperparameters that was used
    shutil.copy('DQN.py', "log/"+time+'/DQN.txt')
    shutil.copy('game.py', "log/"+time+'/game.txt')
    
    load_model = False
    ep = 10000
    returnrewards1, returnrewards2, ratio, rounds = train_dqn(ep, load_model)
    
    if ratio.__len__() < ep:
        ratio.append(0)
    
    plotter(ep=ep, type="win-ratio", data=ratio)
    plotter(ep=ep, type="reward", data=returnrewards1)
    plotter(ep=ep, type="rounds", data=rounds)



    