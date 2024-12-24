a = {
    "a": 1,
    "b": 48,
    "c": 84,
    "d": 40,
    "e": 80,
    "f": 81,
    "g": 3,
    "h": 33,
    "i": 117,
    "j": 44,
    "k": 113,
    "l": 116,
    "m": 57,
    "n": 57,
    "o": 56,
    "p": 65,
    "q": 20,
    "r": 121,
    "s": 18,
    "t": 112,
    "u": 60,
    "v": 36,
    "w": 36,
    "x": 51,
    "y": 34,
    "z": 72,
}

strs = [bin(value)[2:] for key, value in a.items()]
strs = [f"B1{'0'*(7 - len(s))}{s}" for s in strs]

for i in strs:
    print(i)
