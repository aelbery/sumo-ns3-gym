# Custom OpenAI Gym Environment

This environment is a custom gym environment for the sole purpose of training the agent.

## Environment Installation

To install and register the environment with OpenAI Gym run the following script in the working directory of the **setup.py** file.

```bash 
pip3 install -e .
```

## Environment Usage

After installing and registering the custom environment you can use it with the following code block:

```bash 
import gym
import gym_rsu

env = gym.make('rsu-v0')
```

## Running script file
To run the **script.py** file in the custom environment directory type the following command:
```bash
# If you want to train your agent then you also need to
# specify if you are training online or offline.
python3 script.py [ test | train ] [ --online | --offline ]
```
