"""
Our Python sample module
"""
import sys
import random
import emb

def print_random_word(num=5):
    """ Generate a random word then print to screen.

    Args:
        num (int): Number of random letters in word to generate
    """
    some_string = ''.join(random.sample('abcdefghijklmnopqrstuvwyxz', num))
    print (sys.version)
    print('Python says:', some_string)

print (sys.version)
print('hello python!!')
emb.func1()
